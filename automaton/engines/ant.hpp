#pragma once

#include "automaton/engines/engine.hpp"

#include <vector>

namespace automaton::engines {

class ant : public engine {
public:
    ant(parameters& params);
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

        void rotate_and_move_torus(const core::dims& size, bool is_empty);
        void rotate_and_move_plain(const core::dims& size, bool is_empty);
    };

    std::vector<_ant> _ants;
    bool _ant_actions = true;
};

}  // namespace automaton::engines
