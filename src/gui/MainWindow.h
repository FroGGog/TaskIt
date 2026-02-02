#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <string>

class MainWindow
{
private:

    sf::RenderWindow m_window;

    void initWindow(sf::VideoMode vid_mode,const std::string& title);

    void sRender();
    void sWindowEvents();

public:

    MainWindow(sf::VideoMode vid_mode,const std::string& title);

    void update();
    bool isOpen() const;
};