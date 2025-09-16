#include "automaton/engines/ant.hpp"

#include <algorithm>
#include <format>

namespace automaton::engines {

ant::ant(core::grid& grid, core::surface_type surface)
    : engine(grid, core::engine_type::ANT, surface) {}

std::string ant::description() const {
    return std::format("ant[surface={},action={},step={}]",         //
                       options::surface::to_string(_surface_type),  //
                       _ant_actions ? "ant" : "wall",               //
                       current_step());
}

void ant::action1(size_t row, size_t col) {
    if (_ant_actions) {
        _ants.emplace_back(row, col, directions::UP);
        _grid.set(row, col, _grid.get(row, col) | 0b10);
    } else {
        if (auto s = _grid.get(row, col); !(_grid.get(row, col) & 0b01)) _grid.set(row, col, s + 1);
    }
}

void ant::action2(size_t row, size_t col) {
    if (_ant_actions) {
        _ants.erase(std::remove_if(_ants.begin(), _ants.end(),  //
                                   [row, col](const _ant& a) { return a.row == row && a.col == col; }),
                    _ants.end());
        _grid.set(row, col, _grid.get(row, col) & 0b01);
    } else {
        if (auto s = _grid.get(row, col); _grid.get(row, col) & 0b01) _grid.set(row, col, s - 1);
    }
}

void ant::shift_actions() { _ant_actions = !_ant_actions; }

bool ant::do_step() {
    if (_ants.empty()) return false;

    const core::dims& dims = _grid.dims();
    const core::grid_state& state = _grid.state();

    // update ants
    if (_surface_type == core::surface_type::TORUS) {
        for (auto& a : _ants) {
            a.rotate_and_move_torus(state[a.row][a.col] & 0b01, dims);
        }
    } else {
        for (auto& a : _ants) {
            a.rotate_and_move_plain(state[a.row][a.col] & 0b01, dims);
        }
    }

    // update state cells
    for (size_t row = 0; row < dims.rows; ++row) {
        for (size_t col = 0; col < dims.cols; ++col) {
            // if there's an ant in a cell, then invert it's color
            if (state[row][col] & 0b10)  //
                _grid.set(row, col, !static_cast<bool>(state[row][col] & 0b01));
        }
    }

    // update state with ants
    for (const auto& a : _ants) {
        _grid.set(a.row, a.col, state[a.row][a.col] | 0b10);
    }

    return true;
}

void ant::do_clear() {
    const core::dims& dims = _grid.dims();
    const core::grid_state& state = _grid.state();

    for (size_t row = 0; row < dims.rows; ++row) {
        for (size_t col = 0; col < dims.cols; ++col) {
            _grid.set(row, col, state[row][col] & 0b01);
        }
    }

    _ants.clear();
}

void ant::_ant::rotate_and_move_torus(bool is_empty, const core::dims& dims) {
    if ((dir == directions::UP && is_empty) || (dir == directions::DOWN && !is_empty)) {
        dir = directions::RIGHT;
        col = (col == dims.cols - 1 ? 0 : col + 1);
    } else if ((dir == directions::RIGHT && is_empty) || (dir == directions::LEFT && !is_empty)) {
        dir = directions::DOWN;
        row = (row == dims.rows - 1 ? 0 : row + 1);
    } else if ((dir == directions::DOWN && is_empty) || (dir == directions::UP && !is_empty)) {
        dir = directions::LEFT;
        col = (col == 0 ? dims.cols - 1 : col - 1);
    } else if ((dir == directions::LEFT && is_empty) || (dir == directions::RIGHT && !is_empty)) {
        dir = directions::UP;
        row = (row == 0 ? dims.rows - 1 : row - 1);
    }
}

void ant::_ant::rotate_and_move_plain(bool is_empty, const core::dims& dims) {
    if ((dir == directions::UP && is_empty) || (dir == directions::DOWN && !is_empty)) {
        dir = directions::RIGHT;
        if (col < dims.cols) col++;
    } else if ((dir == directions::RIGHT && is_empty) || (dir == directions::LEFT && !is_empty)) {
        dir = directions::DOWN;
        if (row < dims.rows) row++;
    } else if ((dir == directions::DOWN && is_empty) || (dir == directions::UP && !is_empty)) {
        dir = directions::LEFT;
        if (col > 0) col--;
    } else if ((dir == directions::LEFT && is_empty) || (dir == directions::RIGHT && !is_empty)) {
        dir = directions::UP;
        if (row > 0) row--;
    }
}

}  // namespace automaton::engines
