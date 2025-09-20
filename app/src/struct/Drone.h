#pragma once
#include <vector>
#include <array>
#include <span>
#include <utility>
#include <cassert>
#include "Step.h"
#include "Position.h"

class Drone {
public:
    Drone(int id, Position start) noexcept
        : m_id(id), m_start(start), m_pos(start) {}

    void resetToStart(int stepsHint = 0) {
        m_pos = m_start;
        m_path.clear();
        if (stepsHint > 0) m_path.reserve(static_cast<size_t>(stepsHint));
    }

    void moveTo(int newX, int newY, TimeStep timeStep, CellValue valueCollected) {
        // assert(isAdjacent(m_pos.x, m_pos.y, newX, newY) && "illegal move");
        m_pos = {newX, newY};
        m_path.push_back(Step{timeStep, newX, newY, valueCollected});
    }

    struct Move { int dx; int dy; };

    static std::span<const Move> moves8(bool allowStay) noexcept {
        static constexpr std::array<Move, 8> k8 {{
            {-1,-1},{0,-1},{1,-1},
            {-1, 0},        {1, 0},
            {-1, 1},{0, 1},{1, 1}
        }};
        static constexpr std::array<Move, 9> k8s {{
            {-1,-1},{0,-1},{1,-1},
            {-1, 0},        {1, 0},
            {-1, 1},{0, 1},{1, 1},
            {0,0}
        }};
        return allowStay ? std::span<const Move>(k8s) : std::span<const Move>(k8);
    }

    // Accessors
    int id() const noexcept { return m_id; }
    Position start() const noexcept { return m_start; }
    Position pos() const noexcept { return m_pos; }
    const std::vector<Step>& path() const noexcept { return m_path; }

private:
    static bool isAdjacent(int x, int y, int nx, int ny) noexcept {
        const int dx = nx - x, dy = ny - y;
        return dx >= -1 && dx <= 1 && dy >= -1 && dy <= 1;
    }

private:
    int m_id;
    Position m_start;
    Position m_pos;
    std::vector<Step> m_path;
};
