#pragma once

#include "automaton/core/dims.hpp"
#include "automaton/core/grid.hpp"
#include "automaton/engines/engine.hpp"

namespace automaton::app {

class canvas;
class animation;

class controller {
public:
    controller(core::grid& grid, canvas& canvas, animation& animation, engine_ptr engine, core::dims init_dims);

    void add_cell(size_t row, size_t col);
    void remove_cell(size_t row, size_t col);
    bool step();
    void clear_grid();
    void restart_steps();
    void toggle_animation();
    void handle_resize();
    std::string get_status() const;
    bool on_timeout();

private:
    void _resize_grid();

    core::grid& _grid;
    canvas& _canvas;
    animation& _animation;
    engine_ptr _engine;

    core::dims _init_dims;
};

}  // namespace automaton::app
