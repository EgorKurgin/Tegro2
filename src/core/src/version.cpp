#include <version.hpp>

// This file exists to give the static library at least one translation
// unit. It will be replaced by real implementation files as the modules
// are added.

namespace tegro::core {
    namespace {

        [[maybe_unused]] constexpr int version_anchor = 1;

    } // namespace
} // namespace tegro::core