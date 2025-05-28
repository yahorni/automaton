#pragma once

#include <glibmm/ustring.h>

#include <string>
#include <tuple>

namespace automaton {

struct app_config {
    struct ui_options {
        int cols = 0;  // 0 - fill widget width
        int rows = 0;  // 0 - fill widget height
        bool borders = false;
        int delay = 100;
        double cell_width = 15;
    } ui;

    struct automaton_options {
        Glib::ustring logic = "fall";  // = wolfram/fall/life
        int wf_code = 22;              // = 0-255
        int fall_splices = 1;          // >= 0 (0 - unlimited splices)
    } am;

    // returns false with failed options, otherwise true
    std::tuple<bool, std::string> validate() const;

private:
    bool validate_cols() const;
    bool validate_rows() const;
    bool validate_delay() const;
    bool validate_cell_width() const;

    bool validate_logic() const;
    bool validate_wolfram() const;
    bool validate_fall() const;
};

}  // namespace automaton
