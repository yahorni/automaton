#pragma once

#include <glibmm/ustring.h>

namespace automaton {

struct cmdline {
    // OptionEntry params
    int width = 0; // 0 - means fill widget width
    int height = 0; // 0 - means fill widget height
    Glib::ustring type = "2D";
    bool borders = false;
    int delay = 100;

    // validators
    bool validate_width() const;
    bool validate_height() const;
    bool validate_type() const;
    bool validate_delay() const;
};

}  // namespace automaton
