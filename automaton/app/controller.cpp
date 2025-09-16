#include "automaton/app/controller.hpp"

#include <format>

namespace automaton::app {

controller::controller(animation_ptr animation, engine_ptr engine)
    : _animation(std::move(animation)),
      _engine(std::move(engine)) {
    _animation->connect(*this);
}

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

void controller::engine_action1(size_t row, size_t col) { _engine->action1(row, col); }
void controller::engine_action2(size_t row, size_t col) { _engine->action2(row, col); }

void controller::engine_shift_actions() { _engine->shift_actions(); }

void controller::engine_resize(const core::dims& size) { _engine->resize(size); }

void controller::animation_toggle() { _animation->is_running() ? _animation->stop() : _animation->start(); }

bool controller::on_animation_timeout() {
    g_debug("controller::on_animation_timeout()");
    if (!_engine->step()) _animation->stop();
    return true;
}

std::string controller::get_status() const {
    return std::format("{}, {}", _engine->description(), _animation->description());
}

}  // namespace automaton::app
