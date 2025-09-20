#include <iostream>
#include <memory>
#include "CLIOptions.h"
#include "GridHandler.h"
#include "GridFileLoader.h"
#include "struct/GridAlgoConfig.h"
#include "GridAlgo.h"
int main(int argc, char** argv) {
    try {
        CLIOptions opt(argc, argv);
        if (!opt.parseCLI()) {
            std::cout << CLIOptions::usage(argv[0]);
            return 0;
        }

        std::vector<Position> startPositions = { { opt.startX(), opt.startY() } };

        auto loader = std::make_unique<GridFileLoader>(opt.filePath(),
                                                       opt.regrowthRate());

        GridAlgoConfig cfg{ opt.totalSteps(), opt.timeBudgetMs(), opt.horizon(), opt.allowStay() };
        std::unique_ptr<IGridAlgo> algo = std::make_unique<GridAlgo>();
        GridHandler handler(std::move(loader), std::move(algo), startPositions, cfg);

        handler.loadGrid();
        handler.run();
        return 0;

    } catch (const std::exception& e) {
        std::cerr << "[fatal] " << e.what() << "\n";
        return 1;
    }
}
