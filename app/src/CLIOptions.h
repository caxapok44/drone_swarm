#pragma once

#include <string>
#include <filesystem>

struct Options {
    std::filesystem::path file;
    int totalSteps;
    int timeBudgetMs;
    int startX;
    int startY;
    double regrowthRate; // [0.0, 1.0]
    int horizon;         // 1 or 2
    bool allowStay;
};

class CLIOptions {
public:
    CLIOptions() = default;
    CLIOptions(int argc, char** argv) : argc(argc), argv(argv) {}
    ~CLIOptions() = default;

    [[nodiscard]] bool parseCLI();

    static std::string usage(const char* argv0);

    // Accessors
    [[nodiscard]] std::filesystem::path filePath() const { return std::filesystem::path{m_filePath}; }
    [[nodiscard]] const std::string&    filePathStr() const noexcept { return m_filePath; }
    [[nodiscard]] int    totalSteps()   const noexcept { return m_totalSteps; }
    [[nodiscard]] int    timeBudgetMs() const noexcept { return m_timeBudgetMs; }
    [[nodiscard]] int    startX()       const noexcept { return m_startX; }
    [[nodiscard]] int    startY()       const noexcept { return m_startY; }
    [[nodiscard]] double regrowthRate() const noexcept { return m_regrowthRate; }
    [[nodiscard]] int    horizon()      const noexcept { return m_horizon; }
    [[nodiscard]] bool   allowStay()    const noexcept { return m_allowStay; }

    [[nodiscard]] Options toOptions() const;

private:
    int argc = 0;
    char** argv = nullptr;

    void        loadConfigFile(const std::string& path);
    static bool parseBool(const std::string& s);

    std::string m_filePath;
    int    m_totalSteps   = -1;
    int    m_timeBudgetMs = -1;
    int    m_startX       = 0;
    int    m_startY       = 0;
    double m_regrowthRate = 0.0;
    int    m_horizon      = 2;
    bool   m_allowStay    = true;
};
