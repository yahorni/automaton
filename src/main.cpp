#include <gtkmm/application.h>

#include "field_window.hpp"

int main(int argc, char *argv[]) {
    auto app = Gtk::Application::create(argc, argv, "org.gtkmm.Automaton");
    FieldWindow fw;
    return app->run(fw);
}
