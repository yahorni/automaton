#include "grid_window.hpp"

namespace automaton {

grid_window::grid_window() {
    set_title("Automaton");
    set_default_size(800, 600);
    add(_frame);

    _frame.property_margin() = 15;
    _frame.add(_area);
    _frame.property_visible() = true;

    _area.property_visible() = true;

    signal_key_press_event().connect(sigc::mem_fun(*this, &grid_window::on_key_press));
}

grid_window::~grid_window() {}

bool grid_window::on_key_press(GdkEventKey* ev) {
    if (ev->keyval == GDK_KEY_q) {
        close();
        return true;
    }
    return false;
}

}  // namespace automaton
