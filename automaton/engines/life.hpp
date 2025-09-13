#pragma once

#include "automaton/core/grid.hpp"
#include "automaton/engines/engine.hpp"

namespace automaton::engines {

class life : public engine {
public:
    explicit life(core::grid& grid);
    const std::string& name() const override;

    bool step() override;

private:
    core::grid& _grid;

    core::grid_state _new_state;
};

}  // namespace automaton::engines
