#include <automaton/logic/fall.hpp>

namespace automaton {
namespace logic {

fall_2d::fall_2d() {
    std::random_device rd;
    _engine = std::default_random_engine(rd());
    _dist = std::uniform_int_distribution<int16_t>(1, 2);
}


void fall_2d::step(grid_2d& grid) {
    std::set<base_cell> data_copy = {grid.get_data().begin(),
                                     grid.get_data().end()};

    for (auto it = data_copy.begin(); it != data_copy.end(); it++) {
        if (it->row == grid.get_rows() - 1) continue;

        if (!grid.has(it->row + 1, it->col)) {
            grid.move(*it, {it->row + 1, it->col});
            continue;
        }

        std::vector<base_cell> positions;

        if (it->col > 0 && !grid.has(it->row + 1, it->col - 1)) {
            positions.emplace_back(it->row + 1, it->col - 1);
        }

        if (it->col < grid.get_cols() - 1 &&
                   !grid.has(it->row + 1, it->col + 1)) {
            positions.emplace_back(it->row + 1, it->col + 1);
        }

        if (positions.size() == 0) continue;

        int8_t direction = _dist(_engine) % positions.size();
        auto position = positions.at(direction);
        grid.move(*it, position);
    }
}

fall_3d::fall_3d(uint16_t levels) : _levels(levels) {
    std::random_device rd;
    _engine = std::default_random_engine(rd());
    _dist = std::uniform_int_distribution<int16_t>(1, 12);
}

void fall_3d::step(grid_3d& grid) {
    std::set<cell_3d> data_copy = {grid.get_data().begin(),
                                   grid.get_data().end()};

    for (auto it = data_copy.begin(); it != data_copy.end(); it++) {
        if (it->row == grid.get_rows() - 1) continue;

        if (!grid.has(it->row + 1, it->col, it->level)) {
            grid.move(*it, {it->row + 1, it->col, it->level});
            continue;
        }

        std::vector<cell_3d> positions;

        if (it->col > 0 && !grid.has(it->row + 1, it->col - 1, it->level)) {
            positions.emplace_back(it->row + 1, it->col - 1, it->level);
        }

        if (it->col < grid.get_cols() - 1 &&
            !grid.has(it->row + 1, it->col + 1, it->level)) {
            positions.emplace_back(it->row + 1, it->col + 1, it->level);
        }

        if (_levels == 0) {
            if (!grid.has(it->row + 1, it->col, it->level - 1)) {
                positions.emplace_back(it->row + 1, it->col, it->level - 1);
            }

            if (!grid.has(it->row + 1, it->col, it->level + 1)) {
                positions.emplace_back(it->row + 1, it->col, it->level + 1);
            }
        } else {
            if (it->level < _levels &&
                !grid.has(it->row + 1, it->col, it->level + 1)) {
                positions.emplace_back(it->row + 1, it->col, it->level + 1);
            }

            if (it->level > -_levels &&
                !grid.has(it->row + 1, it->col, it->level)) {
                positions.emplace_back(it->row + 1, it->col, it->level - 1);
            }
        }

        if (positions.size() == 0) continue;

        int8_t direction = _dist(_engine) % positions.size();
        auto position = positions.at(direction);
        grid.move(*it, position);
    }
}

}  // namespace logic
}  // namespace automaton
