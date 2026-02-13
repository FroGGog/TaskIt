#include "MainWindow.h"


MainWindow::MainWindow(sf::VideoMode vid_mode)
{
    initWindow(vid_mode);
    initGUI();
}

void MainWindow::initWindow(sf::VideoMode vid_mode)
{
    m_window.create(vid_mode, TaskItSettings::WIN_TITLE);
    m_window.setFramerateLimit(TaskItSettings::FPS);
}

void MainWindow::initGUI()
{
    auto todo_collumn = std::make_shared<gui::KanbanCollumn>("TO DO", materials);
    todo_collumn->setPosition(sf::Vector2f{TaskItSettings::BIG_PADDING * 3 + TaskItSettings::MEDIUM_PADDING,
         TaskItSettings::BIG_PADDING});
    todo_collumn->setIndicatorColor(TaskItColors::INDICATOR_TODO);

    auto in_pg_collumn = std::make_shared<gui::KanbanCollumn>("IN PROGRESS", materials);
    in_pg_collumn->setPosition(sf::Vector2f{ todo_collumn->getGlobalBounds().position.x +
        todo_collumn->getGlobalBounds().size.x
        + TaskItSettings::BIG_PADDING, TaskItSettings::BIG_PADDING});
    in_pg_collumn->setIndicatorColor(TaskItColors::INDICATOR_PROGRESS);
    

    auto done_collumn = std::make_shared<gui::KanbanCollumn>("DONE", materials);
    done_collumn->setPosition(sf::Vector2f{ in_pg_collumn->getGlobalBounds().position.x +
        in_pg_collumn->getGlobalBounds().size.x
        + TaskItSettings::BIG_PADDING, TaskItSettings::BIG_PADDING});
    done_collumn->setIndicatorColor(TaskItColors::INDICATOR_DONE);

    gui_elems.push_back(std::move(todo_collumn));
    gui_elems.push_back(std::move(in_pg_collumn));
    gui_elems.push_back(std::move(done_collumn));


    m_manager.addTask("Title", "Descr");
    auto todo_task = std::make_shared<gui::TaskBox>(m_manager.getTaskById(1), 1, materials);
    todo_task->setPosition(sf::Vector2f{70.f, 70.f});
    todo_task->setColor(TaskItColors::TASK_TODO);

    auto inpg_task = std::make_shared<gui::TaskBox>(m_manager.getTaskById(1), 1, materials);
    inpg_task->setPosition(sf::Vector2f{305.f, 70.f});
    inpg_task->setColor(TaskItColors::TASK_PROGRESS);

    auto done_task = std::make_shared<gui::TaskBox>(m_manager.getTaskById(1), 1, materials);
    done_task->setPosition(sf::Vector2f{540.f, 70.f});
    done_task->setColor(TaskItColors::TASK_DONE);


    gui_elems.push_back(std::move(todo_task));
    gui_elems.push_back(std::move(inpg_task));    
    gui_elems.push_back(std::move(done_task));    


}

void MainWindow::sRender()
{
    m_window.clear(TaskItColors::WIN_BG);

    for(const auto& i : gui_elems)
    {
        i->draw(m_window);
    }

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
