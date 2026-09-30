#pragma once

#include <vector>

#include <tegro/types.hpp>

namespace tegro {

struct TimeParameters {
    real t_start           = 0.1;
    real t_end             = 10.0;
    int n_points           = 100;
    bool is_logarithmic    = false;
    real tolerance         = 1e-4;
    real min_concentration = 1e-6;
};

std::vector<real> make_time_grid(const TimeParameters&);

} 