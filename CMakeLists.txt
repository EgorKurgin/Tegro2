cmake_minimum_required(VERSION 3.24)

project(Tegro2
        VERSION 0.1.0
        DESCRIPTION "Chemical kinetics of conjugated reactions under diffusion control"
        LANGUAGES CXX
)

# ===== C++ standard =====
set(CMAKE_CXX_STANDARD          23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS        OFF)

# ====== Default build type =====
if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# ===== Project options =====
option(TEGRO_BUILD_TESTS "Build unit and integration tests" ON)
option(TEGRO_BUILD_CLI   "Build command-line interface"     ON)
option(TEGRO_BUILD_GUI   "Build Qt GUI (requires Qt6)"      OFF)

# ---- Вспомогательные модули ----
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
include(Warnings)
include(Dependencies)

# ---- Подпроекты ----
add_subdirectory(src/core)

if(TEGRO_BUILD_CLI)
    add_subdirectory(src/cli)
endif()

if(TEGRO_BUILD_GUI)
    add_subdirectory(src/gui)
endif()

if(TEGRO_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()