# tests/CMakeLists.txt

include(FetchContent)

FetchContent_Declare(
    doctest
    GIT_REPOSITORY https://github.com/doctest/doctest
    GIT_TAG        v2.4.12
)

FetchContent_MakeAvailable(doctest)

function(tegro_add_test name)
    set(target "${name}_test")

    add_executable(${target} "${name}_test.cpp")

    target_link_libraries(${target}
        PRIVATE
            tegro_core
            doctest::doctest
    )

    # Регистрируем как отдельный тест в CTest
    add_test(NAME ${target} COMMAND ${target})
endfunction()

tegro_add_test(vector)