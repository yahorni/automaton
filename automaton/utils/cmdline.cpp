#include <automaton/utils/cmdline.hpp>

namespace automaton {

bool cmdline::validate_cols() const { return cols >= 0; }

bool cmdline::validate_rows() const { return rows >= 0; }

bool cmdline::validate_type() const { return type == "2D" || type == "3D"; }

bool cmdline::validate_delay() const { return delay > 0; }

bool cmdline::validate_logic() const {
    if (type == "2D") return logic == "fall" || logic == "life";
    if (type == "3D") return logic == "fall";
    return false;
}

}  // namespace automaton
