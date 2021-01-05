#include <gtkmm/application.h>

#include "grid_2d.hpp"
#include "grid_3d.hpp"
#include "grid_window.hpp"
#include "interfaces/base_grid.hpp"

enum class grid_t { _2D, _3D };

int main(int argc, char *argv[]) {
    auto app = Gtk::Application::create(argc, argv, "org.gtkmm.Automaton");

    auto type = grid_t::_3D;
    size_t width = 40;
    size_t height = 50;

    std::shared_ptr<automaton::base_grid> grid;
    switch (type) {
        case grid_t::_2D:
            grid = std::static_pointer_cast<automaton::base_grid>(
                std::make_shared<automaton::grid_2d>(height, width));
            break;
        case grid_t::_3D:
            grid = std::static_pointer_cast<automaton::base_grid>(
                std::make_shared<automaton::grid_3d>(height, width));
            break;
    }

    automaton::grid_window gw(grid);
    return app->run(gw);
}
