add_executable(vector_test 
    vector_test.cpp
)

target_link_libraries(vector_test
    PRIVATE 
        tegro_core
)

add_test(
    NAME vector_test
    COMMAND vector_test
)