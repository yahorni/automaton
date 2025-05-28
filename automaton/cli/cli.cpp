#include "automaton/cli/cli.hpp"

namespace automaton {

std::tuple<bool, std::string> cli::validate() const {
    // main
    if (!validate_cols()) return {false, "cols"};
    if (!validate_rows()) return {false, "rows"};
    if (!validate_delay()) return {false, "delay"};
    if (!validate_cell_width()) return {false, "cell-width"};

    if (!validate_logic()) return {false, "logic"};
    if (!validate_wolfram()) return {false, "code"};
    if (!validate_fall()) return {false, "splices"};

    return {true, ""};
}

// main
bool cli::validate_cols() const { return cols >= 0; }
bool cli::validate_rows() const { return rows >= 0; }
bool cli::validate_delay() const { return delay > 0; }
bool cli::validate_cell_width() const { return cell_width > 0; }

bool cli::validate_logic() const { return logic == "wolfram" || logic == "fall" || logic == "life"; }
bool cli::validate_wolfram() const { return wolfram.code >= 0 && wolfram.code <= 255; }
bool cli::validate_fall() const { return fall.splices >= 0; }

}  // namespace automaton
