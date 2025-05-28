#include "automaton/grid/grid_1d.hpp"

namespace automaton {

bool cell_1d::operator<(const base_cell& other) const {
    if (row == other.row) return col < other.col;
    return row < other.row;
}

}  // namespace automaton
