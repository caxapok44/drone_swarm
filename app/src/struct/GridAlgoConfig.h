#pragma once

// Configuration parameters for grid-based algorithms
struct GridAlgoConfig {
    int  totalSteps   = 1;
    int  timeBudgetMs = 0;  
    int  horizon      = 1;   
    bool allowStay    = true;
};