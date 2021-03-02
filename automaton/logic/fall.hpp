#pragma once

#include <automaton/base/grid.hpp>
#include <random>

namespace automaton {
namespace logic {

class fall_2d : public base_logic {
public:
    fall_2d();

    void step(base_grid& grid) override;
    void clear() override;

private:
    std::default_random_engine _engine;
    std::uniform_int_distribution<int16_t> _dist;
};

class fall_3d : public base_logic {
public:
    explicit fall_3d(uint16_t levels = 0);
    void step(base_grid& grid) override;
    void clear() override;

private:
    uint16_t _levels;

    std::default_random_engine _engine;
    std::uniform_int_distribution<int16_t> _dist;
};

} // namespace logic
} // namespace automaton
