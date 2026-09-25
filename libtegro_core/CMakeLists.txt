add_library(tegro_core
    src/numerical/vector.cpp 
    src/numerical/matrix.cpp
)

target_include_directories(tegro_core
    PUBLIC 
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)