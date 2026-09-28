add_library(tegro_core
    src/numerical/vector.cpp
)

target_include_directories(tegro_core
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_compile_features(tegro_core PUBLIC cxx_std_23)