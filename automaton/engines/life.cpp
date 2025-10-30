#include "automaton/engines/life.hpp"

#include <format>

namespace automaton::engines {

life::life(parameters& params, rules::life&& rule)
    : engine(core::engine_type::LIFE, params),
      _rule(std::move(rule)) {}

std::string life::description() const {
    return std::format("life[surface={},rule={},size={},step={}]",  //
                       options::surface::to_string(_surface_type),  //
                       _rule.to_string(),                           //
                       _grid.size(), _step);
}

bool life::step() {
    _new_state = {};

    if (_surface_type == core::surface_type::TORUS) {
        _step_torus();
    } else {
        _step_plain();
    }

    _grid.reset(_new_state);

    _step++;
    return true;
}

void life::_step_torus() {
    const core::dims& size = _grid.size();
    for (size_t row = 0; row < size.rows; ++row) {
        for (size_t col = 0; col < size.cols; ++col) {

            size_t prev_row = (row == 0 ? size.rows - 1 : row - 1);
            size_t prev_col = (col == 0 ? size.cols - 1 : col - 1);

            size_t next_row = (row == size.rows - 1 ? 0 : row + 1);
            size_t next_col = (col == size.cols - 1 ? 0 : col + 1);

            uint8_t neighbours =             //
                _grid[prev_row, prev_col] +  //
                _grid[prev_row, col] +       //
                _grid[prev_row, next_col] +  //
                _grid[row, prev_col] +       //
                _grid[row, next_col] +       //
                _grid[next_row, prev_col] +  //
                _grid[next_row, col] +       //
                _grid[next_row, next_col];

            // funny things happen when "+ 1" added to neighbours
            if (_grid[row, col]) {
                _new_state[row][col] = static_cast<bool>(_rule.survival_mask() & (1 << neighbours));
            } else {
                _new_state[row][col] = static_cast<bool>(_rule.birth_mask() & (1 << neighbours));
            }
        }
    }
}

// Ugly calculations, be careful
void life::_step_plain() {
    // use _new_state to temporarily calculate and store neighbours
    // all calculations are left->right & top->bottom

    const core::dims& size = _grid.size();
    size_t last_row = size.rows - 1;
    size_t last_col = size.cols - 1;

    // corners
    _new_state[0][0] =  //
        _grid[0, 1] + _grid[1, 0] + _grid[1, 1];
    _new_state[0][last_col] =  //
        _grid[0, last_col - 1] + _grid[1, last_col - 1] + _grid[1, last_col];
    _new_state[last_row][0] =  //
        _grid[last_row - 1, 0] + _grid[last_row - 1, 1] + _grid[last_row, 1];
    _new_state[last_row][last_col] =
        _grid[last_row - 1, last_col - 1] + _grid[last_row - 1, last_col] + _grid[last_row, last_col - 1];

    // top line (row = 0)
    for (size_t col = 1; col < size.cols - 1; ++col) {
        _new_state[0][col] =                         //
            _grid[0, col - 1] + _grid[0, col + 1] +  //
            _grid[1, col - 1] + _grid[1, col] + _grid[1, col + 1];
    }

    // bottom line (row = size.rows - 1)
    for (size_t col = 1; col < size.cols - 1; ++col) {
        _new_state[last_row][col] =                                                                   //
            _grid[last_row - 1, col - 1] + _grid[last_row - 1, col] + _grid[last_row - 1, col + 1] +  //
            _grid[last_row, col - 1] + _grid[last_row, col + 1];
    }

    // left column (col = 0)
    for (size_t row = 1; row < size.rows - 1; ++row) {
        _new_state[row][0] =                         //
            _grid[row - 1, 0] + _grid[row - 1, 1] +  //
            _grid[row, 1] +                          //
            _grid[row + 1, 0] + _grid[row + 1, 1];
    }

    // right column (col = size.cols - 1)
    for (size_t row = 1; row < size.rows - 1; ++row) {
        _new_state[row][last_col] =                                    //
            _grid[row - 1, last_col - 1] + _grid[row - 1, last_col] +  //
            _grid[row, last_col - 1] +                                 //
            _grid[row + 1, last_col - 1] + _grid[row + 1, last_col];
    }

    // center
    for (size_t row = 1; row < size.rows - 1; ++row) {
        for (size_t col = 1; col < size.cols - 1; ++col) {
            _new_state[row][col] =         //
                _grid[row - 1, col - 1] +  //
                _grid[row - 1, col] +      //
                _grid[row - 1, col + 1] +  //
                _grid[row, col - 1] +      //
                _grid[row, col + 1] +      //
                _grid[row + 1, col - 1] +  //
                _grid[row + 1, col] +      //
                _grid[row + 1, col + 1];
        }
    }

    // calculate state
    for (size_t row = 0; row < size.rows; ++row) {
        for (size_t col = 0; col < size.cols; ++col) {
            // _new_state[row][col] is overwritten here for each cell,
            // it's important to not mess neighbours with the new state
            if (_grid[row, col]) {
                _new_state[row][col] = static_cast<bool>(_rule.survival_mask() & (1 << _new_state[row][col]));
            } else {
                _new_state[row][col] = static_cast<bool>(_rule.birth_mask() & (1 << _new_state[row][col]));
            }
        }
    }
}

}  // namespace automaton::engines
