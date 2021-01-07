#pragma once

#include <glibmm/ustring.h>

namespace automaton {

struct cmdline {
    // OptionEntry params
    int width;
    int height;
    Glib::ustring type;
    bool borders;

    cmdline();

    // validators
    bool validate_width() const;
    bool validate_height() const;
    bool validate_type() const;

    // default values
    static const uint32_t default_width = 40;
    static const uint32_t default_height = 50;
    static constexpr char default_type[] = "2D";
    static const bool default_borders = false;
};

}  // namespace automaton
