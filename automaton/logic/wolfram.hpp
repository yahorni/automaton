#pragma once

#include "automaton/grid/grid_1d.hpp"
#include "automaton/logic/base_logic.hpp"

namespace automaton::logic {

class wolfram : public base_logic {
public:
    wolfram(base_grid_ptr grid, int8_t code);
    bool step() override;
    void reset() override;

private:
    grid_1d_ptr _grid;

    int8_t _code;
    uint32_t _current_step = 0;
};

}  // namespace automaton::logic
