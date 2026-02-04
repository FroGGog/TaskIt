#pragma once
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Color.hpp>

#include <string>
#include <chrono>
#include <ctime>
#include <iostream>
#include <sstream>

#include "task.h"

struct UsedMaterials
{
    std::shared_ptr<sf::Font> task_box_font;

    UsedMaterials();
};

namespace gui
{
    class TaskBox
{
public:
    TaskBox(const Task& task, const int task_index, const UsedMaterials& materials);

    void draw(sf::RenderWindow& r_wind);

private:

    std::string getCurrentTime();

    int m_task_index;

    std::unique_ptr<sf::Text> m_text_title;
    std::unique_ptr<sf::Text> m_text_descr;
    std::unique_ptr<sf::Text> m_text_create_time;

    std::tm m_create_time{};

    sf::RectangleShape m_background_shape;

};
}

