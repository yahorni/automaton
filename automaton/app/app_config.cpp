#include "automaton/app/app_config.hpp"

#include <string>
#include <tuple>

namespace automaton {

std::tuple<bool, std::string> app_config::validate() const {
    // ui
    if (!validate_cols()) return {false, "cols"};
    if (!validate_rows()) return {false, "rows"};
    if (!validate_delay()) return {false, "delay"};
    if (!validate_cell_width()) return {false, "cell-width"};
    // automaton
    if (!validate_logic()) return {false, "logic"};
    if (!validate_wolfram()) return {false, "code"};
    if (!validate_fall()) return {false, "splices"};

    return {true, ""};
}

// ui
bool app_config::validate_cols() const { return ui.cols >= 0; }
bool app_config::validate_rows() const { return ui.rows >= 0; }
bool app_config::validate_delay() const { return ui.delay > 0; }
bool app_config::validate_cell_width() const { return ui.cell_width > 0; }

// automaton
bool app_config::validate_logic() const { return am.logic == "wolfram" || am.logic == "fall" || am.logic == "life"; }
bool app_config::validate_wolfram() const { return am.wf_code >= 0 && am.wf_code <= 255; }
bool app_config::validate_fall() const { return am.fall_splices >= 0; }

}  // namespace automaton
