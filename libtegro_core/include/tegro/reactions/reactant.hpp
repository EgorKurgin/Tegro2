#pragma once

#include <string>

#include <tegro/types.hpp>

namespace tegro {

struct Reactant {
    std::string name;
    real radius          = 2.5;
    real charge          = 0.0;
    real diffusion       = 0.5e-5;
    real concentration   = 0.1;
    bool is_constant_concentration = false;
};

}