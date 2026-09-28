add_executable(tegro_gui
    main.cpp
)

target_link_libraries(tegro_gui
    PRIVATE
        tegro_core
)