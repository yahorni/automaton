#include "automaton/core/config.hpp"

#include "automaton/core/join_options.hpp"
#include "automaton/core/rule_option.hpp"

#include <string>
#include <tuple>

namespace automaton::core {

std::tuple<bool, std::string> config::validate() const {
    // ui
    if (!_validate_cell_width()) return {false, "cell-width"};
    if (!_validate_delay()) return {false, "delay"};
    // automaton
    if (!_validate_engine()) return {false, "engine"};
    if (!_validate_surface()) return {false, "surface"};
    if (!_validate_cols()) return {false, "cols"};
    if (!_validate_rows()) return {false, "rows"};
    if (!_validate_wolfram_code()) return {false, "wf-code"};
    if (!_validate_life_rule()) return {false, "life-rule"};

    if (!_validate_surface_for_engine()) return {false, "surface"};

    return {true, ""};
}

// ui
bool config::_validate_cell_width() const { return ui.cell_width > 0; }
bool config::_validate_delay() const { return ui.delay > 0; }

// automaton
bool config::_validate_engine() const { return options::engine::is_valid(automaton.engine); }
bool config::_validate_surface() const { return options::surface::is_valid(automaton.surface); }
bool config::_validate_cols() const { return automaton.init_cols >= 0; }
bool config::_validate_rows() const { return automaton.init_rows >= 0; }
bool config::_validate_wolfram_code() const { return automaton.wolfram_code >= 0 && automaton.wolfram_code <= 255; }
bool config::_validate_life_rule() const { return options::rule::is_valid(automaton.life_rule); }

bool config::_validate_surface_for_engine() const {
    engine_type engine = options::engine::from_string(automaton.engine);
    surface_type surface = options::surface::from_string(automaton.surface);

    switch (engine) {
    case engine_type::LIFE:
        return surface == surface_type::DEFAULT || surface == surface_type::PLAIN || surface == surface_type::TORUS;
    case engine_type::WOLFRAM:
        return surface == surface_type::DEFAULT || surface == surface_type::PLAIN || surface == surface_type::CYLINDER;
    case engine_type::SAND:
        return surface == surface_type::DEFAULT || surface == surface_type::PLAIN || surface == surface_type::CYLINDER;
    default:
        return false;
    }
}

engine_type config::get_automaton_engine() { return options::engine::from_string(automaton.engine); }

surface_type config::get_automaton_surface() {
    engine_type engine = options::engine::from_string(automaton.engine);
    surface_type surface = options::surface::from_string(automaton.surface);

    switch (engine) {
    case engine_type::LIFE:
        return surface == surface_type::DEFAULT ? surface_type::TORUS : surface;
    case engine_type::WOLFRAM:
        return surface == surface_type::DEFAULT ? surface_type::CYLINDER : surface;
    case engine_type::SAND:
        return surface == surface_type::DEFAULT ? surface_type::CYLINDER : surface;
    default:
        return surface_type::INVALID;
    }
}

std::tuple<std::uint16_t, std::uint16_t> config::get_life_rule() {
    return options::rule::from_string(automaton.life_rule);
}

std::string config::get_engine_options() { return options::join<core::engine_type>(); }

std::string config::get_surface_options() { return options::join<core::surface_type>(); }
}  // namespace automaton::core
