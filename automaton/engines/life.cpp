#include "automaton/engines/life.hpp"

#include <format>

namespace automaton::engines {

life::life(core::grid& grid)
    : engine(core::engine_type::LIFE, core::surface_type::TORUS),
      _grid(grid) {}

bool life::step() {
    const core::dims& dims = _grid.dims();
    const auto& state = _grid.state();
    _new_state = {};

    for (size_t row = 0; row < dims.rows; row++) {
        for (size_t col = 0; col < dims.cols; col++) {
            size_t prev_row = (row == 0 ? dims.rows - 1 : row - 1);
            size_t prev_col = (col == 0 ? dims.cols - 1 : col - 1);

            size_t next_row = (row == dims.rows - 1 ? 0 : row + 1);
            size_t next_col = (col == dims.cols - 1 ? 0 : col + 1);

            std::uint8_t neighbours =        //
                state[prev_row][prev_col] +  //
                state[prev_row][col] +       //
                state[prev_row][next_col] +  //
                state[row][prev_col] +       //
                state[row][next_col] +       //
                state[next_row][prev_col] +  //
                state[next_row][col] +       //
                state[next_row][next_col];

            if (!state[row][col] && neighbours == 3) _new_state[row][col] = true;
            if (state[row][col] && (neighbours == 2 || neighbours == 3)) _new_state[row][col] = true;
        }
    }

    _grid.reset(_new_state);

    return true;
}

const std::string& life::name() const {
    static std::string name = std::format("life[surface={}]", options::surface::to_string(_surface_type));
    return name;
}

}  // namespace automaton::engines
