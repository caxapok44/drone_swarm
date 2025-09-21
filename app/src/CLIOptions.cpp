#include "CLIOptions.h"
#include <fstream>
#include <sstream>
#include <stdexcept>

namespace {
    int toInt(const std::string& who, const std::string& s) {
        try { return std::stoi(s); }
        catch (...) { throw std::runtime_error(who + " expects integer, got '" + s + "'"); }
    }
    double toDouble(const std::string& who, const std::string& s) {
        try { return std::stod(s); }
        catch (...) { throw std::runtime_error(who + " expects floating point, got '" + s + "'"); }
    }
}

bool CLIOptions::parseCLI() {
    // pass 1: --config
    for (int i = 1; i < argc; ++i) {
        std::string a = argv[i];
        if (a == "--config") {
            if (i + 1 >= argc)
            {
                throw std::runtime_error("Missing value for --config");
            } 
            loadConfigFile(argv[++i]);
        }
    }

    // pass 2: args
    for (int i = 1; i < argc; ++i) {
        std::string a = argv[i];
        auto needValue = [&](const std::string& name) -> std::string {
            if (i + 1 >= argc) 
            {
                throw std::runtime_error("Missing value for " + name);
            }
            return std::string(argv[++i]);
        };

        if (a == "--file")               m_filePath     = needValue(a);
        else if (a == "--steps")         m_totalSteps   = toInt(a, needValue(a));
        else if (a == "--time_ms")       m_timeBudgetMs = toInt(a, needValue(a));
        else if (a == "--start_x")       m_startX       = toInt(a, needValue(a));
        else if (a == "--start_y")       m_startY       = toInt(a, needValue(a));
        else if (a == "--regrowth_rate") m_regrowthRate = toDouble(a, needValue(a));
        else if (a == "--horizon")       m_horizon      = toInt(a, needValue(a));
        else if (a == "--no-stay")       m_allowStay    = false;
        else if (a == "--allow-stay")    m_allowStay    = true;
        else if (a == "--config")        { ++i; continue; }
        else if (a == "-h" || a == "--help") {
            return false;
        }
        else if (!a.empty() && a[0] != '-') {
            m_filePath = a; // positional file
        }
        else {
            throw std::runtime_error("Unknown argument: " + a);
        }
    }

    // Required
    if (m_filePath.empty())  
    {
        throw std::runtime_error("Missing required option: --file <path>");
    }
    if (m_totalSteps <= 0)   
    {
        throw std::runtime_error("--steps must be a positive integer");
    }
    if (m_timeBudgetMs <= 0) 
    {
        throw std::runtime_error("--time_ms must be a positive integer (milliseconds)");
    }

    // Ranges
    if (m_horizon < 1 || m_horizon > 2)
    {
        throw std::runtime_error("--horizon must be 1 or 2");
    }
    if (m_regrowthRate < 0.0 || m_regrowthRate > 1.0)
    {
        throw std::runtime_error("--regrowth_rate must be in [0.0, 1.0]");
    }

    return true;
}

bool CLIOptions::parseBool(const std::string& s) {
    return s == "1" || s == "true" || s == "TRUE" || s == "True" || s == "yes";
}

void CLIOptions::loadConfigFile(const std::string& path) {
    std::ifstream in(path);
    if (!in) 
    {
        throw std::runtime_error("Failed to open config file: " + path);
    }
    std::string line;
    while (std::getline(in, line)) 
    {
        auto l = line.find_first_not_of("\t \r\n");
        auto r = line.find_last_not_of("\t \r\n");
        if (l == std::string::npos) {
            continue;
        }
        std::string trimmed = line.substr(l, r - l + 1);
        if (trimmed.empty() || trimmed[0] == '#') {
            continue;
        }
        auto eq = trimmed.find('=');
        if (eq == std::string::npos) {
            continue;
        }
        std::string key = trimmed.substr(0, eq);
        std::string val = trimmed.substr(eq + 1);
        auto l2 = key.find_first_not_of("\t \r\n");
        auto r2 = key.find_last_not_of("\t \r\n");
        key = key.substr(l2, r2 - l2 + 1);
        auto l3 = val.find_first_not_of("\t \r\n");
        auto r3 = val.find_last_not_of("\t \r\n");
        val = val.substr(l3, r3 - l3 + 1);

        if      (key == "file")          m_filePath     = val;
        else if (key == "steps")         m_totalSteps   = toInt("steps", val);
        else if (key == "time_ms")       m_timeBudgetMs = toInt("time_ms", val);
        else if (key == "start_x")       m_startX       = toInt("start_x", val);
        else if (key == "start_y")       m_startY       = toInt("start_y", val);
        else if (key == "regrowth_rate") m_regrowthRate = toDouble("regrowth_rate", val);
        else if (key == "horizon")       m_horizon      = toInt("horizon", val);
        else if (key == "allow_stay")    m_allowStay    = parseBool(val);
    }
}

std::string CLIOptions::usage(const char* argv0) {
    std::ostringstream ss;
    ss << "Usage:\n"
       << "  " << argv0 << " --file <path> --steps <t> --time_ms <T> --start_x <x> --start_y <y>\n"
       << "               [--regrowth_rate <r>] [--horizon <1|2>] [--allow-stay|--no-stay] [--config <cfg>]\n\n"
       << "Input file format:\n"
       << "  First line: N (grid size)\n"
       << "  Next N lines: N integers per line (initial cell scores)\n";
    return ss.str();
}

Options CLIOptions::toOptions() const {
    return Options{
        /*file*/         std::filesystem::path{m_filePath},
        /*totalSteps*/   m_totalSteps,
        /*timeBudgetMs*/ m_timeBudgetMs,
        /*startX*/       m_startX,
        /*startY*/       m_startY,
        /*regrowthRate*/ m_regrowthRate,
        /*horizon*/      m_horizon,
        /*allowStay*/    m_allowStay
    };
}
