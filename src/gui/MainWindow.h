#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <string>

#include "task_manager.h"
#include "GUIElements.h"

class DialogWindow
{
public:

    DialogWindow();

    void handleEvents(const sf::Event& event);

    void update();
    void draw(sf::RenderWindow& window);

    void setIsOpen(bool open);
    void setPosition(sf::Vector2f pos);

    bool isOpen() const;

private:

    bool m_isOpen = false;

    sf::RectangleShape m_overlay;

    std::unique_ptr<gui::Button> m_okay_button;
    std::unique_ptr<gui::Button> m_cancel_button;
};


class MainWindow
{
private:

    sf::RenderWindow m_window;

    void initWindow(sf::VideoMode vid_mode);
    void initGUI();
    void initDialogWindows();

    void sRender();
    void sWindowEvents();
    void sRefreshBoard();
    void sCheckHover();
    void sMoveTasks();
    void sMoveTaskToCollumn();

    TaskManager m_manager;
    DialogWindow m_dialog_win;

    UsedMaterials materials;

    std::vector<std::shared_ptr<gui::GuiElement>> gui_elems;

    std::shared_ptr<gui::KanbanCollumn> todo_collumn, in_pg_collumn, done_collumn;
    std::shared_ptr<gui::TaskBox> m_hovered_task;
    std::shared_ptr<gui::TaskBox> m_choosed_task;
    std::shared_ptr<gui::Button> m_hovered_button;

    std::shared_ptr<gui::CircleButton> m_add_task_button;

    bool m_d_key_available = true;

    bool m_is_mouse_pressed = false;
    bool m_is_long_mouse_press = false;
    sf::Clock m_long_press_clock;
    sf::Vector2f m_saved_pos;

    void resetClickedTask();
public:

    explicit MainWindow(sf::VideoMode vid_mode);

    void update();
    bool isOpen() const;
};