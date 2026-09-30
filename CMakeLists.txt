cmake_minimum_required(VERSION 4.0)

project(Tegro2
        VERSION 0.1.0
        LANGUAGES CXX 
)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(TEGRO_BUILD_CLI "Build CLI" OFF)
option(TEGRO_BUILD_GUI "Build GUI" OFF)
option(TEGRO_BUILD_TESTS "Build Tests" OFF)

add_subdirectory(libtegro_core)

if(TEGRO_BUILD_CLI)
    add_subdirectory(apps/cli)
endif()

if(TEGRO_BUILD_GUI)
    add_subdirectory(apps/gui)
endif()

if(TEGRO_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()