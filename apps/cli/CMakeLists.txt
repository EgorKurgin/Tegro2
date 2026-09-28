add_executable(tegro_cli
    main.cpp
)

target_link_libraries(tegro_cli
    PRIVATE
        tegro_core
)