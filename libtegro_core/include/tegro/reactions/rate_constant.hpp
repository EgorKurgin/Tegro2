#pragma once

#include <tegro/types.hpp>

namespace tegro {

class MonomolecularRate {
    public:
        virtual ~MonomolecularRate() = default;
        virtual real K(real t) const = 0;
};

class ConstantRate : public MonomolecularRate {
    public:
        explicit ConstantRate(real rate_constant) 
                    : rate_constant_(rate_constant) {}
        
        real K(real) const override {
            return rate_constant_;
        }

    private:
        real rate_constant_;
};

}