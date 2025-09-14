#pragma once

#include "automaton/core/engine_type.hpp"
#include "automaton/core/surface_type.hpp"

#include <memory>
#include <string>

namespace automaton::engines {

class engine {
public:
    engine(core::engine_type engine_type, core::surface_type surface_type)
        : _engine_type(engine_type),
          _surface_type(surface_type) {}
    virtual ~engine() = default;

    // return: false if no steps left
    virtual bool step() = 0;
    virtual void reset() {};

    virtual std::string description() const = 0;

protected:
    core::engine_type _engine_type;
    core::surface_type _surface_type;
};

}  // namespace automaton::engines

namespace automaton {
using engine_ptr = std::unique_ptr<engines::engine>;
}  // namespace automaton
