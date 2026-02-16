#include <SFML/Graphics/Color.hpp>
#include <SFML/Window/VideoMode.hpp>

namespace TaskItColors
{
    constexpr sf::Color WIN_BG {30, 30, 35, 255}; 

    // Kanban collumns
    constexpr sf::Color COLLUMN {45, 45, 50, 255};
    constexpr sf::Color COLLUMN_HEADER {60, 60, 65, 255};

    constexpr sf::Color INDICATOR_TODO {220, 100, 100, 255};   
    constexpr sf::Color INDICATOR_PROGRESS {240, 180, 80, 255};
    constexpr sf::Color INDICATOR_DONE {100, 200, 120, 255};

    constexpr sf::Color COLLUMN_HEADER_TEXT {240, 240, 245, 255};

    // Tasks
    constexpr sf::Color TASK_TODO {200, 140, 140, 255};
    constexpr sf::Color TASK_PROGRESS {230, 190, 120, 255}; 
    constexpr sf::Color TASK_DONE {140, 190, 160, 255};     

    constexpr sf::Color TASK_TODO_HOVER {220, 155, 155, 255};     
    constexpr sf::Color TASK_PROGRESS_HOVER {250, 205, 135, 255};
    constexpr sf::Color TASK_DONE_HOVER {155, 205, 175, 255};

    constexpr sf::Color TASK_TEXT {30, 30, 35, 255};
};

namespace TaskItSettings
{
    constexpr float SMALL_PADDING = 5.f;
    constexpr float MEDIUM_PADDING = 10.f;
    constexpr float BIG_PADDING = 15.f;
    
    constexpr unsigned int FPS = 30;

    const sf::VideoMode WIN_RESOLUTION(sf::Vector2u{800, 600});
    const sf::String WIN_TITLE{"TaskIt"};
} 

namespace TaskItSizes
{
    // Task size
    constexpr float TASK_SIZE_X = 187.f;
    constexpr float TASK_SIZE_Y = 90.f;

    // Kanban sizes
    constexpr float COLLUMN_SIZE_X = 220.f;
    constexpr float COLLUMN_SIZE_Y = 550.f;

    constexpr float COLLUMN_TITLE_SIZE_Y = 30.f;
    constexpr float COLLUMN_INDICATOR_SIZE_Y = 5.f;


}