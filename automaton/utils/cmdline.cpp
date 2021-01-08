#include <automaton/utils/cmdline.hpp>

namespace automaton {

std::tuple<bool, std::string> cmdline::validate() const {
    if (!validate_cols()) return {false, "cols"};
    if (!validate_rows()) return {false, "rows"};
    if (!validate_type()) return {false, "type"};
    if (!validate_delay()) return {false, "delay"};
    if (!validate_logic()) return {false, "logic"};
    if (!validate_levels_3d()) return {false, "levels-3D"};

    return {true, ""};
}

bool cmdline::validate_cols() const { return cols >= 0; }

bool cmdline::validate_rows() const { return rows >= 0; }

bool cmdline::validate_type() const { return type == "2D" || type == "3D"; }

bool cmdline::validate_delay() const { return delay > 0; }

bool cmdline::validate_logic() const {
    if (type == "2D") return logic == "fall" || logic == "life";
    if (type == "3D") return logic == "fall";
    return false;
}

bool cmdline::validate_levels_3d() const { return levels_3d >= 0; }

}  // namespace automaton
