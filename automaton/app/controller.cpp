#include "automaton/app/controller.hpp"

#include "automaton/app/canvas.hpp"

#include <format>

namespace automaton::app {

controller::controller(core::grid& grid, canvas& canvas, animation_ptr animation, engine_ptr engine,
                       core::dims init_dims)
    : _grid(grid),
      _canvas(canvas),
      _animation(std::move(animation)),
      _engine(std::move(engine)),
      _init_dims(init_dims) {
    _resize_grid();
    _animation->connect(*this);
}

void controller::grid_add(size_t row, size_t col) { _grid.add(row, col); }
void controller::grid_remove(size_t row, size_t col) { _grid.remove(row, col); }

void controller::grid_clear() {
    _animation->stop();
    _grid.clear();
    _engine->reset();
}

bool controller::engine_step() {
    _animation->stop();
    return _engine->step();
}

void controller::engine_restart() {
    _animation->stop();
    _engine->reset();
}

void controller::animation_toggle() { _animation->is_running() ? _animation->stop() : _animation->start(); }

void controller::window_resize() {
    g_debug("controller::window_resize()");
    _resize_grid();
}

std::string controller::get_status() const {
    static const std::string engine_desc = _engine->description();
    static const std::string animation_desc = _animation->description();

    const core::dims& dims = _grid.dims();
    return std::format("{}, {}, size[{}x{}]", engine_desc, animation_desc, dims.cols, dims.rows);
}

bool controller::on_animation_timeout() {
    g_debug("controller::on_animation_timeout()");
    if (!_engine->step()) _animation->stop();
    return true;
}

void controller::_resize_grid() {
    const core::dims& dims = _canvas.calculate_dims();
    _grid.resize(_init_dims.rows ? _init_dims.rows : dims.rows,  //
                 _init_dims.cols ? _init_dims.cols : dims.cols);
}

}  // namespace automaton::app
