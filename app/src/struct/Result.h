#pragma once
#include <vector>
#include "Step.h"

struct DronePath {
    int                droneId;
    std::vector<Step>  path;
};

struct RunResult {
    long long          totalScore   = 0;
    int                drones       = 0;
    int                timeElapsedMs= 0;
    std::vector<DronePath> paths;
};
