#include <SFML/Window.hpp>
#include "MainWindow.h"


int main()
{
    MainWindow win{sf::VideoMode({800, 600}), "Test"};

    while(win.isOpen())
    {
        win.update();
    }
}