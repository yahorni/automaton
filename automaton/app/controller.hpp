#pragma once

#include "automaton/app/animation.hpp"
#include "automaton/core/dims.hpp"
#include "automaton/core/grid.hpp"
#include "automaton/engines/engine.hpp"

namespace automaton::app {

class canvas;
class animation;

// controller encapsulates access to grid for modifications from window/canvas/animation
class controller {
public:
    controller(core::grid& grid, canvas& canvas, animation_ptr animation, engine_ptr engine, core::dims init_dims);

    void grid_add(size_t row, size_t col);
    void grid_remove(size_t row, size_t col);
    void grid_clear();

    bool engine_step();
    void engine_restart();

    void animation_toggle();

    void window_resize();

    std::string get_status() const;

    bool on_animation_timeout();

private:
    void _resize_grid();

    core::grid& _grid;
    canvas& _canvas;
    animation_ptr _animation;
    engine_ptr _engine;

    core::dims _init_dims;
};

}  // namespace automaton::app
