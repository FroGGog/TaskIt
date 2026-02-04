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

add_executable(TaskItGUI main.cpp MainWindow.cpp GUIElements.cpp)

target_compile_features(TaskItGUI PRIVATE cxx_std_17)

target_link_libraries(TaskItGUI PRIVATE core SFML::Graphics)

# =================COPY ASSET FILES=================

# Add fonts
# Dirs to font
set(FONT_FILE "${CMAKE_SOURCE_DIR}/BrownieStencil-8O8MJ.ttf")

# Copy font file to build dir
add_custom_command(TARGET TaskItGUI POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy ${FONT_FILE} $<TARGET_FILE_DIR:TaskItGUI>
    COMMENT "Copying font file to output directory"
)