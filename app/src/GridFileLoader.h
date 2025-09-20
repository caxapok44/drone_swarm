#pragma once
#include <filesystem>
#include <memory>
#include "interfaces/IGridLoader.h"

class GridFileLoader final : public IGridLoader {
public:
    GridFileLoader(std::filesystem::path filePath, double regrowthRate);
    [[nodiscard]] std::unique_ptr<Grid> loadGrid() const override;

private:
    static Grid parseText(const std::string& text, double regrowthRate);

    const std::filesystem::path m_filePath;
    const double                m_regrowthRate;
};
