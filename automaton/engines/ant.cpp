#include "automaton/engines/ant.hpp"

#include <algorithm>
#include <format>
#include <utility>

namespace automaton::engines {

// the last bit is to set if there's an ant in a cell, other bits - for colors
//  ant_mask = 0b1000...
// ~ant_mask = 0b0111...
static constexpr core::cell_state ant_mask = (1 << (sizeof(core::cell_state) * CHAR_BIT - 1));

ant::ant(parameters& params, rules::ant&& rule)
    : engine(core::engine_type::ANT, params),
      _rule(std::move(rule)) {}

std::string ant::description() const {
    return std::format("ant[surface={},action={},size={},step={}]",  //
                       options::surface::to_string(_surface_type),   //
                       _ant_actions ? "ant" : "wall", _grid.size(), _step);
}

bool ant::step() {
    if (_ants.empty()) return true;

    const core::dims& size = _grid.size();

    // update ants direction
    for (auto& a : _ants) {
        const auto s = _grid[a.row, a.col] & (~ant_mask);
        switch (_rule[s]) {
        case rules::ant::action::LEFT:
            a.rotate_left();
            break;
        case rules::ant::action::RIGHT:
            a.rotate_right();
            break;
        case rules::ant::action::FORWARD:
            break;
        default:
            std::unreachable();
        }
    }

    // update ants position
    if (_surface_type == core::surface_type::TORUS) {
        for (auto& a : _ants) {
            a.move_on_torus(size);
        }
    } else {
        for (auto& a : _ants) {
            a.move_on_plain(size);
        }
    }

    // update cells color and remove ants
    for (size_t row = 0; row < size.rows; ++row) {
        for (size_t col = 0; col < size.cols; ++col) {
            if (_grid[row, col] & ant_mask) _grid.set(row, col, ((_grid[row, col] & (~ant_mask)) + 1) % (_rule.size()));
        }
    }

    // place ants to cells
    for (const auto& a : _ants) {
        _grid.set(a.row, a.col, _grid[a.row, a.col] | ant_mask);
    }

    _step++;
    return true;
}

void ant::restart() {
    engine::restart();

    for (size_t row = 0; row < _grid.rows(); ++row) {
        for (size_t col = 0; col < _grid.cols(); ++col) {
            _grid.set(row, col, _grid[row, col] & ant_mask);
        }
    }

    _ants.clear();
}

void ant::clear() {
    engine::clear();
    _ants.clear();
}

void ant::action1(size_t row, size_t col) {
    const auto s = _grid.get(row, col);
    if (_ant_actions) {
        _ants.emplace_back(row, col, directions::UP);
        _grid.set(row, col, s | ant_mask);
    } else {
        if (!(s & (~ant_mask))) _grid.set(row, col, s | 1);
    }
}

void ant::action2(size_t row, size_t col) {
    const auto s = _grid.get(row, col);
    if (_ant_actions) {
        _ants.erase(std::remove_if(                  //
                        _ants.begin(), _ants.end(),  //
                        [row, col](const _ant& a) { return a.row == row && a.col == col; }),
                    _ants.end());
        _grid.set(row, col, s & (~ant_mask));
    } else {
        if (s & (~ant_mask)) _grid.set(row, col, s & ant_mask);
    }
}

void ant::shift_actions() { _ant_actions = !_ant_actions; }

void ant::resize(const core::dims& size) {
    engine::resize(size);
    const core::dims& new_size = _grid.size();
    std::erase_if(_ants, [&new_size](const _ant& a) { return a.row >= new_size.rows || a.col >= new_size.cols; });
}

void ant::_ant::rotate_left() {
    switch (dir) {
    case directions::DOWN:
        dir = directions::RIGHT;
        break;
    case directions::LEFT:
        dir = directions::DOWN;
        break;
    case directions::UP:
        dir = directions::LEFT;
        break;
    case directions::RIGHT:
        dir = directions::UP;
        break;
    default:
        std::unreachable();
    }
}

void ant::_ant::rotate_right() {
    switch (dir) {
    case directions::UP:
        dir = directions::RIGHT;
        break;
    case directions::RIGHT:
        dir = directions::DOWN;
        break;
    case directions::DOWN:
        dir = directions::LEFT;
        break;
    case directions::LEFT:
        dir = directions::UP;
        break;
    default:
        std::unreachable();
    }
}

void ant::_ant::move_on_torus(const core::dims& size) {
    switch (dir) {
    case directions::RIGHT:
        col = (col == size.cols - 1 ? 0 : col + 1);
        break;
    case directions::DOWN:
        row = (row == size.rows - 1 ? 0 : row + 1);
        break;
    case directions::LEFT:
        col = (col == 0 ? size.cols - 1 : col - 1);
        break;
    case directions::UP:
        row = (row == 0 ? size.rows - 1 : row - 1);
        break;
    default:
        std::unreachable();
    }
}

void ant::_ant::move_on_plain(const core::dims& size) {
    switch (dir) {
    case directions::RIGHT:
        if (col < size.cols) col++;
        break;
    case directions::DOWN:
        if (row < size.rows) row++;
        break;
    case directions::LEFT:
        if (col > 0) col--;
        break;
    case directions::UP:
        if (row > 0) row--;
        break;
    default:
        std::unreachable();
    }
}

}  // namespace automaton::engines
