#include <tegro/reactions/reaction.hpp>
#include <tegro/reactions/system_builder.hpp>

namespace tegro {

void Monomolecular::assemble(SystemBuilder& b) const {
    b.add_mono(substrate, product, kf.get());
    if (kb) b.add_mono(product, substrate, kb.get());
};

}