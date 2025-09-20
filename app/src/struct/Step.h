#pragma once
#include <cstdint>

// Domain aliases kept simple (узгоджені з Grid/Result)
using TimeStep  = int;
using CellValue = int;

// Atomic unit of a drone's path
struct Step {
    TimeStep  timeStep;
    int       x;
    int       y;
    int       valueCollected;
};
