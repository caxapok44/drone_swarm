#pragma once

#include <memory>
#include <vector>
#include <span>

#include "struct/Position.h"
#include "struct/Drone.h"
#include "struct/Grid.h"
#include "struct/GridAlgoConfig.h"
#include "interfaces/IGridLoader.h"
#include "interfaces/IGridAlgo.h"

class GridHandler {
public:
    GridHandler(std::unique_ptr<IGridLoader> gridLoader,
                std::unique_ptr<IGridAlgo>   gridAlgo,
                std::vector<Position>        startPositions,
                GridAlgoConfig               cfg);

    ~GridHandler();

    void loadGrid();   // may throw
    void run();        // may throw

    GridHandler(const GridHandler&) = delete;
    GridHandler& operator=(const GridHandler&) = delete;
    GridHandler(GridHandler&&) noexcept = default;
    GridHandler& operator=(GridHandler&&) noexcept = default;

private:
    void initializeDrones();

private:
    std::unique_ptr<IGridLoader> m_gridLoader;
    std::unique_ptr<IGridAlgo>   m_gridAlgo;
    std::unique_ptr<Grid>        m_grid;

    std::vector<Drone>           m_drones;
    GridAlgoConfig               m_cfg;
};
