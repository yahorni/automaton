#pragma once

#include "automaton/core/engine_type.hpp"
#include "automaton/core/grid.hpp"
#include "automaton/core/surface_type.hpp"

#include <memory>
#include <string>

namespace automaton::engines {

struct parameters {
    core::surface_type surface_type;
    core::grid& grid;
};

class engine {
public:
    engine(core::engine_type engine_type, parameters& params)
        : _engine_type(engine_type),
          _surface_type(params.surface_type),
          _grid(params.grid) {}
    virtual ~engine() = default;

    virtual std::string description() const = 0;

    // return false if no steps left
    virtual bool step() = 0;
    virtual void restart() { _step = 0; }
    virtual void clear() {
        _step = 0;
        _grid.clear();
    }

    virtual void action1(size_t row, size_t col) { _grid.set(row, col, 1); }
    virtual void action2(size_t row, size_t col) { _grid.set(row, col, 0); }
    virtual void shift_actions() {}

    virtual void resize(const core::dims& size) { _grid.resize(size); }

protected:
    const core::engine_type _engine_type;
    const core::surface_type _surface_type;
    core::grid& _grid;

    size_t _step = 0;
};

}  // namespace automaton::engines

namespace automaton {
using engine_ptr = std::unique_ptr<engines::engine>;
}  // namespace automaton
