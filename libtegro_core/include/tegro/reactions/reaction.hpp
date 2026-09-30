#pragma once

#include <string>
#include <memory>

#include <tegro/types.hpp>
#include <tegro/reactions/rate_constant.hpp>
#include <tegro/reactions/system_builder.hpp>

namespace tegro {

class Reaction {
    public: 
        virtual ~Reaction() = default;
        virtual void assemble(SystemBuilder&) const = 0;
};

class Monomolecular : public Reaction {
    public: 
        void assemble(SystemBuilder&) const override; 

        std::string substrate;
        std::string product;
        std::unique_ptr<MonomolecularRate> kf;
        std::unique_ptr<MonomolecularRate> kb;
};

}