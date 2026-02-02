add_executable(TaskItCLI main.cpp)

target_compile_features(TaskItCLI PRIVATE cxx_std_17)

target_link_libraries(TaskItCLI PRIVATE core)