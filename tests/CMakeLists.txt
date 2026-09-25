add_executable(numerical_test 
    vector_test.cpp
    matrix_test.cpp
)

target_link_libraries(numerical_test
    PRIVATE 
        tegro_core
)

add_test(
    NAME numerical_test
    COMMAND numerical_test
)