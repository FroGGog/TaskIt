#include "MainWindow.h"

void MainWindow::resetClickedTask()
{
    for(const auto& collumn : gui_elems)
    {
        auto kanbanCollumn = std::dynamic_pointer_cast<gui::KanbanCollumn>(collumn);
        if(!kanbanCollumn)
        {
            continue;
        }
        for(const auto& task : kanbanCollumn->getAllTasks())
        {
            task->setIsChoosen(false);
        }
    }
}

MainWindow::MainWindow(sf::VideoMode vid_mode)
{
    initWindow(vid_mode);
    initGUI();

    m_saved_pos = sf::Vector2f{0.f, 0.f};
}

void MainWindow::initWindow(sf::VideoMode vid_mode)
{
    m_window.create(vid_mode, TaskItSettings::WIN_TITLE);
    m_window.setFramerateLimit(TaskItSettings::FPS);
}

void MainWindow::sRefreshBoard()
{
    todo_collumn->getAllTasks().clear();
    in_pg_collumn->getAllTasks().clear();
    done_collumn->getAllTasks().clear();

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
    if(m_is_long_mouse_press)
    {
        return;
    }

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

void MainWindow::sMoveTasks()
{
    if(m_hovered_task && m_is_long_mouse_press)
    {
        if(m_saved_pos.x == 0 && m_saved_pos.y == 0)
        {
            m_saved_pos = m_hovered_task->getGlobalBounds().position;
        }
        if(!m_hovered_task->isChoosen())
        {
            m_hovered_task->setIsChoosen(true);
        }
        sf::Vector2i mouse_pos = sf::Mouse::getPosition(m_window);
        sf::Vector2f world_pos = m_window.mapPixelToCoords(mouse_pos);

        m_hovered_task->setPosition(world_pos);
    }
}

void MainWindow::sMoveTaskToCollumn()
{
    if(!m_hovered_task || !m_choosed_task)
    {
        return;
    }
    int task_index = m_choosed_task->getIndex();
    bool moved = false;

    for(const auto& collumn : gui_elems)
    {
        auto kanbanCollumn = std::dynamic_pointer_cast<gui::KanbanCollumn>(collumn);
        if(!kanbanCollumn)
        {
            continue;
        }   
        if((m_hovered_task->getGlobalBounds().findIntersection(kanbanCollumn->getGlobalBounds())).has_value()
            && !kanbanCollumn->taskInColumn(m_choosed_task->getIndex()))
        {
            m_manager.getTaskById(task_index).setStatus(kanbanCollumn->getCollumnTaskStatus());
            moved = true;
            sRefreshBoard();
            std::cout << "Moved to pg collumn\n";
        }
    }
    if(!moved)
    {
        m_hovered_task->setPosition(m_saved_pos);
    }
        m_saved_pos = sf::Vector2f{0.f, 0.f};

}

void MainWindow::initGUI()
{
    todo_collumn = std::make_shared<gui::KanbanCollumn>("TO DO", materials);
    todo_collumn->setPosition(sf::Vector2f{TaskItSettings::BIG_PADDING * 3 + TaskItSettings::MEDIUM_PADDING,
         TaskItSettings::BIG_PADDING});
    todo_collumn->setIndicatorColor(TaskItColors::INDICATOR_TODO);
    todo_collumn->setCollumnTaskState(TaskStatus::TO_DO);

    // Используем TaskItSizes::COLLUMN_SIZE_X вместо getGlobalBounds().size.x
    in_pg_collumn = std::make_shared<gui::KanbanCollumn>("IN PROGRESS", materials);
    in_pg_collumn->setPosition(sf::Vector2f{ 
        TaskItSettings::BIG_PADDING * 3 + TaskItSettings::MEDIUM_PADDING + 
        TaskItSizes::COLLUMN_SIZE_X + TaskItSettings::BIG_PADDING, 
        TaskItSettings::BIG_PADDING});
    in_pg_collumn->setIndicatorColor(TaskItColors::INDICATOR_PROGRESS);
    in_pg_collumn->setCollumnTaskState(TaskStatus::IN_PROGRESS);

    done_collumn = std::make_shared<gui::KanbanCollumn>("DONE", materials);
    done_collumn->setPosition(sf::Vector2f{ 
        TaskItSettings::BIG_PADDING * 3 + TaskItSettings::MEDIUM_PADDING + 
        (TaskItSizes::COLLUMN_SIZE_X + TaskItSettings::BIG_PADDING) * 2, 
        TaskItSettings::BIG_PADDING});
    done_collumn->setIndicatorColor(TaskItColors::INDICATOR_DONE);
    done_collumn->setCollumnTaskState(TaskStatus::DONE);

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
    todo_collumn->drawTasks(m_window);
    in_pg_collumn->drawTasks(m_window);
    done_collumn->drawTasks(m_window);

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
            else if(keyPressed->scancode == sf::Keyboard::Scancode::D)
            {
                if(m_choosed_task && m_d_key_available)
                {
                    m_d_key_available = false;
                    m_manager.deleteTaskById(m_choosed_task->getIndex());
                    m_choosed_task = nullptr;
                    sRefreshBoard();
                }
                
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
            else if (keyReleased->scancode == sf::Keyboard::Scancode::D)
            {
                m_d_key_available = true;
            }
        }
        else if(const auto* mousePressed = event->getIf<sf::Event::MouseButtonPressed>())
        {
            if(mousePressed->button == sf::Mouse::Button::Left)
            {
                if(m_hovered_task)
                {
                    m_choosed_task = m_hovered_task;
                    m_choosed_task->setIsChoosen(true);
                }
                m_is_mouse_pressed = true;
                m_long_press_clock.restart();
                m_is_long_mouse_press = false;
            }
        }
        else if(const auto* mouseReleased = event->getIf<sf::Event::MouseButtonReleased>())
        {
            if(mouseReleased->button == sf::Mouse::Button::Left)
            {
                m_is_mouse_pressed = false;
                // check if just click
                if(m_long_press_clock.getElapsedTime().asSeconds() < TaskItSettings::LONG_PRESS_DURATION && !m_is_long_mouse_press)
                {
                    resetClickedTask();
                    if(m_hovered_task)
                    {
                        m_hovered_task->setIsChoosen(true);
                    }
                }
                // release after long press
                if(m_is_long_mouse_press)
                {
                    m_is_long_mouse_press = false;
                    if(m_hovered_task)
                    {
                        sMoveTaskToCollumn();
                        m_hovered_task->setIsChoosen(false);
                    }
                    std::cout << "Press ended after: " << m_long_press_clock.getElapsedTime().asSeconds() << '\n';
                }


            }
        }
    }
}


void MainWindow::update()
{
    sCheckHover();

    sWindowEvents();

    if(m_is_mouse_pressed && !m_is_long_mouse_press)
    {
        if(m_long_press_clock.getElapsedTime().asSeconds() >= TaskItSettings::LONG_PRESS_DURATION)
        {
            m_is_long_mouse_press = true;

            std::cout << "Long clicked mouse: " << m_long_press_clock.getElapsedTime().asSeconds() << '\n';
        }
    }

    sMoveTasks();

    sRender();
}

bool MainWindow::isOpen() const
{
    return m_window.isOpen();
}
