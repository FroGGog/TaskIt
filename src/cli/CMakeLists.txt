add_executable(TaskIt main.cpp parse_input.cpp)

target_compile_features(TaskIt PRIVATE cxx_std_17)

target_link_libraries(TaskIt PRIVATE core)