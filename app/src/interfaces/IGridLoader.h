#pragma once

#include <memory>

class Grid;

class IGridLoader {
public:
    virtual ~IGridLoader() = default;
    
    [[nodiscard]] virtual std::unique_ptr<Grid> loadGrid() const = 0;
};