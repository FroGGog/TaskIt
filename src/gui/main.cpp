#include <SFML/Window.hpp>
#include "MainWindow.h"


int main()
{
    MainWindow win{TaskItSettings::WIN_RESOLUTION};

    while(win.isOpen())
    {
        win.update();
    }
    std::cout << "Saving tasks..." << std::endl;
    win.save();
    std::cout << "Save completed" << std::endl;
}