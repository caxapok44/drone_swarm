#include "GridHandler.h"
#include <stdexcept>
#include <utility>
#include "struct/GridAlgoConfig.h"
#include "io/json.h"
#include <iostream>
struct GridError : std::runtime_error { using std::runtime_error::runtime_error; };
struct AlgoError : std::runtime_error { using std::runtime_error::runtime_error; };

GridHandler::GridHandler(std::unique_ptr<IGridLoader> gridLoader,
                         std::unique_ptr<IGridAlgo>   gridAlgo,
                         std::vector<Position>        startPositions,
                         GridAlgoConfig               cfg)
    : m_gridLoader(std::move(gridLoader))
    , m_gridAlgo(std::move(gridAlgo))
    , m_grid(nullptr)
    , m_drones()
    , m_cfg(std::move(cfg))
{
    if (!m_gridLoader) {
        throw GridError("GridLoader is null");
    }
    if (!m_gridAlgo)   {
        throw GridError("GridAlgo is null");
    }

    m_drones.reserve(startPositions.size());
    for (std::size_t i = 0; i < startPositions.size(); ++i) {
        m_drones.emplace_back(static_cast<int>(i), Position{ startPositions[i].x, startPositions[i].y });
    }
}

GridHandler::~GridHandler() = default;

void GridHandler::loadGrid()
{
    try {
        m_grid = m_gridLoader->loadGrid();
    } catch (const std::exception& e) {
        throw GridError(std::string("Failed to load grid: ") + e.what());
    }

    if (!m_grid) {
        throw GridError("Loader returned null grid");
    }

    // Validate start positions
    for (const auto& drone : m_drones) {
        const auto start_pos = drone.start();
        if (!m_grid->inBounds(start_pos.x, start_pos.y)) {
            const int N = m_grid->N;
            throw GridError("Start position out of bounds: (" +
                            std::to_string(start_pos.x) + "," + std::to_string(start_pos.y) +
                            "); valid ranges: x∈[0," + std::to_string(N-1) +
                            "], y∈[0," + std::to_string(N-1) + "]");
        }
    }
}

void GridHandler::initializeDrones()
{
    for (auto& drone : m_drones) {
        drone.resetToStart(m_cfg.totalSteps);
    }
}

void GridHandler::run()
{
    if (!m_grid) {
        throw AlgoError("Grid not loaded");
    }

    initializeDrones();

    try {
        auto result = m_gridAlgo->run(*m_grid, std::span<Drone>(m_drones), m_cfg);
        io::write_json(std::cout, result);
    } catch (const std::exception& e) {
        throw AlgoError(std::string("Algorithm failed: ") + e.what());
    }
}
