#pragma once

#include "automaton/core/grid.hpp"
#include "automaton/engines/engine.hpp"

#include <random>

namespace automaton::engines {

class sand : public engine {
public:
    sand(core::grid& grid, core::surface_type surface);
    const std::string& name() const override;

    bool step() override;

private:
    core::grid& _grid;

    std::mt19937 _re;
    std::bernoulli_distribution _dist;
};

}  // namespace automaton::engines
