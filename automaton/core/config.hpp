#pragma once

#include "automaton/core/defaults.hpp"
#include "automaton/core/options/engine_type.hpp"
#include "automaton/core/options/surface_type.hpp"
#include "automaton/core/rules/ant.hpp"
#include "automaton/core/rules/life.hpp"

#include <cstdint>
#include <string>
#include <tuple>

namespace automaton::core {

struct config {
    struct grid_group {
        double cell_width = defaults::cell::width;
        bool show_borders = defaults::ui::enable_borders;
        int initial_rows = defaults::grid::initial_rows;
        int initial_cols = defaults::grid::initial_cols;
        bool adapt_to_window = defaults::grid::adapt_to_window;
    } grid;

    struct automaton_group {
        std::string engine{options::engine::sand};
        std::string surface{options::surface::default_};
        std::string rule;
    } automaton;

    struct animation_group {
        int delay = defaults::animation::delay_ms;
        bool enable = defaults::animation::enable;
    } animation;

    // return: false with failed option, otherwise true
    std::tuple<bool, std::string> validate() const;

    engine_type get_automaton_engine() const;
    surface_type get_automaton_surface() const;

    uint8_t get_wolfram_code() const;
    rules::life get_life_rule() const;
    rules::ant get_ant_rule() const;

    static std::string get_life_presets();
    static std::string get_engine_options();
    static std::string get_surface_options();

private:
    bool _validate_cell_width() const;
    bool _validate_cols() const;
    bool _validate_rows() const;

    bool _validate_engine() const;
    bool _validate_surface() const;
    bool _validate_rule() const;

    bool _validate_pause() const;

    bool _validate_surface_for_engine() const;
};

}  // namespace automaton::core
