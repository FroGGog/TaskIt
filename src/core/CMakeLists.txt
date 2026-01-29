add_library(core STATIC
    task.cpp
    task.h
    task_manager.cpp
    task_manager.h
    parse_input.cpp
    parse_input.h
)

target_include_directories(core PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)

target_compile_features(core PUBLIC cxx_std_17)