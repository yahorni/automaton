#include "automaton/app/app_window.hpp"
#include "automaton/grid/grid_1d.hpp"
#include "automaton/grid/grid_2d.hpp"
#include "automaton/grid/grid_3d.hpp"
#include "automaton/logic/fall.hpp"
#include "automaton/logic/life.hpp"
#include "automaton/logic/wolfram.hpp"

#include <glibmm/optioncontext.h>

#include <format>
#include <iostream>
#include <memory>
#include <string>

namespace automaton {

app_window::app_window()
    : _grid(nullptr) {
    set_title("Automaton");
    set_default_size(800, 600);
    add(_frame);

    _frame.property_margin() = 10;
    _frame.add(_area);
    _frame.property_visible() = true;

    _area.property_visible() = true;

    signal_key_press_event().connect(sigc::mem_fun(*this, &app_window::on_key_press));
    signal_check_resize().connect(sigc::mem_fun(*this, &app_window::on_resize));
}

bool app_window::on_key_press(GdkEventKey* ev) {
    g_debug("grid_window::on_key_press(key='%s')", ev->string);
    if (ev->keyval == GDK_KEY_q) {
        close();
        return true;
    }
    return false;
}

static Glib::OptionGroup _add_ui_group(app_config::ui_options* options) {
    // NOTE:
    // https://gitlab.gnome.org/GNOME/glibmm/-/blob/glibmm-2-64/examples/options/main.cc

    Glib::OptionGroup group("options", "Main options");

    Glib::OptionEntry entry;
    entry.set_short_name('c');
    entry.set_long_name("cols");
    entry.set_arg_description("NUMBER");
    entry.set_description(
        Glib::ustring::compose("Grid columns. Should be >= 0. '0' means fill window. Default: %1", options->cols));
    group.add_entry(entry, options->cols);

    entry = Glib::OptionEntry();
    entry.set_short_name('r');
    entry.set_long_name("rows");
    entry.set_arg_description("NUMBER");
    entry.set_description(
        Glib::ustring::compose("Grid rows. Should be >= 0. '0' means fill window. Default: %1", options->rows));
    group.add_entry(entry, options->rows);

    entry = Glib::OptionEntry();
    entry.set_short_name('b');
    entry.set_long_name("borders");
    entry.set_description("Draw borders between cells in grid");
    group.add_entry(entry, options->borders);

    entry = Glib::OptionEntry();
    entry.set_long_name("delay");
    entry.set_arg_description("NUMBER");
    entry.set_description(Glib::ustring::compose("Sets delay between grid updates in ms. Default: %1", options->delay));
    group.add_entry(entry, options->delay);

    entry = Glib::OptionEntry();
    entry.set_long_name("cell-width");
    entry.set_arg_description("NUMBER");
    entry.set_description(Glib::ustring::compose("Sets cell width in pixels. Default: %1", options->cell_width));
    group.add_entry(entry, options->cell_width);

    return group;
}

static Glib::OptionGroup _add_automaton_group(app_config::automaton_options* options) {
    Glib::OptionGroup group("automaton", "Automaton options");

    auto entry = Glib::OptionEntry();
    entry.set_short_name('l');
    entry.set_long_name("logic");
    entry.set_arg_description("STRING");
    entry.set_description(
        Glib::ustring::compose("Grid logic. Should be 'wolfram', 'fall' or 'life'. Default: '%1'", options->logic));
    group.add_entry(entry, options->logic);

    entry = Glib::OptionEntry();
    entry.set_long_name("code");
    entry.set_arg_description("NUMBER");
    entry.set_description(
        Glib::ustring::compose("Wolfram code. Should be from 0 to 255. Default: %1", options->wf_code));
    group.add_entry(entry, options->wf_code);

    entry = Glib::OptionEntry();
    entry.set_long_name("splices");
    entry.set_arg_description("NUMBER");
    entry.set_description(Glib::ustring::compose(
        "Grid splices amount. Should be >= 0. '0' means unlimited depth. Default: %1", options->fall_splices));
    group.add_entry(entry, options->fall_splices);

    return group;
}

int app_window::on_cli(const app_cli_ptr& cli, app_ptr& app) {
    g_debug("grid_window::on_cmdline()");
    // NOTE:
    // https://gitlab.gnome.org/GNOME/glibmm/-/blob/glibmm-2-64/examples/options/main.cc

    Glib::OptionContext ctx;

    auto main_group = _add_ui_group(&_options.ui);
    ctx.add_group(main_group);

    auto wolfram_group = _add_automaton_group(&_options.am);
    ctx.add_group(wolfram_group);

    // add GTK options, --help-gtk, etc
    Glib::OptionGroup gtkgroup(gtk_get_option_group(true));
    ctx.add_group(gtkgroup);

    int argc;
    char** argv = cli->get_arguments(argc);

    try {
        ctx.parse(argc, argv);
    } catch (Glib::OptionError& e) {
        g_warning("%s", e.what().c_str());
        std::cout << e.what() << std::endl;
        return 1;
    }

    auto invalid_value = [](const Glib::ustring& param) {
        Glib::ustring msg = "Invalid value for param " + param;
        g_warning("%s", msg.c_str());
        std::cout << msg << std::endl;
    };

    auto validation = _options.validate();
    if (!std::get<0>(validation)) {
        invalid_value(std::get<1>(validation));
        return -1;
    }

    app->activate();
    initialize_grid();
    return 0;
}

std::string app_window::get_status() const {
    std::string details;
    if (_options.am.logic == "fall")
        details = std::format("splices: {}", _options.am.fall_splices);
    else if (_options.am.logic == "wolfram")
        details = std::format("code: {}", _options.am.wf_code);

    return std::format("logic: {}, {}, cols: {}, rows: {}", _options.am.logic.c_str(), (details.size() ? details : ""),
                       _grid->get_cols(), _grid->get_rows());
}

void app_window::initialize_grid() {
    g_debug("grid_window::initialize_grid(logic='%s')", _options.am.logic.c_str());

    uint32_t cols = _options.ui.cols == 0 ? _area.get_width() / _area.get_cell_width() : _options.ui.cols;
    uint32_t rows = _options.ui.rows == 0 ? _area.get_height() / _area.get_cell_width() : _options.ui.rows;

    // initialize grid
    if (_options.am.logic == "wolfram") {
        _grid = std::make_shared<automaton::grid_1d>(rows, cols);
        _logic = std::make_shared<logic::wolfram>(_grid, _options.am.wf_code);

    } else if (_options.am.logic == "fall" && _options.am.fall_splices != 1) {
        _grid = std::make_shared<automaton::grid_3d>(rows, cols);
        _logic = std::make_shared<logic::fall_3d>(_grid, _options.am.fall_splices);

    } else {  // "life" or "fall" with splices == 1
        _grid = std::make_shared<automaton::grid_2d>(rows, cols);

        if (_options.am.logic == "fall") {
            _logic = std::make_shared<logic::fall_2d>(_grid);
        } else if (_options.am.logic == "life") {
            _logic = std::make_shared<logic::life_2d>(_grid);
        }
    }

    // setup grid drawing area
    _area.set_automaton(_logic, _grid);
    _area.set_grid_borders(_options.ui.borders);
    _area.set_step_delay(_options.ui.delay);
    _area.set_cell_width(_options.ui.cell_width);
    _area.set_information(get_status());
}

void app_window::on_resize() {
    g_debug("grid_window::on_resize()");
    if (!_grid) return;

    if (_options.ui.cols == 0) _grid->set_cols(_area.get_width() / _area.get_cell_width());
    if (_options.ui.rows == 0) _grid->set_rows(_area.get_height() / _area.get_cell_width());

    _area.set_information(get_status());
}

}  // namespace automaton
