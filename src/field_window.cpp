#include "field_window.hpp"
#include <iostream>

namespace automaton {

FieldWindow::FieldWindow() {
    set_title("Automaton");
    set_default_size(800, 600);
    add(_frame);

    _frame.property_margin() = 15;
    _frame.add(_area);
    _frame.property_visible() = true;

    _area.property_visible() = true;

    signal_key_press_event().connect(sigc::mem_fun(*this, &FieldWindow::on_key_press));
}

FieldWindow::~FieldWindow() {}

bool FieldWindow::on_key_press(GdkEventKey* ev) {
    if (ev->keyval == GDK_KEY_q) {
        close();
        return true;
    }
    return false;
}

}  // namespace automaton
