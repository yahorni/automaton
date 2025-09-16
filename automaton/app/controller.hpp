#pragma once

#include "automaton/app/animation.hpp"
#include "automaton/core/dims.hpp"
#include "automaton/engines/engine.hpp"

namespace automaton::app {

class animation;

// controller encapsulates access to grid for modifications from window/canvas/animation
class controller {
public:
    controller(animation_ptr animation, engine_ptr engine);

    bool engine_step();
    void engine_restart();
    void engine_clear();

    void engine_action1(size_t row, size_t col);
    void engine_action2(size_t row, size_t col);
    void engine_shift_actions();

    void engine_resize(const core::dims& size);

    void animation_toggle();
    bool on_animation_timeout();

    std::string get_status() const;

private:
    animation_ptr _animation;
    engine_ptr _engine;
};

}  // namespace automaton::app
