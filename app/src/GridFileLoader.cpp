#include "GridFileLoader.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>
#include <cctype>
#include <cmath>
#include "struct/Grid.h"
#include "io/json.h"
struct GridLoadError : std::runtime_error { using std::runtime_error::runtime_error; };

static inline std::string trim(const std::string& s) {
    size_t l = 0, r = s.size();
    while (l < r && std::isspace(static_cast<unsigned char>(s[l]))) ++l;
    while (r > l && std::isspace(static_cast<unsigned char>(s[r-1]))) --r;
    return s.substr(l, r - l);
}

GridFileLoader::GridFileLoader(std::filesystem::path filePath, double regrowthRate)
    : m_filePath(std::move(filePath))
    , m_regrowthRate(regrowthRate)
{
    if (m_regrowthRate < 0.0) {
        throw GridLoadError("regrowthRate must be >= 0");
    }
}

std::unique_ptr<Grid> GridFileLoader::loadGrid() const {
    std::ifstream in(m_filePath);
    if (!in) {
        throw GridLoadError("Failed to open file: " + m_filePath.string());
    }
    std::ostringstream oss;
    oss << in.rdbuf();

    try {
        Grid g = parseText(oss.str(), m_regrowthRate);
        return std::make_unique<Grid>(std::move(g));
    } catch (const std::exception& e) {
        throw GridLoadError("While parsing '" + m_filePath.string() + "': " + std::string(e.what()));
    }
}

Grid GridFileLoader::parseText(const std::string& text, double regrowthRate) {
    std::vector<std::string> lines;
    lines.reserve(128);

    std::istringstream iss(text);
    std::string line;
    while (std::getline(iss, line)) {
        // Strip comments starting with '#' (optional)
        if (auto pos = line.find('#'); pos != std::string::npos) {
            line.erase(pos);
        }
        line = trim(line);
        if (!line.empty()) lines.emplace_back(std::move(line));
    }

    if (lines.empty()) {
        throw GridLoadError("Empty grid file");
    }

    auto tokenize = [](const std::string& s, int lineNo) -> std::vector<int> {
        std::vector<int> out;
        std::istringstream ls(s);
        int v;
        while (ls >> v) {
            out.push_back(v);
        }
        if (!ls.eof()) {
            throw GridLoadError("Non-integer token at line " + std::to_string(lineNo));
        }
        return out;
    };

    // Header: N
    const auto header = tokenize(lines[0], /*lineNo=*/1);
    if (header.size() != 1) {
        throw GridLoadError("First line must contain a single integer N");
    }

    const int N = header[0];
    if (N <= 0) {
        throw GridLoadError("N must be positive");
    }

    // Guard against overflow 
    constexpr int kMaxN = 10'000;
    if (N > kMaxN) {
        throw GridLoadError("N too large: " + std::to_string(N));
    }

    if (static_cast<int>(lines.size()) - 1 < N) {
        throw GridLoadError("Not enough grid rows after header N=" + std::to_string(N) +
                            "; provided=" + std::to_string(lines.size() - 1));
    }

    std::vector<std::vector<int>> rows;
    rows.reserve(N);
    for (int i = 0; i < N; ++i) {
        rows.push_back(tokenize(lines[1 + i], /*lineNo=*/2 + i));
        if (static_cast<int>(rows.back().size()) != N) {
            throw GridLoadError("Row " + std::to_string(2 + i) +
                                " must have exactly N integers (found " +
                                std::to_string(rows.back().size()) + ")");
        }
    }

    Grid g;
    g.N = N;
    const std::size_t total = static_cast<std::size_t>(N) * static_cast<std::size_t>(N);

    g.base.resize(total);
    g.lastVisitTime.assign(total, -1);
    for (int y = 0; y < N; ++y) {
        for (int x = 0; x < N; ++x) {
            const int v = rows[y][x];
            g.base[static_cast<std::size_t>(g.idx(x, y, N))] = (v < 0 ? 0 : v);
        }
    }

    g.inc.resize(total);
    for (std::size_t i = 0; i < total; ++i) {
        const int b = g.base[i];
        long long incLL = std::llround(static_cast<double>(b) * regrowthRate);
        int inc = static_cast<int>(incLL);

        if (regrowthRate > 0.0 && b > 0 && inc <= 0) {
            inc = 1; // ensure minimal positive increment when regrowth enabled
        }
        if (regrowthRate == 0.0) {
            inc = 0; 
        }
        g.inc[i] = inc;
    }

    return g;
}
