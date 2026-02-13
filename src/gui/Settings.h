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

    constexpr sf::Color TASK_TEXT {30, 30, 35, 255};
};

namespace TaskItSettings
{
    constexpr float SMALL_PADDING = 5.f;
    constexpr float MEDIUM_PADDING = 10.f;
    constexpr float BIG_PADDING = 15.f;
    
    constexpr unsigned int FPS = 60;

    const sf::VideoMode WIN_RESOLUTION(sf::Vector2u{800, 600});
    const sf::String WIN_TITLE{"TaskIt"};
} 
