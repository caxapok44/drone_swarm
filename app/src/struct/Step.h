#pragma once
#include <cstdint>

using TimeStep  = int;
using CellValue = int;

// Atomic unit of a drone's path
struct Step {
    TimeStep  timeStep;
    int       x;
    int       y;
    int       valueCollected;
};
