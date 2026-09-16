include(FetchContent)

# ---- Catch2 (only needed for tests) ----
if(TEGRO_BUILD_TESTS)
    FetchContent_Declare(
            Catch2
            GIT_REPOSITORY https://github.com/catchorg/Catch2.git
            GIT_TAG        v3.5.4
            GIT_SHALLOW    TRUE
            FIND_PACKAGE_ARGS NAMES Catch2
    )
    FetchContent_MakeAvailable(Catch2)

    # Enable `catch_discover_tests(...)` for CTest integration
    list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
    include(Catch)
endif()