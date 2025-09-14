#pragma once

#include "automaton/core/grid.hpp"
#include "automaton/engines/engine.hpp"

namespace automaton::engines {

class life : public engine {
public:
    explicit life(core::grid& grid, core::surface_type surface, std::uint16_t birth_mask, std::uint16_t survival_mask);
    std::string description() const override;

protected:
    bool do_step() override;

private:
    void _step_torus(const core::dims& dims, const core::grid_state&);
    void _step_plain(const core::dims& dims, const core::grid_state&);

    std::uint16_t _birth_mask;
    std::uint16_t _survival_mask;

    core::grid_state _new_state;
};

}  // namespace automaton::engines
