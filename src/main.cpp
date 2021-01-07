#include <gtkmm/application.h>

#include "base/grid.hpp"
#include "grid_2d.hpp"
#include "grid_3d.hpp"
#include "grid_window.hpp"

int main(int argc, char* argv[]) {
    auto app = Gtk::Application::create(argc, argv, "org.gtkmm.Automaton",
                                        Gio::APPLICATION_HANDLES_COMMAND_LINE);

    automaton::grid_window gw;
    app->signal_command_line().connect(
        sigc::bind(sigc::mem_fun(gw, &automaton::grid_window::on_cmdline), app),
        false);

    return app->run(gw);
}
