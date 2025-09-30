#pragma once

#include "automaton/core/grid.hpp"
#include "automaton/engines/engine.hpp"

namespace automaton::engines {

class life : public engine {
public:
    life(parameters& params, uint16_t birth_mask, uint16_t survival_mask);
    std::string description() const override;

    bool step() override;

private:
    void _step_torus(const core::dims& size, const core::grid_state&);
    void _step_plain(const core::dims& size, const core::grid_state&);

    uint16_t _birth_mask;
    uint16_t _survival_mask;

    core::grid_state _new_state;
};

}  // namespace automaton::engines
