#pragma once

#include "automaton/logic/base_logic.hpp"
#include "automaton/grid/grid_2d.hpp"

namespace automaton::logic {

class life_2d : public base_logic {
public:
    explicit life_2d(base_grid_ptr grid);

    bool step() override;
    void reset() override;

private:
    grid_2d_ptr _grid;
};

}  // namespace automaton::logic
