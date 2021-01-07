#include <automaton/utils/cmdline.hpp>

namespace automaton {

bool cmdline::validate_width() const { return width >= 0; }

bool cmdline::validate_height() const { return height >= 0; }

bool cmdline::validate_type() const { return type == "2D" || type == "3D"; }

bool cmdline::validate_delay() const { return delay > 0; }

}  // namespace automaton
