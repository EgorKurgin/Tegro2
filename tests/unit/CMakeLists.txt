add_executable(tegro_unit_tests
        test_version.cpp
)

target_link_libraries(tegro_unit_tests PRIVATE
        Tegro::Core
        Catch2::Catch2WithMain
)

catch_discover_tests(tegro_unit_tests)