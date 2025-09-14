#include "automaton/app/controller.hpp"

#include <format>

namespace automaton::app {

controller::controller(core::grid& grid, animation_ptr animation, engine_ptr engine)
    : _grid(grid),
      _animation(std::move(animation)),
      _engine(std::move(engine)) {
    _animation->connect(*this);
}

void controller::engine_action1(size_t row, size_t col) { _engine->action1(row, col); }
void controller::engine_action2(size_t row, size_t col) { _engine->action2(row, col); }

bool controller::engine_step() {
    _animation->stop();
    return _engine->step();
}

void controller::engine_restart() {
    _animation->stop();
    _engine->restart();
}

void controller::engine_clear() {
    _animation->stop();
    _engine->clear();
}

void controller::engine_shift_actions() { _engine->shift_actions(); }

void controller::animation_toggle() { _animation->is_running() ? _animation->stop() : _animation->start(); }

void controller::grid_resize(const core::dims& dims) { _grid.resize(dims.rows, dims.cols); }

std::string controller::get_status() const {
    const std::string& engine_desc = _engine->description();
    static const std::string animation_desc = _animation->description();

    const core::dims& dims = _grid.dims();
    return std::format("{}, {}, size[{}x{}]", engine_desc, animation_desc, dims.cols, dims.rows);
}

bool controller::on_animation_timeout() {
    g_debug("controller::on_animation_timeout()");
    if (!_engine->step()) _animation->stop();
    return true;
}

}  // namespace automaton::app
