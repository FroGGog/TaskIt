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
    : m_manager()
{
    initWindow(vid_mode);
    initDialogWindows();
    initGUI();

    m_saved_pos = sf::Vector2f{0.f, 0.f};
    sRefreshBoard();
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

    m_hovered_button = nullptr;
    for(const auto& button : m_buttons)
    {
        if(button->getGlobalBounds().contains(world_pos))
        {
            m_hovered_button = button;
            break;
        }
    }

    m_hovered_text_field = nullptr;
    if(m_dialog_win->isOpen())
    {
        for(const auto& field : m_dialog_win->getAllFields())
        {
            if(field->getGlobalBounds().contains(world_pos))
            {
                m_hovered_text_field = field;
                break;
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

    auto m_add_task_button = std::make_shared<gui::CircleButton>(materials);
    m_add_task_button->setCallbackFunction([&](){m_dialog_win->setIsOpen(true);});
    m_buttons.push_back((m_add_task_button));

    sf::Vector2f button_pos = todo_collumn->getGlobalBounds().position;
    button_pos.x += todo_collumn->getGlobalBounds().size.x - m_add_task_button->getGlobalBounds().size.x - TaskItSettings::MEDIUM_PADDING;
    button_pos.y += todo_collumn->getGlobalBounds().size.y - m_add_task_button->getGlobalBounds().size.y - TaskItSettings::MEDIUM_PADDING;
    m_add_task_button->setPosition(button_pos);

    sRefreshBoard();

    gui_elems.push_back(todo_collumn);
    gui_elems.push_back(in_pg_collumn);
    gui_elems.push_back(done_collumn);

    gui_elems.push_back(m_add_task_button);

}

void MainWindow::initDialogWindows()
{
    m_dialog_win = std::make_unique<DialogWindow>(materials);

    m_dialog_win->setOkayButtonOnClick([&]()
    {
        auto info = m_dialog_win->getAddTaskInfo();
        m_manager.addTask(info.first, info.second);
        m_dialog_win->setIsOpen(true);
        sRefreshBoard();
    });

    m_dialog_win->setCancelButtonOnClick([&](){m_dialog_win->setIsOpen(true); });
    m_dialog_win->setOutLineThickness(1.f);

    for(auto button : m_dialog_win->getAllButton())
    {
        m_buttons.push_back(std::move(button));
    }

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

    m_dialog_win->draw(m_window);

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
            if(keyPressed->scancode == sf::Keyboard::Scancode::D)
            {
                if(m_choosed_task && m_d_key_available && !m_dialog_win->isOpen())
                {
                    m_d_key_available = false;
                    m_manager.deleteTaskById(m_choosed_task->getIndex());
                    m_choosed_task = nullptr;
                    sRefreshBoard();
                }
            }
            if(keyPressed->scancode == sf::Keyboard::Scancode::Backspace)
            {
                if(m_dialog_win->isOpen() && m_choosed_text_field)
                {
                    m_choosed_text_field->handleDelete();
                }
            }
        }
        else if (const auto* keyReleased = event->getIf<sf::Event::KeyReleased>())
        {
            
            if (keyReleased->scancode == sf::Keyboard::Scancode::D)
            {
                m_d_key_available = true;
            }
        }
        else if(const auto* textEntered = event->getIf<sf::Event::TextEntered>())
        {
            if(m_choosed_text_field && m_dialog_win->isOpen())
            {
                m_choosed_text_field->handleInput(textEntered->unicode);
            }
        }


        else if(const auto* mousePressed = event->getIf<sf::Event::MouseButtonPressed>())
        {
            if(mousePressed->button == sf::Mouse::Button::Left)
            {
                if(m_hovered_task)
                {
                    m_choosed_text_field = nullptr;
                    m_choosed_task = m_hovered_task;
                    m_choosed_task->setIsChoosen(true);
                }
                else if(m_hovered_button)
                {
                    std::cout << "Pressed button\n";
                    m_choosed_text_field = nullptr;

                    m_hovered_button->onClick();
                    sf::Vector2u win_size = m_window.getSize();
                    sf::Vector2f center(win_size.x / 2.0f, win_size.y / 2.0f);
                    m_dialog_win->setPosition(center);
                }
                else if(m_hovered_text_field && m_dialog_win->isOpen())
                {
                    std::cout << "Field pressed\n";
                    //reset previous
                    if(m_choosed_text_field)
                    {
                        m_choosed_text_field->handleChoosed();
                    }
                    //handle new one
                    m_choosed_text_field = m_hovered_text_field;
                    m_choosed_text_field->handleChoosed();
                }
                else
                {
                    m_choosed_task = nullptr;
                    m_choosed_text_field = nullptr;
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

void MainWindow::save()
{
    m_manager.saveTasks();
}

DialogWindow::DialogWindow(const UsedMaterials& materials)
{
    m_overlay.setSize(sf::Vector2f(400, 300));
    m_overlay.setFillColor(sf::Color(0, 0, 0, 150));

    m_okay_button = std::make_shared<gui::Button>(materials);
    m_okay_button->setFillColor(TaskItColors::BUTTON_OKEY).setOutLineThickness(1.5f);

    m_cancel_button = std::make_shared<gui::Button>(materials);
    m_cancel_button->setFillColor(TaskItColors::BUTTON_CANCEL).setOutLineThickness(1.5f);
    
    m_title_text_field = std::make_shared<gui::TextField>(materials.global_font);
    m_title_text_field->setSize(sf::Vector2f{300, 25});

    m_descr_text_field = std::make_shared<gui::TextField>(materials.global_font);
    m_descr_text_field->setSize(sf::Vector2f{300, 25});

    m_title = std::make_unique<sf::Text>(*materials.global_font);
    m_title->setString("Title:");
    m_title->setCharacterSize(14);

    m_description = std::make_unique<sf::Text>(*materials.global_font);
    m_description->setString("Description:");
    m_description->setCharacterSize(14);
}

void DialogWindow::handleEvents(const sf::Event &event)
{
    if (!m_isOpen) return;
}

void DialogWindow::update()
{
    if (!m_isOpen) return;
}

void DialogWindow::draw(sf::RenderWindow &window)
{
    if (!m_isOpen) return;

    window.draw(m_overlay);

    m_okay_button->draw(window);
    m_cancel_button->draw(window);

    m_title_text_field->draw(window);
    m_descr_text_field->draw(window);

    window.draw(*m_title);
    window.draw(*m_description);
}

void DialogWindow::setIsOpen(bool open)
{
    if(m_isOpen)
    {
        m_isOpen = !m_isOpen;
        m_title_text_field->resetField();
        m_descr_text_field->resetField();

    }
    else
    {
        m_isOpen = open;
    }
}

void DialogWindow::setPosition(sf::Vector2f pos)
{
    m_overlay.setOrigin(m_overlay.getGeometricCenter());
    m_overlay.setPosition(pos);

    m_okay_button->setSize(sf::Vector2f{80, 25});
    m_cancel_button->setSize(sf::Vector2f{80, 25});

    m_okay_button->setOrigin(m_okay_button->getGlobalBounds().getCenter());
    m_cancel_button->setOrigin(m_cancel_button->getGlobalBounds().getCenter());

    m_okay_button->setPosition(m_overlay.getGlobalBounds().getCenter() + sf::Vector2f{-95.f , 100.f});
    m_cancel_button->setPosition(m_overlay.getGlobalBounds().getCenter() + sf::Vector2f{95.f, 100.f});

    auto okay_button_text = m_okay_button->getText();
    auto cancel_button_text = m_cancel_button->getText();


    static bool text_initialized = false;
    if(!text_initialized)
    {
        okay_button_text->setString("Okay");
        cancel_button_text->setString("Cancel");

        okay_button_text->setCharacterSize(14);
        cancel_button_text->setCharacterSize(14);

        okay_button_text->setOrigin(okay_button_text->getGlobalBounds().getCenter());
        cancel_button_text->setOrigin(cancel_button_text->getGlobalBounds().getCenter());

        text_initialized = true;
    }

    okay_button_text->setPosition(m_okay_button->getGlobalBounds().getCenter());
    cancel_button_text->setPosition(m_cancel_button->getGlobalBounds().getCenter());
    
    m_title_text_field->setOrigin(m_title_text_field->getGlobalBounds().getCenter());
    m_title_text_field->setPosition(m_overlay.getGlobalBounds().getCenter() - sf::Vector2f{0.f, 85.f});

    m_descr_text_field->setOrigin(m_descr_text_field->getGlobalBounds().getCenter());
    m_descr_text_field->setPosition(m_overlay.getGlobalBounds().getCenter() - sf::Vector2f{0.f, 25.f});

    m_title->setPosition(m_title_text_field->getGlobalBounds().position - sf::Vector2f{0.f, 25.f});
    m_description->setPosition(m_descr_text_field->getGlobalBounds().position - sf::Vector2f{0.f, 25.f});
    
}

void DialogWindow::setOutLineThickness(float value)
{
    m_overlay.setOutlineThickness(value);
}

void DialogWindow::setOkayButtonOnClick(std::function<void()> callBack)
{
    m_okay_button->setCallbackFunction(callBack);
}

void DialogWindow::setCancelButtonOnClick(std::function<void()> callBack)
{
    m_cancel_button->setCallbackFunction(callBack);
}

std::vector<std::shared_ptr<gui::Button>> DialogWindow::getAllButton()
{
    return {m_okay_button, m_cancel_button};
}

std::vector<std::shared_ptr<gui::TextField>> DialogWindow::getAllFields()
{
    return {m_title_text_field, m_descr_text_field};
}

std::pair<std::string, std::string> DialogWindow::getAddTaskInfo()
{
    return {m_title_text_field->getText(), m_descr_text_field->getText()};
}

bool DialogWindow::isOpen() const
{
    return m_isOpen;
}
