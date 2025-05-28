#pragma once

#include "automaton/grid/grid_2d.hpp"
#include "automaton/grid/grid_3d.hpp"
#include "automaton/logic/base_logic.hpp"

#include <random>

namespace automaton::logic {

class fall_2d : public base_logic {
public:
    explicit fall_2d(base_grid_ptr grid);
    bool step() override;

private:
    grid_2d_ptr _grid;

    std::random_device rd;
    std::default_random_engine _engine;
    std::uniform_int_distribution<int16_t> _dist;
};

class fall_3d : public base_logic {
public:
    explicit fall_3d(base_grid_ptr grid, uint16_t slices = 0);
    bool step() override;

private:
    grid_3d_ptr _grid;
    uint16_t _slices;

    std::default_random_engine _engine;
    std::uniform_int_distribution<int16_t> _dist;
};

}  // namespace automaton::logic
