add_executable(TaskIt main.cpp)

target_compile_features(TaskIt PRIVATE cxx_std_17)

target_link_libraries(TaskIt PRIVATE core)