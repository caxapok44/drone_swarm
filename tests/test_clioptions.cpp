#include <gtest/gtest.h>
#include "CLIOptions.h"

TEST(CLIOptionsUsageTest, ContainsRequiredFlags) {
    std::string u = CLIOptions::usage("app");
    EXPECT_NE(u.find("--file"), std::string::npos);
    EXPECT_NE(u.find("--steps"), std::string::npos);
    EXPECT_NE(u.find("--time_ms"), std::string::npos);
    EXPECT_NE(u.find("--start_x"), std::string::npos);
    EXPECT_NE(u.find("--start_y"), std::string::npos);
}

TEST(CLIOptionsParseTest, HelpReturnsFalseAndDoesNotThrow) {
    const char* argv[] = {"app", "--help"};
    CLIOptions opt(2, const_cast<char**>(argv));
    EXPECT_NO_THROW({
        bool ok = opt.parseCLI();
        EXPECT_FALSE(ok);
    });
}


