#include <SFML/Window.hpp>
#include "MainWindow.h"


int main()
{
    MainWindow win{TaskItSettings::WIN_RESOLUTION};

    while(win.isOpen())
    {
        win.update();
    }
}