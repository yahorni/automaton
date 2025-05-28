#include "automaton/ui/app_window.hpp"

#include <gtkmm/application.h>

int main(int argc, char* argv[]) {
    auto app = Gtk::Application::create(argc, argv, "org.gtkmm.Automaton", Gio::APPLICATION_HANDLES_COMMAND_LINE);

    automaton::app_window window;
    app->signal_command_line().connect(sigc::bind(sigc::mem_fun(window, &automaton::app_window::on_cli), app), false);

    return app->run(window);
}
