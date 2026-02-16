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

void MainWindow::sRefreshBoard()
{
    for(auto& i : m_manager.getAllTasks())
    {
        if(!todo_collumn->taskInColumn(i.first) && !in_pg_collumn->taskInColumn(i.first) && !done_collumn->taskInColumn(i.first))
        {
            switch (i.second.getStatus())
            {
            case TaskStatus::TO_DO:
                todo_collumn->addTaskToLayout(i.second, i.first, materials);
                break;
            case TaskStatus::IN_PROGRESS:
                in_pg_collumn->addTaskToLayout(i.second, i.first, materials);
                break;
            case TaskStatus::DONE:
                done_collumn->addTaskToLayout(i.second, i.first, materials);
                break;
            default:
                break;
            }
        }

    }
}

void MainWindow::sCheckHover()
{
    sf::Vector2i mouse_pos = sf::Mouse::getPosition(m_window);
    sf::Vector2f world_pos = m_window.mapPixelToCoords(mouse_pos);

    if(m_hovered_task)
    {
        switch (m_hovered_task->getStatus())
        {
            case TaskStatus::TO_DO:
                m_hovered_task->setColor(TaskItColors::TASK_TODO);
                break;
            case TaskStatus::IN_PROGRESS:
                m_hovered_task->setColor(TaskItColors::TASK_PROGRESS);
                break;
            case TaskStatus::DONE:
                m_hovered_task->setColor(TaskItColors::TASK_DONE);
                break;
            default:
                break;
        }
        m_hovered_task = nullptr;
    }

    for(const auto& collumn : gui_elems)
    {
        auto kanbanCollumn = std::dynamic_pointer_cast<gui::KanbanCollumn>(collumn);
        if(!kanbanCollumn)
        {
            continue;
        }

        for(const auto& task : kanbanCollumn->getAllTasks())
        {
            if(task->getGlobalBounds().contains(world_pos))
            {
                switch (task->getStatus())
                {
                    case TaskStatus::TO_DO:
                        task->setColor(TaskItColors::TASK_TODO_HOVER);
                        break;
                    case TaskStatus::IN_PROGRESS:
                        task->setColor(TaskItColors::TASK_PROGRESS_HOVER);
                        break;
                    case TaskStatus::DONE:
                        task->setColor(TaskItColors::TASK_DONE_HOVER);
                        break;
                    default:
                        break;
                }
                m_hovered_task = task;
            }
        }
    }
}

void MainWindow::initGUI()
{
    todo_collumn = std::make_shared<gui::KanbanCollumn>("TO DO", materials);
    todo_collumn->setPosition(sf::Vector2f{TaskItSettings::BIG_PADDING * 3 + TaskItSettings::MEDIUM_PADDING,
         TaskItSettings::BIG_PADDING});
    todo_collumn->setIndicatorColor(TaskItColors::INDICATOR_TODO);

    // Используем TaskItSizes::COLLUMN_SIZE_X вместо getGlobalBounds().size.x
    in_pg_collumn = std::make_shared<gui::KanbanCollumn>("IN PROGRESS", materials);
    in_pg_collumn->setPosition(sf::Vector2f{ 
        TaskItSettings::BIG_PADDING * 3 + TaskItSettings::MEDIUM_PADDING + 
        TaskItSizes::COLLUMN_SIZE_X + TaskItSettings::BIG_PADDING, 
        TaskItSettings::BIG_PADDING});
    in_pg_collumn->setIndicatorColor(TaskItColors::INDICATOR_PROGRESS);

    done_collumn = std::make_shared<gui::KanbanCollumn>("DONE", materials);
    done_collumn->setPosition(sf::Vector2f{ 
        TaskItSettings::BIG_PADDING * 3 + TaskItSettings::MEDIUM_PADDING + 
        (TaskItSizes::COLLUMN_SIZE_X + TaskItSettings::BIG_PADDING) * 2, 
        TaskItSettings::BIG_PADDING});
    done_collumn->setIndicatorColor(TaskItColors::INDICATOR_DONE);

    sRefreshBoard();

    gui_elems.push_back(todo_collumn);
    gui_elems.push_back(in_pg_collumn);
    gui_elems.push_back(done_collumn);

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
        else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
        {
            if (keyPressed->scancode == sf::Keyboard::Scancode::T && m_t_key_available)
            {
                m_t_key_available = false;
                Task& tempTask = m_manager.addTask("Test", "Task");
                tempTask.setStatus(TaskStatus::TO_DO);
                sRefreshBoard();
            }
            else if(keyPressed->scancode == sf::Keyboard::Scancode::Y && m_y_key_available)
            {
                m_y_key_available = false;
                Task& tempTask = m_manager.addTask("Test", "Task");
                tempTask.setStatus(TaskStatus::IN_PROGRESS);
                sRefreshBoard();
            }
            else if(keyPressed->scancode == sf::Keyboard::Scancode::U && m_u_key_available)
            {
                m_u_key_available = false;
                Task& tempTask = m_manager.addTask("Test", "Task");
                tempTask.setStatus(TaskStatus::DONE);
                sRefreshBoard();
            }
            
        }
        else if (const auto* keyReleased = event->getIf<sf::Event::KeyReleased>())
        {
            if (keyReleased->scancode == sf::Keyboard::Scancode::T)
            {
                m_t_key_available = true;
            }
            else if (keyReleased->scancode == sf::Keyboard::Scancode::Y)
            {
                m_y_key_available = true;
            }
            else if (keyReleased->scancode == sf::Keyboard::Scancode::U)
            {
                m_u_key_available = true;
            }
        }
    }
}


void MainWindow::update()
{
    sWindowEvents();

    sCheckHover();

    sRender();
}

bool MainWindow::isOpen() const
{
    return m_window.isOpen();
}
