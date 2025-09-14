#pragma once

#include "automaton/core/engine_type.hpp"
#include "automaton/core/grid.hpp"
#include "automaton/core/surface_type.hpp"

#include <memory>
#include <string>

namespace automaton::engines {

class engine {
public:
    engine(core::grid& grid, core::engine_type engine_type, core::surface_type surface_type)
        : _grid(grid),
          _engine_type(engine_type),
          _surface_type(surface_type) {}
    virtual ~engine() = default;

    bool step() {
        _step++;
        return do_step();
    }
    void restart() {
        _step = 0;
        do_restart();
    }
    void clear() {
        _grid.clear();
        _step = 0;
        do_clear();
    }

    virtual void activate(size_t row, size_t col) { _grid.set(row, col, 1); }
    virtual void deactivate(size_t row, size_t col) { _grid.set(row, col, 0); }

    virtual std::string description() const = 0;

    size_t current_step() const { return _step; }

protected:
    // return false if no steps left
    virtual bool do_step() = 0;
    virtual void do_restart() {};
    virtual void do_clear() {};

    core::grid& _grid;
    core::engine_type _engine_type;
    core::surface_type _surface_type;

private:
    size_t _step = 0;
};

}  // namespace automaton::engines

namespace automaton {
using engine_ptr = std::unique_ptr<engines::engine>;
}  // namespace automaton
