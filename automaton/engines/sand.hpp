#pragma once

#include "automaton/core/grid.hpp"
#include "automaton/engines/engine.hpp"

#include <random>

namespace automaton::engines {

class sand : public engine {
public:
    sand(core::grid& grid, core::surface_type surface);
    std::string description() const override;

    bool step() override;

    void action1(size_t row, size_t col) override;
    void shift_actions() override;

private:
    enum class blocks : unsigned {
        SAND = 1,
        WALL,

        SIZE,
    } _block_actions = blocks::SAND;

    std::mt19937 _re;
    std::bernoulli_distribution _dist;
};

}  // namespace automaton::engines
