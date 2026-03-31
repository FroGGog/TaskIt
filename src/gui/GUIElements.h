#pragma once
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/CircleShape.hpp>


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
            virtual ~GuiElement() = default;

            virtual void draw(sf::RenderWindow& r_wind) const = 0;

            virtual sf::FloatRect getGlobalBounds() const = 0;
    };

    class TaskBox : public GuiElement
    {
    public:
        TaskBox(const Task& task, const int task_index, const UsedMaterials& materials);

        void draw(sf::RenderWindow& r_wind) const override;

        void setPosition(sf::Vector2f pos);
        void setColor(sf::Color color);
        void setIsChoosen(bool is_choosen);

        [[nodiscard]] sf::FloatRect getGlobalBounds() const override;
        [[nodiscard]] int getIndex() const;
        [[nodiscard]] TaskStatus getStatus() const;
        [[nodiscard]] bool isChoosen() const;

    private:

        void updateTextPositions();

        int m_task_index;

        std::unique_ptr<sf::Text> m_text_title;
        std::unique_ptr<sf::Text> m_text_descr;
        std::unique_ptr<sf::Text> m_text_create_time;

        sf::RectangleShape m_background_shape;

        TaskStatus m_current_status;

        bool m_is_choosen = false;
    };

    class KanbanCollumn : public GuiElement
    {
    public:
        KanbanCollumn(std::string title ,const UsedMaterials& materials);

        void setPosition(sf::Vector2f pos);
        void setIndicatorColor(sf::Color color);
        void setHeaderTitle(std::string str);
        void setCollumnTaskState(TaskStatus status);

        [[nodiscard]] sf::FloatRect getGlobalBounds() const override;
        [[nodiscard]] TaskStatus getCollumnTaskStatus() const;

        void draw(sf::RenderWindow& r_wind) const override;
        void drawTasks(sf::RenderWindow& r_wind) const;

        // void addTask(std::shared_ptr<TaskBox> taskBox);
        void addTaskToLayout(const Task& task, const int task_index, const UsedMaterials& materials);

        [[nodiscard]] bool taskInColumn(int task_id);
        std::vector<std::shared_ptr<gui::TaskBox>>& getAllTasks();
        void clearCollumn();

    private:
        sf::RectangleShape m_body_box;
        sf::RectangleShape m_title_box;
        sf::RectangleShape m_indicator_box;
        sf::RectangleShape m_layout_box;

        TaskStatus m_collumn_task_state;
        std::unique_ptr<sf::Text> m_header_text;

        std::vector<std::shared_ptr<gui::TaskBox>> m_task_box_in_collumn;
        int m_max_task_in_collumn;

        void setUpLayout();

    };

    class Button : public GuiElement
    {
    public:

        Button();

        void setPosition(sf::Vector2f pos);

        [[nodiscard]] sf::FloatRect getGlobalBounds() const override;
        
        void draw(sf::RenderWindow& r_wind) const override;

    private:

        sf::RectangleShape m_button_shape;

    };

    class CircleButton : public Button
    {
    public:

        CircleButton();

        void setPosition(sf::Vector2f pos);

        [[nodiscard]] sf::FloatRect getGlobalBounds() const override;
        
        void draw(sf::RenderWindow& r_wind) const override;

    private:

        sf::CircleShape m_button_shape;

        sf::RectangleShape m_plus_horizontal, m_plus_vertical;

    };

}

