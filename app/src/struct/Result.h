#pragma once
#include <vector>
#include "Step.h"

// Path taken by a single drone
struct DronePath {
    int                droneId;
    std::vector<Step>  path;
};

// Result of a full run with one or more drones
struct RunResult {
    long long          totalScore   = 0;
    int                drones       = 0;
    int                timeElapsedMs= 0;
    std::vector<DronePath> paths;
};
