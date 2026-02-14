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

    // Используем TaskItSizes::COLLUMN_SIZE_X вместо getGlobalBounds().size.x
    auto in_pg_collumn = std::make_shared<gui::KanbanCollumn>("IN PROGRESS", materials);
    in_pg_collumn->setPosition(sf::Vector2f{ 
        TaskItSettings::BIG_PADDING * 3 + TaskItSettings::MEDIUM_PADDING + 
        TaskItSizes::COLLUMN_SIZE_X + TaskItSettings::BIG_PADDING, 
        TaskItSettings::BIG_PADDING});
    in_pg_collumn->setIndicatorColor(TaskItColors::INDICATOR_PROGRESS);

    auto done_collumn = std::make_shared<gui::KanbanCollumn>("DONE", materials);
    done_collumn->setPosition(sf::Vector2f{ 
        TaskItSettings::BIG_PADDING * 3 + TaskItSettings::MEDIUM_PADDING + 
        (TaskItSizes::COLLUMN_SIZE_X + TaskItSettings::BIG_PADDING) * 2, 
        TaskItSettings::BIG_PADDING});
    done_collumn->setIndicatorColor(TaskItColors::INDICATOR_DONE);

    Task& task1 = m_manager.addTask("To do", "Task");

    Task& task2 = m_manager.addTask("To do 2", "Task");

    // Для in progress
    Task& task6 = m_manager.addTask("In pg", "Task");
    task6.setStatus(TaskStatus::IN_PROGRESS);

    Task& taskdone = m_manager.addTask("Done", "Task");
    taskdone.setStatus(TaskStatus::DONE);

    Task& taskdone2 = m_manager.addTask("Done", "Task");
    taskdone2.setStatus(TaskStatus::DONE);

    Task& taskdone3 = m_manager.addTask("Done", "Task");
    taskdone3.setStatus(TaskStatus::DONE);

    for(auto& i : m_manager.getAllTasks())
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

    // m_manager.addTask("In progress", "Task");
    // auto inpg_task = std::make_shared<gui::TaskBox>(m_manager.getTaskById(2), 1, materials);
    // inpg_task->setPosition(sf::Vector2f{305.f, 70.f});
    // inpg_task->setColor(TaskItColors::TASK_PROGRESS);

    // m_manager.addTask("Done", "Task");
    // auto done_task = std::make_shared<gui::TaskBox>(m_manager.getTaskById(3), 1, materials);
    // done_task->setPosition(sf::Vector2f{540.f, 70.f});
    // done_task->setColor(TaskItColors::TASK_DONE);


    // gui_elems.push_back(std::move(inpg_task));    
    // gui_elems.push_back(std::move(done_task));    

    gui_elems.push_back(std::move(todo_collumn));
    gui_elems.push_back(std::move(in_pg_collumn));
    gui_elems.push_back(std::move(done_collumn));

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

void MainWindow::sUpdateCollumns()
{
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
