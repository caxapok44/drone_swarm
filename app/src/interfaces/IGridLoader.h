#pragma once

#include <memory>

class Grid;

class IGridLoader {
public:
    virtual ~IGridLoader() = default;

    // Loader should not mutate its own state
    [[nodiscard]] virtual std::unique_ptr<Grid> loadGrid() const = 0;
};