#pragma once
#include <span>

// Forward declarations
class Grid;
class Drone;

struct GridAlgoConfig;
struct RunResult;

class IGridAlgo {
public:
    virtual ~IGridAlgo() = default;

    [[nodiscard]] virtual RunResult run(Grid& grid,
                                        std::span<Drone> drones,
                                        const GridAlgoConfig& cfg) = 0;
};
