#include "automaton/app/animation.hpp"

#include "automaton/app/controller.hpp"

#include <format>

namespace automaton::app {

animation::animation(int delay, bool enabled)
    : _delay(delay),
      _enabled(enabled),
      _is_running(false) {}

void animation::connect(controller& ctrl) {
    _motion_slot = sigc::mem_fun(ctrl, &controller::on_animation_timeout);
    if (_enabled) _animation_connection = Glib::signal_timeout().connect(_motion_slot, _delay);
    _is_running = _enabled;
}

void animation::start() {
    g_debug("animation::start(is_running=%d)", _is_running);
    if (_is_running) return;

    _animation_connection = Glib::signal_timeout().connect(_motion_slot, _delay);
    _is_running = true;
}

void animation::stop() {
    g_debug("animation::stop(is_running=%d)", _is_running);
    if (!_is_running) return;

    _animation_connection.disconnect();
    _is_running = false;
}

bool animation::is_running() const { return _is_running; }

int animation::delay() const { return _delay; }

void animation::set_delay(int delay) { _delay = delay; }

std::string animation::description() const {
    return std::format("animation[enabled={},delay={}ms]", _is_running, _delay);
}

}  // namespace automaton::app
