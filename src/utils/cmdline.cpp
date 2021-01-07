#include "cmdline.hpp"

namespace automaton {

cmdline::cmdline()
    : width(default_width), height(default_height), type(default_type), borders(default_borders) {}

bool cmdline::validate_width() const { return width > 0; }

bool cmdline::validate_height() const { return height > 0; }

bool cmdline::validate_type() const {
    return type == "2D" || type == "3D";
}

}  // namespace automaton
