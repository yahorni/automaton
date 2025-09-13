#pragma once

#include "automaton/core/engine_type.hpp"
#include "automaton/core/surface_type.hpp"

#include <string>
#include <tuple>

namespace automaton::core {

struct config {
    struct ui_group {
        double cell_width = 10;
        int delay = 100;
        bool animation = false;
        bool borders = false;
    } ui;

    struct automaton_group {
        std::string engine{options::engine::sand};
        std::string surface{options::surface::default_};
        int cols = 0;           // 0 - fill widget width
        int rows = 0;           // 0 - fill widget height
        int wolfram_code = 22;  // = 0-255

    } automaton;

    // return: false with failed options, otherwise true
    std::tuple<bool, std::string> validate() const;

    engine_type get_automaton_engine();
    surface_type get_automaton_surface();

    static std::string get_engine_options();
    static std::string get_surface_options();

private:
    bool _validate_cell_width() const;
    bool _validate_delay() const;

    bool _validate_engine() const;
    bool _validate_surface() const;
    bool _validate_cols() const;
    bool _validate_rows() const;
    bool _validate_wolfram_code() const;

    bool _validate_surface_for_engine() const;
};

}  // namespace automaton::core
