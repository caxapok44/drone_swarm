// io/json.h
#pragma once
#include <ostream>
#include <sstream>
#include <locale>
#include "Result.h"

// struct/GridAlgoConfig.h
#pragma once

struct GridAlgoConfig {
    int  totalSteps   = 1;   // >=1
    int  timeBudgetMs = 0;   // >=0 (0 = без ліміту)
    int  horizon      = 1;   // підтримуємо 1..2 (кліпиться в алгоритмі)
    bool allowStay    = true;
};