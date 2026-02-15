#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <string>

#include "task_manager.h"
#include "GUIElements.h"

class MainWindow
{
private:

    sf::RenderWindow m_window;

    void initWindow(sf::VideoMode vid_mode);
    void initGUI();

    void sRender();
    void sWindowEvents();
    void sRefreshBoard();

    TaskManager m_manager;

    UsedMaterials materials;

    std::vector<std::shared_ptr<gui::GuiElement>> gui_elems;

    std::shared_ptr<gui::KanbanCollumn> todo_collumn, in_pg_collumn, done_collumn;

    bool m_t_key_available = true;
    bool m_y_key_available = true;
    bool m_u_key_available = true;
public:

    MainWindow(sf::VideoMode vid_mode);

    void update();
    bool isOpen() const;
};