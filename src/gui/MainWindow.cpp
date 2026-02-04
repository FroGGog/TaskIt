#include "MainWindow.h"


MainWindow::MainWindow(sf::VideoMode vid_mode,const std::string& title)
{
    initWindow(vid_mode, title);
    initGUI();
}

void MainWindow::initWindow(sf::VideoMode vid_mode,const std::string& title)
{
    m_window.create(vid_mode, title);
    m_window.setFramerateLimit(60);
}

void MainWindow::initGUI()
{
    m_manager.addTask("Title", "Descr");
    temp_box = std::make_shared<gui::TaskBox>(m_manager.getTaskById(1), 1, materials);
}

void MainWindow::sRender()
{
    m_window.clear();

    temp_box->draw(m_window);

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
