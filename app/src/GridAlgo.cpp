#include "GridAlgo.h"
#include "struct/Grid.h"
#include "struct/Drone.h"
#include "struct/GridAlgoConfig.h"
#include "struct/Result.h"
#include <chrono>
#include <limits>
#include <stdexcept>
#include <array>

std::span<const GridAlgo::Move> GridAlgo::buildMoves(bool allowStay) noexcept {
    static constexpr std::array<Move,4> k4 {{
        { 1, 0}, {-1, 0}, {0, 1}, {0,-1}
    }};
    static constexpr std::array<Move,5> k4s {{
        { 1, 0}, {-1, 0}, {0, 1}, {0,-1}, {0,0}
    }};
    return allowStay ? std::span<const Move>(k4s) : std::span<const Move>(k4);
}

int GridAlgo::collectAndUpdate(Grid& grid, Drone& drone, int x, int y, int t) noexcept {
    const int gain = grid.valueAt(x, y, t);
    drone.moveTo(x, y, t, gain);
    // prefer encapsulated mutation
    grid.markVisited(x, y, t);
    return gain;
}

std::pair<int,int> GridAlgo::findBestMove(
    const Grid& grid,
    const Drone& drone,
    std::span<const Move> moves,
    int tNow,
    int horizon
) const noexcept {
    long long bestGain = std::numeric_limits<long long>::min();
    int bestDx = 0, bestDy = 0;

    for (auto [dx1, dy1] : moves) {
        const auto p1 = drone.pos();
        const int nx1 = p1.x + dx1;
        const int ny1 = p1.y + dy1;
        if (!grid.inBounds(nx1, ny1)) continue;

        const int gain1 = grid.valueAt(nx1, ny1, tNow);
        long long combined = gain1;

        if (horizon >= 2) {
            const std::size_t idx1 = Grid::idx(nx1, ny1, grid.N);
            for (auto [dx2, dy2] : moves) {
                const auto p2 = drone.pos();
                const int nx2 = nx1 + dx1;
                const int ny2 = ny1 + dy1;
                if (!grid.inBounds(nx2, ny2)) continue;
                const int gain2 = grid.valueAtWithOverride(nx2, ny2, tNow + 1, idx1, tNow);
                const long long twoStep = static_cast<long long>(gain1) + gain2;
                if (twoStep > combined) combined = twoStep;
            }
        }

        if (combined > bestGain) {
            bestGain = combined;
            bestDx = dx1; bestDy = dy1;
        }
    }
    return {bestDx, bestDy};
}

RunResult GridAlgo::run(Grid& grid, std::span<Drone> drones, const GridAlgoConfig& cfg) {
    if (drones.empty()) throw std::runtime_error("No drones to run algorithm");

    const int horizon = (cfg.horizon < 1) ? 1 : (cfg.horizon > 2 ? 2 : cfg.horizon);
    const auto moves = buildMoves(cfg.allowStay);

    const auto tStart = std::chrono::steady_clock::now();
    RunResult result;
    result.drones = static_cast<int>(drones.size());
    result.paths.reserve(drones.size());

    // t = 0 initialization
    for (auto& d : drones) {
        const auto p = d.pos();
        result.totalScore += collectAndUpdate(grid, d, p.x, p.y, /*t=*/0);
    }

    // main loop
    for (int tNow = 1; tNow < cfg.totalSteps; ++tNow) {
        const auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::steady_clock::now() - tStart).count();
        if (elapsed >= cfg.timeBudgetMs) break;

        for (auto& d : drones) {
            const auto p = d.pos();
            auto [dx, dy] = findBestMove(grid, d, moves, tNow, horizon);
            int nx = p.x + dx;
            int ny = p.y + dy;
            if (!grid.inBounds(nx, ny)) { nx = p.x; ny = p.y; }
            result.totalScore += collectAndUpdate(grid, d, nx, ny, tNow);
        }
    }

    result.timeElapsedMs = static_cast<int>(
        std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::steady_clock::now() - tStart).count()
    );

    // extract paths (no double-reserve)
    for (auto& d : drones) {
        result.paths.push_back(DronePath{ d.id(), d.path() });
    }
    return result;
}
