// io/json.h
#pragma once
#include <ostream>
#include <sstream>
#include <locale>
#include <algorithm> // for std::max
#include "../struct/Result.h"

namespace io {

// Write JSON to a stream; pretty = human-readable, otherwise compact.
inline std::ostream& write_json(std::ostream& os, const RunResult& r,
                                bool pretty = true, int indent_size = 2) {
    // Ensure locale-independent digits (e.g., no grouped thousands)
    os.imbue(std::locale::classic());

    const char* nl = pretty ? "\n" : "";
    const char* sp = pretty ? " "  : "";
    const std::string indent_unit(pretty ? std::max(0, indent_size) : 0, ' ');
    auto indent = [&](int level) {
        if (pretty) for (int i = 0; i < level; ++i) os << indent_unit;
    };

    os << "{" << nl;

    indent(1); os << "\"score\":" << sp << r.totalScore << "," << nl;
    indent(1); os << "\"drones\":" << sp << r.drones << "," << nl;
    indent(1); os << "\"time_elapsed_ms\":" << sp << r.timeElapsedMs << "," << nl;

    indent(1); os << "\"paths\":" << sp << "[" << nl;
    for (std::size_t i = 0; i < r.paths.size(); ++i) {
        const auto& p = r.paths[i];
        indent(2); os << "{" << nl;

        indent(3); os << "\"drone_id\":" << sp << p.droneId << "," << nl;
        indent(3); os << "\"steps\":"    << sp << p.path.size() << "," << nl;

        indent(3); os << "\"path\":" << sp << "[" << nl;
        for (std::size_t j = 0; j < p.path.size(); ++j) {
            const auto& s = p.path[j];
            indent(4);
            os << "{"
               << "\"t\":"     << sp << s.timeStep << ","
               << sp << "\"x\":" << sp << s.x       << ","
               << sp << "\"y\":" << sp << s.y       << ","
               << sp << "\"value\":" << sp << s.valueCollected
               << "}";
            if (j + 1 < p.path.size()) os << ",";
            os << nl;
        }
        indent(3); os << "]" << nl;

        indent(2); os << "}";
        if (i + 1 < r.paths.size()) os << ",";
        os << nl;
    }
    indent(1); os << "]" << nl;

    os << "}" << nl;
    return os;
}

// Convenience: produce a std::string
inline std::string to_json(const RunResult& r, bool pretty = true, int indent_size = 2) {
    std::ostringstream oss;
    write_json(oss, r, pretty, indent_size);
    return oss.str();
}

} // namespace io
