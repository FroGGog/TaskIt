#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <string>

#include "task_manager.h"
#include "GUIElements.h"

class MainWindow
{
private:

    sf::RenderWindow m_window;

    void initWindow(sf::VideoMode vid_mode,const std::string& title);
    void initGUI();


    void sRender();
    void sWindowEvents();

    TaskManager m_manager;

    UsedMaterials materials;
    std::shared_ptr<gui::TaskBox> temp_box;

public:

    MainWindow(sf::VideoMode vid_mode,const std::string& title);

    void update();
    bool isOpen() const;
};