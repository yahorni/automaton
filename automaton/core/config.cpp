#include "automaton/core/config.hpp"

#include "automaton/core/ant_rule.hpp"
#include "automaton/core/defaults.hpp"
#include "automaton/core/join_options.hpp"
#include "automaton/core/life_rule.hpp"
#include "automaton/core/surface_type.hpp"

#include <string>
#include <tuple>

namespace automaton::core {

std::tuple<bool, std::string> config::validate() const {
    if (!_validate_cell_width()) return {false, "cell-width"};
    if (!_validate_cols()) return {false, "cols"};
    if (!_validate_rows()) return {false, "rows"};

    if (!_validate_engine()) return {false, "engine"};
    if (!_validate_surface()) return {false, "surface"};
    if (!_validate_rule()) return {false, "rule"};

    if (!_validate_pause()) return {false, "pause"};

    if (!_validate_surface_for_engine()) return {false, "surface"};

    return {true, ""};
}

bool config::_validate_cell_width() const {
    return grid.cell_width > defaults::cell::min_width && grid.cell_width < defaults::cell::max_width;
}

bool config::_validate_cols() const { return grid.initial_cols >= 0; }
bool config::_validate_rows() const { return grid.initial_rows >= 0; }

bool config::_validate_engine() const { return options::engine::is_valid(automaton.engine); }
bool config::_validate_surface() const { return options::surface::is_valid(automaton.surface); }

bool config::_validate_rule() const {
    if (automaton.rule.empty()) return true;

    switch (get_automaton_engine()) {
    case core::engine_type::WOLFRAM:
        return std::stoi(automaton.rule) >= 0 && std::stoi(automaton.rule) <= 255;
    case core::engine_type::LIFE:
        return options::life::is_valid(automaton.rule);
    case core::engine_type::ANT:
        return rules::ant::is_valid(automaton.rule);
    default:
        return automaton.rule.empty();
    }
}

bool config::_validate_pause() const { return animation.delay > 0; }

bool config::_validate_surface_for_engine() const {
    engine_type engine = options::engine::from_string(automaton.engine);
    surface_type surface = options::surface::from_string(automaton.surface);

    switch (engine) {
    case engine_type::WOLFRAM:
        return surface == surface_type::DEFAULT || surface == surface_type::PLAIN || surface == surface_type::CYLINDER;
    case engine_type::SAND:
        return surface == surface_type::DEFAULT || surface == surface_type::PLAIN || surface == surface_type::CYLINDER;
    case engine_type::LIFE:
        return surface == surface_type::DEFAULT || surface == surface_type::PLAIN || surface == surface_type::TORUS;
    case engine_type::ANT:
        return surface == surface_type::DEFAULT || surface == surface_type::PLAIN || surface == surface_type::TORUS;
    default:
        return false;
    }
}

engine_type config::get_automaton_engine() const { return options::engine::from_string(automaton.engine); }

surface_type config::get_automaton_surface() const {
    engine_type engine = options::engine::from_string(automaton.engine);
    surface_type surface = options::surface::from_string(automaton.surface);

    switch (engine) {
    case engine_type::WOLFRAM:
        return surface == surface_type::DEFAULT ? surface_type::CYLINDER : surface;
    case engine_type::SAND:
        return surface == surface_type::DEFAULT ? surface_type::CYLINDER : surface;
    case engine_type::LIFE:
        return surface == surface_type::DEFAULT ? surface_type::TORUS : surface;
    case engine_type::ANT:
        return surface == surface_type::DEFAULT ? surface_type::TORUS : surface;
    default:
        return surface_type::INVALID;
    }
}

uint8_t config::get_wolfram_code() const {
    return automaton.rule.empty() ? defaults::rules::wolfram : std::stoi(automaton.rule);
}

std::tuple<uint16_t, uint16_t> config::get_life_rule() const {
    return options::life::from_string(automaton.rule.empty() ? defaults::rules::life : automaton.rule);
}

std::string config::get_life_presets() {
    static const std::string life_presets = options::life::get_preset_names();
    return life_presets;
}

rules::ant config::get_ant_rule() const {
    return rules::ant(automaton.rule.empty() ? defaults::rules::ant : automaton.rule);
}

std::string config::get_engine_options() {
    static const std::string engine_options = options::join<core::engine_type>();
    return engine_options;
}

std::string config::get_surface_options() {
    static const std::string surface_options = options::join<core::surface_type>();
    return surface_options;
}

}  // namespace automaton::core
