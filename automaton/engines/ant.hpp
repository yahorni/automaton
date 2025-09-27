#pragma once

#include "automaton/core/grid.hpp"
#include "automaton/engines/engine.hpp"

#include <vector>

namespace automaton::engines {

class ant : public engine {
public:
    ant(core::grid& grid, core::surface_type surface);
    std::string description() const override;

    bool step() override;
    void restart() override;
    void clear() override;

    void action1(size_t row, size_t col) override;
    void action2(size_t row, size_t col) override;
    void shift_actions() override;

    void resize(const core::dims& size) override;

private:
    enum class directions { UP, DOWN, LEFT, RIGHT };

    struct _ant {
        size_t row;
        size_t col;
        directions dir;

        void rotate_and_move_torus(bool is_empty, const core::dims& size);
        void rotate_and_move_plain(bool is_empty, const core::dims& size);
    };

    std::vector<_ant> _ants;
    bool _ant_actions = true;
};

}  // namespace automaton::engines
