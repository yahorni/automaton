#include "automaton/app/controller.hpp"

#include "automaton/app/animation.hpp"
#include "automaton/app/canvas.hpp"

#include <format>

namespace automaton::app {

controller::controller(core::grid& grid, canvas& canvas, animation& animation, engine_ptr engine, core::dims init_dims)
    : _grid(grid),
      _canvas(canvas),
      _animation(animation),
      _engine(std::move(engine)),
      _init_dims(init_dims) {
    _resize_grid();
    _animation.connect(*this);
}

void controller::add_cell(size_t row, size_t col) { _grid.add(row, col); }
void controller::remove_cell(size_t row, size_t col) { _grid.remove(row, col); }

bool controller::step() {
    _animation.stop();
    return _engine->step();
}

void controller::clear_grid() {
    _animation.stop();
    _grid.clear();
    _engine->reset();
}

void controller::restart_steps() {
    _animation.stop();
    _engine->reset();
}

void controller::toggle_animation() { _animation.is_running() ? _animation.stop() : _animation.start(); }

void controller::handle_resize() {
    g_debug("controller::handle_resize()");
    _resize_grid();
}

std::string controller::get_status() const {
    const core::dims& dims = _grid.dims();
    return std::format("{}, size={}x{}, animation[enabled={},delay={}ms]",  //
                       _engine->name(),                                     //
                       dims.cols, dims.rows,                                //
                       _animation.is_running(), _animation.delay());
}

bool controller::on_animation_timeout() {
    g_debug("controller::on_animation_timeout()");
    if (!_engine->step()) _animation.stop();
    return true;
}

void controller::_resize_grid() {
    const core::dims& dims = _canvas.calculate_dims();
    _grid.resize(_init_dims.rows ? _init_dims.rows : dims.rows,  //
                 _init_dims.cols ? _init_dims.cols : dims.cols);
}

}  // namespace automaton::app
