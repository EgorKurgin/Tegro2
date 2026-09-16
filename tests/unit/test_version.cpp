#include <catch2/catch_test_macros.hpp>

#include <libtegro_core.hpp>

#include <string_view>

TEST_CASE("core: version is available and non-empty", "[core][version]") {
    REQUIRE(tegro::version_major >= 0);
    REQUIRE(tegro::version_minor >= 0);
    REQUIRE(tegro::version_patch >= 0);

    const std::string_view v{tegro::version_string};
    REQUIRE_FALSE(v.empty());
    REQUIRE(v.find('.') != std::string_view::npos);
}