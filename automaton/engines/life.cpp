#include "automaton/engines/life.hpp"

#include "automaton/core/rule_option.hpp"

#include <format>

namespace automaton::engines {

life::life(core::grid& grid, core::surface_type surface, std::uint16_t birth_mask, std::uint16_t survival_mask)
    : engine(core::engine_type::LIFE, surface),
      _grid(grid),
      _birth_mask(birth_mask),
      _survival_mask(survival_mask) {}

bool life::step() {
    const core::dims& dims = _grid.dims();
    const core::grid_state& state = _grid.state();
    _new_state = {};

    if (_surface_type == core::surface_type::TORUS) {
        _step_torus(dims, state);
    } else {  // core::surface_type::PLAIN
        _step_plain(dims, state);
    }

    _grid.reset(_new_state);

    return true;
}

void life::_step_torus(const core::dims& dims, const core::grid_state& state) {
    for (size_t row = 0; row < dims.rows; ++row) {
        for (size_t col = 0; col < dims.cols; ++col) {

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

            // funny things happen when "+ 1" added to neighbours
            if (state[row][col]) {
                _new_state[row][col] = static_cast<bool>(_survival_mask & (1 << neighbours));
            } else {
                _new_state[row][col] = static_cast<bool>(_birth_mask & (1 << neighbours));
            }
        }
    }
}

// Ugly calculations, be careful
void life::_step_plain(const core::dims& dims, const core::grid_state& state) {
    // use _new_state to temporarily calculate and store neighbours
    // all calculations are left->right & top->bottom

    size_t last_row = dims.rows - 1;
    size_t last_col = dims.cols - 1;

    // corners
    _new_state[0][0] =  //
        state[0][1] + state[1][0] + state[1][1];
    _new_state[0][last_col] =  //
        state[0][last_col - 1] + state[1][last_col - 1] + state[1][last_col];
    _new_state[last_row][0] =  //
        state[last_row - 1][0] + state[last_row - 1][1] + state[last_row][1];
    _new_state[last_row][last_col] =
        state[last_row - 1][last_col - 1] + state[last_row - 1][last_col] + state[last_row][last_col - 1];

    // top line (row = 0)
    for (size_t col = 1; col < dims.cols - 1; ++col) {
        _new_state[0][col] =                         //
            state[0][col - 1] + state[0][col + 1] +  //
            state[1][col - 1] + state[1][col] + state[1][col + 1];
    }

    // bottom line (row = dims.rows - 1)
    for (size_t col = 1; col < dims.cols - 1; ++col) {
        _new_state[last_row][col] =                                                                   //
            state[last_row - 1][col - 1] + state[last_row - 1][col] + state[last_row - 1][col + 1] +  //
            state[last_row][col - 1] + state[last_row][col + 1];
    }

    // left column (col = 0)
    for (size_t row = 1; row < dims.rows - 1; ++row) {
        _new_state[row][0] =                         //
            state[row - 1][0] + state[row - 1][1] +  //
            state[row][1] +                          //
            state[row + 1][0] + state[row + 1][1];
    }

    // right column (col = dims.cols - 1)
    for (size_t row = 1; row < dims.rows - 1; ++row) {
        _new_state[row][last_col] =                                    //
            state[row - 1][last_col - 1] + state[row - 1][last_col] +  //
            state[row][last_col - 1] +                                 //
            state[row + 1][last_col - 1] + state[row + 1][last_col];
    }

    // center
    for (size_t row = 1; row < dims.rows - 1; ++row) {
        for (size_t col = 1; col < dims.cols - 1; ++col) {
            _new_state[row][col] =         //
                state[row - 1][col - 1] +  //
                state[row - 1][col] +      //
                state[row - 1][col + 1] +  //
                state[row][col - 1] +      //
                state[row][col + 1] +      //
                state[row + 1][col - 1] +  //
                state[row + 1][col] +      //
                state[row + 1][col + 1];
        }
    }

    // calculate state
    for (size_t row = 0; row < dims.rows; ++row) {
        for (size_t col = 0; col < dims.cols; ++col) {
            // _new_state[row][col] is overwritten here for each cell,
            // it's important to not mess neighbours with the new state
            if (state[row][col]) {
                _new_state[row][col] = static_cast<bool>(_survival_mask & (1 << _new_state[row][col]));
            } else {
                _new_state[row][col] = static_cast<bool>(_birth_mask & (1 << _new_state[row][col]));
            }
        }
    }
}

const std::string& life::name() const {
    static std::string name = std::format("life[surface={},rule={}]", options::surface::to_string(_surface_type),
                                          options::rule::to_string(_birth_mask, _survival_mask));
    return name;
}

}  // namespace automaton::engines
