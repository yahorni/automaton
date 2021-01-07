#include "grid_window.hpp"

#include <glibmm/optioncontext.h>

#include <iostream>
#include <memory>

#include "grid_2d.hpp"
#include "grid_3d.hpp"

namespace automaton {

grid_window::grid_window() : _grid(nullptr) {
    set_title("Automaton");
    set_default_size(800, 600);
    add(_frame);

    _frame.property_margin() = 15;
    _frame.add(_area);
    _frame.property_visible() = true;

    _area.property_visible() = true;

    signal_key_press_event().connect(
        sigc::mem_fun(*this, &grid_window::on_key_press));
    signal_check_resize().connect(
        sigc::mem_fun(*this, &grid_window::on_resize));
}

grid_window::~grid_window() {}

bool grid_window::on_key_press(GdkEventKey* ev) {
    if (ev->keyval == GDK_KEY_q) {
        close();
        return true;
    }
    return false;
}

int grid_window::on_cmdline(
    const Glib::RefPtr<Gio::ApplicationCommandLine>& cmdline,
    Glib::RefPtr<Gtk::Application>& app) {
    // NOTE:
    // https://gitlab.gnome.org/GNOME/glibmm/-/blob/glibmm-2-64/examples/options/main.cc

    Glib::OptionContext ctx;
    Glib::OptionGroup group("options", "Main options");

    Glib::OptionEntry entry;
    entry.set_short_name('w');
    entry.set_long_name("width");
    entry.set_arg_description("NUM");
    entry.set_description("Grid width. Should be greater than 0");
    group.add_entry(entry, _options.width);

    entry = Glib::OptionEntry();
    entry.set_short_name('H');
    entry.set_long_name("height");
    entry.set_arg_description("NUM");
    entry.set_description("Grid height. Should be greater than 0");
    group.add_entry(entry, _options.height);

    entry = Glib::OptionEntry();
    entry.set_short_name('t');
    entry.set_long_name("type");
    entry.set_arg_description("TYPE");
    entry.set_description("Grid type. Should be in '2D' or '3D'");
    group.add_entry(entry, _options.type);

    entry = Glib::OptionEntry();
    entry.set_short_name('b');
    entry.set_long_name("borders");
    /* entry.set_flags(Glib::OptionEntry::FLAG_NO_ARG); */
    entry.set_description("Draw borders between cells in grid");
    group.add_entry(entry, _options.borders);

    entry = Glib::OptionEntry();
    entry.set_short_name('d');
    entry.set_long_name("delay");
    entry.set_description("Sets delay between grid updates in ms");
    group.add_entry(entry, _options.delay);

    ctx.add_group(group);

    // add GTK options, --help-gtk, etc
    Glib::OptionGroup gtkgroup(gtk_get_option_group(true));
    ctx.add_group(gtkgroup);

    int argc;
    char** argv = cmdline->get_arguments(argc);

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

    if (!_options.validate_width()) {
        invalid_value("width");
        return -1;
    }
    if (!_options.validate_height()) {
        invalid_value("height");
        return -1;
    }
    if (!_options.validate_type()) {
        invalid_value("type");
        return -1;
    }
    if (!_options.validate_delay()) {
        invalid_value("delay");
        return -1;
    }

    app->activate();

    initialize_grid();

    return 0;
}

void grid_window::initialize_grid() {
    size_t width =
        (_options.width == 0 ? _area.get_width() / _area.get_cell_width()
                             : _options.width);
    size_t height =
        (_options.height == 0 ? _area.get_height() / _area.get_cell_width()
                              : _options.height);

    // initialize grid
    if (_options.type == "2D")
        _grid = std::static_pointer_cast<automaton::base_grid>(
            std::make_shared<automaton::grid_2d>(height, width));
    else if (_options.type == "3D")
        _grid = std::static_pointer_cast<automaton::base_grid>(
            std::make_shared<automaton::grid_3d>(height, width));

    // setup grid drawing area
    _area.set_grid(_grid);
    _area.set_grid_borders(_options.borders);
}

void grid_window::on_resize() {
    if (!_grid) return;

    if (_options.width == 0) {
        size_t width = _area.get_width() / _area.get_cell_width();
        _grid->set_cols(width);
    }

    if (_options.height == 0) {
        size_t height = _area.get_height() / _area.get_cell_width();
        _grid->set_rows(height);
    }
}

}  // namespace automaton
