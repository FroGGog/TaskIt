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

    TaskManager m_manager;

    UsedMaterials materials;

    std::vector<std::shared_ptr<gui::GuiElement>> gui_elems;

public:

    MainWindow(sf::VideoMode vid_mode);

    void update();
    bool isOpen() const;
};