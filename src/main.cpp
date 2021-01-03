#include <gtkmm/application.h>

#include "grid_window.hpp"

int main(int argc, char *argv[]) {
    auto app = Gtk::Application::create(argc, argv, "org.gtkmm.Automaton");
    automaton::grid_window gw;
    return app->run(gw);
}
