# src/gui/CMakeLists.txt
include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG master 
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM
)
FetchContent_MakeAvailable(SFML)

add_executable(TaskItGUI main.cpp MainWindow.cpp)

target_compile_features(TaskItGUI PRIVATE cxx_std_17)

target_link_libraries(TaskItGUI PRIVATE core SFML::Graphics)