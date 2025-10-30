#pragma once

#include "automaton/core/grid.hpp"
#include "automaton/core/rules/life.hpp"
#include "automaton/engines/engine.hpp"

namespace automaton::engines {

class life : public engine {
public:
    life(parameters& params, rules::life&& rule);
    std::string description() const override;

    bool step() override;

private:
    void _step_torus(const core::dims& size, const core::grid_state&);
    void _step_plain(const core::dims& size, const core::grid_state&);

    rules::life _rule;

    core::grid_state _new_state;
};

}  // namespace automaton::engines
