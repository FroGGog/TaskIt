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
#include "Settings.h"

struct UsedMaterials
{
    std::shared_ptr<sf::Font> global_font;

    UsedMaterials();
};

namespace gui
{

    class GuiElement
    {
        public:
            GuiElement() = default;

            virtual void draw(sf::RenderWindow& r_wind) const = 0;
    };

    class TaskBox : public GuiElement
    {
    public:
        TaskBox(const Task& task, const int task_index, const UsedMaterials& materials);

        void draw(sf::RenderWindow& r_wind) const override;

        void setPosition(sf::Vector2f pos);
        void setColor(sf::Color color);

        sf::FloatRect getGlobalBounds() const;

    private:

        std::string getCurrentTime();

        void updateTextPositions();

        int m_task_index;

        std::unique_ptr<sf::Text> m_text_title;
        std::unique_ptr<sf::Text> m_text_descr;
        std::unique_ptr<sf::Text> m_text_create_time;

        std::tm m_create_time{};

        sf::RectangleShape m_background_shape;
    };

    class KanbanCollumn : public GuiElement
    {
    public:
        KanbanCollumn(std::string title ,const UsedMaterials& materials);

        void setPosition(sf::Vector2f pos);
        void setIndicatorColor(sf::Color color);
        void setHeaderTitle(std::string str);

        sf::FloatRect getGlobalBounds() const;

        void draw(sf::RenderWindow& r_wind) const override;

        // void addTask(std::shared_ptr<TaskBox> taskBox);
        void addTaskToLayout(const Task& task, const int task_index, const UsedMaterials& materials);

    private:
        sf::RectangleShape m_body_box;
        sf::RectangleShape m_title_box;
        sf::RectangleShape m_indicator_box;
        sf::RectangleShape m_layout_box;

        std::unique_ptr<sf::Text> m_header_text;

        std::vector<std::shared_ptr<TaskBox>> m_task_box_in_collumn;
        int m_max_task_in_collumn;

        void setUpLayout();

    };

}

