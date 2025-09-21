#pragma once
#include <span>
#include <vector>
#include <utility>
#include "interfaces/IGridAlgo.h"
#include "struct/GridAlgoConfig.h"
#include "struct/Result.h"

// Forward declarations to reduce coupling
class Grid;
class Drone;

class GridAlgo final : public IGridAlgo {
public:
    [[nodiscard]] RunResult run(Grid& grid, std::span<Drone> drones, const GridAlgoConfig& cfg) override;

private:
    struct Move { int dx; int dy; };

    static std::span<const Move> buildMoves(bool allowStay) noexcept;

    std::pair<int,int> findBestMove(
        const Grid& grid,
        const Drone& drone,
        std::span<const Move> moves,
        int tNow,
        int horizon
    ) const noexcept;

    static int collectAndUpdate(Grid& grid, Drone& drone, int x, int y, int t) noexcept;
};
