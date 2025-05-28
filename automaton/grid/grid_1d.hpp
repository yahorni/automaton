#pragma once

#include "automaton/grid/base_cell.hpp"
#include "automaton/grid/base_grid.hpp"

#include <memory>

namespace automaton {

struct cell_1d : public base_cell {
    using base_cell::base_cell;

    bool operator<(const base_cell& other) const;
};

class grid_1d : public grid<cell_1d> {
public:
    using grid<cell_1d>::grid;
};

using grid_1d_ptr = std::shared_ptr<grid_1d>;

}  // namespace automaton
