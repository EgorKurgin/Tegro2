add_library(tegro_core STATIC
        src/version.cpp
)

add_library(Tegro::Core ALIAS tegro_core)

target_include_directories(tegro_core
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

# Генерация version.hpp в стандартном месте
configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/include/tegro/core/version.hpp.in
        ${CMAKE_CURRENT_BINARY_DIR}/include/tegro/core/version.hpp
        @ONLY
)

target_compile_features(tegro_core PUBLIC cxx_std_23)

tegro_set_warnings(tegro_core)