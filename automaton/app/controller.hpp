#pragma once

#include "automaton/app/animation.hpp"
#include "automaton/core/dims.hpp"
#include "automaton/core/grid.hpp"
#include "automaton/engines/engine.hpp"

namespace automaton::app {

class animation;

// controller encapsulates access to grid for modifications from window/canvas/animation
class controller {
public:
    controller(core::grid& grid, animation_ptr animation, engine_ptr engine);

    void engine_activate(size_t row, size_t col);
    void engine_deactivate(size_t row, size_t col);
    bool engine_step();
    void engine_restart();
    void engine_clear();

    void animation_toggle();

    void grid_resize(const core::dims& size);

    std::string get_status() const;

    bool on_animation_timeout();

private:
    core::grid& _grid;
    animation_ptr _animation;
    engine_ptr _engine;
};

}  // namespace automaton::app
