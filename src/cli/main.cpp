#include <tegro/core.hpp>

#include <cstdio>

int main(int /*argc*/, char** /*argv*/) {
    std::printf("Tegro CLI v%s\n", tegro::core::version_string);
    std::printf("Not yet implemented.\n");
    return 0;
}