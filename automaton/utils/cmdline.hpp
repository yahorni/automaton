#pragma once

#include <glibmm/ustring.h>

namespace automaton {

struct cmdline {
    // OptionEntry params
    int cols = 0; // 0 - means fill widget width
    int rows = 0; // 0 - means fill widget height
    Glib::ustring type = "2D";
    bool borders = false;
    int delay = 100;
    Glib::ustring logic = "fall";

    // validators
    bool validate_cols() const;
    bool validate_rows() const;
    bool validate_type() const;
    bool validate_delay() const;
    bool validate_logic() const;
};

}  // namespace automaton
