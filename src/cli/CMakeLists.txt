add_executable(tegro_cli
        main.cpp
)

target_link_libraries(tegro_cli PRIVATE Tegro::Core)

tegro_set_warnings(tegro_cli)