#pragma once

#include <string>
#include <cstdint>

struct Reactant {
    std::string name;
    std::float64_t radius          = 2.5;
    std::float64_t charge          = 0.0;
    std::float64_t diffusion       = 0.5e-5;
    std::float64_t concentration   = 0.1;
    bool is_constant_concentration = false;
}