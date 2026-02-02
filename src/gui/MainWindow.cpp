#include "MainWindow.h"


MainWindow::MainWindow(sf::VideoMode vid_mode,const std::string& title)
{
    initWindow(vid_mode, title);
}

void MainWindow::initWindow(sf::VideoMode vid_mode,const std::string& title)
{
    m_window.create(vid_mode, title);
    m_window.setFramerateLimit(60);
}

void MainWindow::sRender()
{
    m_window.clear();

    m_window.display();
}

void MainWindow::sWindowEvents()
{
    while (const std::optional event = m_window.pollEvent())
    {
        if (event->is<sf::Event::Closed>())
            m_window.close();
    }
}

void MainWindow::update()
{
    sWindowEvents();

    sRender();
}

bool MainWindow::isOpen() const
{
    return m_window.isOpen();
}
