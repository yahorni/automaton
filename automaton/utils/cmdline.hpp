#pragma once

#include <glibmm/ustring.h>

#include <tuple>

namespace automaton {

struct cmdline {
    // OptionEntry params
    int cols = 0;  // 0 - means fill widget width
    int rows = 0;  // 0 - means fill widget height
    Glib::ustring type = "2D";
    bool borders = false;
    int delay = 100;
    Glib::ustring logic = "fall";
    int levels_3d = 0;  // 0 - means unlimited levels

    std::tuple<bool, std::string> validate() const;

   private:
    // validators
    bool validate_cols() const;
    bool validate_rows() const;
    bool validate_type() const;
    bool validate_delay() const;
    bool validate_logic() const;
    bool validate_levels_3d() const;
};

}  // namespace automaton
