#pragma once
#include <vector>
#include <cstddef>
#include <limits>
#include <stdexcept>
#include <utility>

using CellValue = int;
using TimeStep  = int;

class Grid {
public:
    Grid() = default;

    // Initialize grid storage to N x N. Throws on invalid N or overflow.
    void initialize(int n, CellValue defaultBase = 0, CellValue defaultInc = 0) {
        if (n <= 0) throw std::runtime_error("Grid::initialize: N must be positive");
        const std::size_t Nsz = static_cast<std::size_t>(n);
        // overflow-safe check for N*N
        if (Nsz > std::numeric_limits<std::size_t>::max() / Nsz) {
            throw std::runtime_error("Grid::initialize: N too large (overflow)");
        }
        const std::size_t total = Nsz * Nsz;

        N = n;
        base.assign(total, defaultBase);
        inc.assign(total, defaultInc);
        lastVisitTime.assign(total, -1);
    }

    // Row-major index helpers
    static constexpr std::size_t idx(int x, int y, int n) noexcept {
        return static_cast<std::size_t>(y) * static_cast<std::size_t>(n)
             + static_cast<std::size_t>(x);
    }
    constexpr std::size_t idx(int x, int y) const noexcept {
        return idx(x, y, N);
    }

    [[nodiscard]] bool inBounds(int x, int y) const noexcept {
        return x >= 0 && y >= 0 && x < N && y < N;
    }

    // Collectible value at (x,y) given current time; never exceeds base.
    [[nodiscard]] CellValue valueAt(int x, int y, TimeStep tNow) const {
        const std::size_t k = idx(x, y);
        const CellValue b   = base[k];
        const TimeStep  lv  = lastVisitTime[k];

        if (lv < 0) 
        {
            return b;
        } // never visited
        const long long stepsSince = static_cast<long long>(tNow) - static_cast<long long>(lv);
        if (stepsSince <= 0) 
        {
            return 0;
        }

        const long long grow = static_cast<long long>(inc[k]) * stepsSince;
        return static_cast<CellValue>(grow >= b ? b : grow);
    }

    // Same as valueAt but with temporary override for a single cell's last-visit time
    [[nodiscard]] CellValue valueAtWithOverride(
        int x, int y, TimeStep tNow, std::size_t overrideIndex, TimeStep overrideLV
    ) const 
    {
        const std::size_t k = idx(x, y);
        const CellValue b   = base[k];
        const TimeStep  lv  = (k == overrideIndex) ? overrideLV : lastVisitTime[k];

        if (lv < 0) return b;
        const long long stepsSince = static_cast<long long>(tNow) - static_cast<long long>(lv);
        if (stepsSince <= 0) return 0;

        const long long grow = static_cast<long long>(inc[k]) * stepsSince;
        return static_cast<CellValue>(grow >= b ? b : grow);
    }

    void markVisited(int x, int y, TimeStep t) {
        if (!inBounds(x, y)) 
        {
            throw std::out_of_range("Grid::markVisited: out of bounds");
        }
        lastVisitTime[idx(x, y)] = t;
    }

    // Convenient accessors
    [[nodiscard]] int side() const noexcept { return N; }
    [[nodiscard]] int size() const noexcept { return N; }
    [[nodiscard]] std::pair<int,int> dimensions() const noexcept { return {N, N}; }

    int N = 0;
    std::vector<CellValue> base;
    std::vector<CellValue> inc;
    std::vector<TimeStep>  lastVisitTime;
};
