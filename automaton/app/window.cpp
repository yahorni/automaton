#include "automaton/app/window.hpp"

#include "automaton/app/animation.hpp"
#include "automaton/app/controller.hpp"
#include "automaton/core/defaults.hpp"
#include "automaton/engines/ant.hpp"
#include "automaton/engines/engine.hpp"
#include "automaton/engines/life.hpp"
#include "automaton/engines/sand.hpp"
#include "automaton/engines/wolfram.hpp"

#include <glibmm/optioncontext.h>
#include <glibmm/optionentry.h>

#include <iostream>
#include <memory>

namespace automaton::app {

window::window()
    : _canvas(_grid) {
    set_title("Automaton");
    set_default_size(defaults::window_width, defaults::window_height);
    add(_frame);

    _frame.property_margin() = defaults::window_margin;
    _frame.add(_canvas);
    _frame.property_visible() = true;

    _canvas.property_visible() = true;

    signal_key_press_event().connect(sigc::mem_fun(*this, &window::_on_key_press));
    signal_check_resize().connect(sigc::mem_fun(_canvas, &canvas::on_resize));
}

int window::on_command_line(const app_cli_ptr& cli, const app_ptr& app) {
    g_debug("window::on_command_line()");
    if (!_parse_command_line(cli)) return 1;
    app->activate();
    _initialize();
    return 0;
}

void window::_initialize() {
    g_debug("window::initialize(engine='%s',surface='%s')",  //
            _config.automaton.engine.c_str(), _config.automaton.surface.c_str());

    engines::parameters engine_params{_config.get_automaton_surface(), _grid};
    engine_ptr engine;

    switch (_config.get_automaton_engine()) {
    case core::engine_type::WOLFRAM: {
        engine = std::make_unique<engines::wolfram>(engine_params, _config.get_wolfram_code());
        if (_config.animation.enable) {
            g_warning("window::initialize(): animation disabled for wolfram on start");
            _config.animation.enable = false;
        }
        break;
    }
    case core::engine_type::SAND: {
        engine = std::make_unique<engines::sand>(engine_params);
        break;
    }
    case core::engine_type::LIFE: {
        const auto [birth, survival] = _config.get_life_rule();
        engine = std::make_unique<engines::life>(engine_params, birth, survival);
        break;
    }
    case core::engine_type::ANT: {
        engine = std::make_unique<engines::ant>(engine_params);
        break;
    }
    default: {
        std::unreachable();
        break;
    }
    }

    auto animation_ = std::make_unique<animation>(_config.animation.pause, _config.animation.enable);
    _ctrl = std::make_shared<controller>(std::move(animation_), std::move(engine));

    canvas_config cfg{
        .initial_size = {_config.grid.initial_rows, _config.grid.initial_cols},
        .cell_width = _config.grid.cell_width,
        .show_borders = _config.grid.show_borders,
        .show_status = defaults::enable_status,
        .adapt_to_window = _config.grid.adapt_to_window,
    };
    _canvas.initialize(cfg, _ctrl);
}

bool window::_on_key_press(GdkEventKey* ev) {
    g_debug("window::on_key_press(key='%s')", ev->string);
    if (ev->keyval == GDK_KEY_q) {
        close();
        return true;
    }
    return false;
}

static Glib::OptionGroup _add_grid_group(core::config::grid_group*);
static Glib::OptionGroup _add_automaton_group(core::config::automaton_group*);
static Glib::OptionGroup _add_animation_group(core::config::animation_group*);

bool window::_parse_command_line(const app_cli_ptr& cli) {
    g_debug("window::parse_command_line()");

    // https://gitlab.gnome.org/GNOME/glibmm/-/blob/glibmm-2-64/examples/options/main.cc
    Glib::OptionContext ctx;

    auto grid_group = _add_grid_group(&_config.grid);
    ctx.add_group(grid_group);

    auto automaton_group = _add_automaton_group(&_config.automaton);
    ctx.add_group(automaton_group);

    auto animation_group = _add_animation_group(&_config.animation);
    ctx.add_group(animation_group);

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
        return false;
    }

    auto validation = _config.validate();
    if (!std::get<0>(validation)) {
        Glib::ustring msg = "Invalid value for option: " + std::get<1>(validation);
        g_warning("%s", msg.c_str());
        std::cout << msg << std::endl;
        return false;
    }

    return true;
}

static Glib::OptionGroup _add_grid_group(core::config::grid_group* opts) {
    // NOTE:
    // https://gitlab.gnome.org/GNOME/glibmm/-/blob/glibmm-2-64/examples/options/main.cc
    Glib::OptionGroup group("options", "Grid options");
    Glib::OptionEntry entry;

    entry = Glib::OptionEntry();
    entry.set_short_name('w');
    entry.set_long_name("cell-width");
    entry.set_arg_description("NUMBER");
    entry.set_description(Glib::ustring::compose("Sets cell width in pixels. Default: %1", opts->cell_width));
    group.add_entry(entry, opts->cell_width);

    entry = Glib::OptionEntry();
    entry.set_short_name('b');
    entry.set_long_name("borders");
    entry.set_description("Show borders between cells in grid");
    group.add_entry(entry, opts->show_borders);

    entry = Glib::OptionEntry();
    entry.set_short_name('c');
    entry.set_long_name("cols");
    entry.set_arg_description("NUMBER");
    entry.set_description(Glib::ustring::compose("Grid columns. Should be >= 0. '0' means to fill window. Default: %1",
                                                 opts->initial_cols));
    group.add_entry(entry, opts->initial_cols);

    entry = Glib::OptionEntry();
    entry.set_short_name('r');
    entry.set_long_name("rows");
    entry.set_arg_description("NUMBER");
    entry.set_description(
        Glib::ustring::compose("Grid rows. Should be >= 0. '0' means fill window. Default: %1", opts->initial_rows));
    group.add_entry(entry, opts->initial_rows);

    entry = Glib::OptionEntry();
    entry.set_long_name("adapt");
    entry.set_description("Adapt grid size to window");
    group.add_entry(entry, opts->adapt_to_window);

    return group;
}

static Glib::OptionGroup _add_automaton_group(core::config::automaton_group* opts) {
    Glib::OptionGroup group("automaton", "Automaton options");
    Glib::OptionEntry entry;

    entry = Glib::OptionEntry();
    entry.set_short_name('e');
    entry.set_long_name("engine");
    entry.set_arg_description("STRING");
    entry.set_description(Glib::ustring::compose(  //
        "Grid engine. Options: %1. Default: %2", core::config::get_engine_options(), opts->engine));
    group.add_entry_filename(entry, opts->engine);

    entry = Glib::OptionEntry();
    entry.set_short_name('s');
    entry.set_long_name("surface");
    entry.set_arg_description("STRING");
    entry.set_description(Glib::ustring::compose("Surface. Options: %1", core::config::get_surface_options()));
    group.add_entry_filename(entry, opts->surface);

    entry = Glib::OptionEntry();
    entry.set_long_name("rule");
    entry.set_arg_description("STRING");
    entry.set_description(Glib::ustring::compose(  //
        R"(Automaton rule.
    For wolfram: number from 0 to 255. Default: %1
    For Game of Life: uses Hensel notation. Default: %2
    Presets can be used for Game of Life: %3)",
        defaults::wolfram_code, defaults::life_rule, core::config::get_life_presets()));
    group.add_entry_filename(entry, opts->rule);

    return group;
}

static Glib::OptionGroup _add_animation_group(core::config::animation_group* opts) {
    Glib::OptionGroup group("animation", "Animation options");
    Glib::OptionEntry entry;

    entry = Glib::OptionEntry();
    entry.set_short_name('p');
    entry.set_long_name("pause");
    entry.set_arg_description("NUMBER");
    entry.set_description(Glib::ustring::compose("Sets pause between animation steps in ms. Default: %1", opts->pause));
    group.add_entry(entry, opts->pause);

    entry = Glib::OptionEntry();
    entry.set_short_name('a');
    entry.set_long_name("animation");
    entry.set_description(Glib::ustring::compose("Enable animation on start. Default: %1", opts->enable));
    group.add_entry(entry, opts->enable);

    return group;
}

}  // namespace automaton::app
