#pragma once

#include <cstdfloat>

namespace tegro {

class MonomolecularRate {
    public:
        virtual std::float64_t at(std::float64_t t) const = 0;
};

class ConstantRate : public MonomolecularRate {
    public:
        ConstantRate(std::float64_t C);
        
        std::float64_t at(std::float64_t) const override;

    private:
        std::float64_t C_ = 1.0;
};

}