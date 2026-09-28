#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include <tegro/numerical/vector.hpp>

TEST_CASE("Vector: construction and basic access") {
    tegro::Vector vector(10);
    CHECK(vector.size() == 10);

    vector[3] = 42.0;
    CHECK(vector[3] == 42.0);
}