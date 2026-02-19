#include "GUIElements.h"

UsedMaterials::UsedMaterials()
    : global_font(std::make_shared<sf::Font>())
{
    if(!global_font->openFromFile("BrownieStencil-8O8MJ.ttf"))
    {
        std::cerr << "ERROR::OPENFROMFILE::FONT global_font\n";
    }
}

// Support functions
void gui::TaskBox::updateTextPositions()
{
    sf::Vector2f back_pos = m_background_shape.getGlobalBounds().position;
    float line_spacing = 7.f;

    m_text_title->setPosition(sf::Vector2f{back_pos.x + TaskItSettings::MEDIUM_PADDING, back_pos.y + line_spacing});

    m_text_descr->setPosition(sf::Vector2f{back_pos.x + TaskItSettings::MEDIUM_PADDING,
         m_text_title->getPosition().y + m_text_title->getCharacterSize() + line_spacing});

    m_text_create_time->setPosition(sf::Vector2f{ back_pos.x + TaskItSettings::MEDIUM_PADDING,
         m_text_descr->getPosition().y  + m_text_descr->getCharacterSize() + line_spacing});
}

gui::TaskBox::TaskBox(const Task &task, const int task_index, const UsedMaterials &materials)
    : m_task_index(task_index), m_current_status(task.getStatus())
{
    m_background_shape.setSize(sf::Vector2f{TaskItSizes::TASK_SIZE_X, TaskItSizes::TASK_SIZE_Y});
    m_text_title = std::make_unique<sf::Text>(*materials.global_font,
        task.getTitle(), 16);
    m_text_title->setFillColor(TaskItColors::TASK_TEXT);

    m_text_descr = std::make_unique<sf::Text>(*materials.global_font,
        task.getDescription(), 15);
    m_text_descr->setFillColor(TaskItColors::TASK_TEXT);
    
    m_text_create_time = std::make_unique<sf::Text>(*materials.global_font,
        task.getTimeCreate(), 15);
    m_text_create_time->setFillColor(TaskItColors::TASK_TEXT);

    updateTextPositions();
}

void gui::TaskBox::draw(sf::RenderWindow& r_win) const
{
    r_win.draw(m_background_shape);
    r_win.draw(*m_text_title);
    r_win.draw(*m_text_descr);
    r_win.draw(*m_text_create_time);
}

void gui::TaskBox::setPosition(sf::Vector2f pos)
{
    m_background_shape.setPosition(pos);
    updateTextPositions();
}

void gui::TaskBox::setColor(sf::Color color)
{
    m_background_shape.setFillColor(color);
}

void gui::TaskBox::setIsChoosen(bool is_choosen)
{
    if(!m_is_choosen && is_choosen)
    {
        m_background_shape.setOutlineThickness(3.f);
        m_background_shape.setOutlineColor(TaskItColors::TASK_HOVER_OUTLINE);
        m_is_choosen = true;
    }
    else if((m_is_choosen && is_choosen) || !is_choosen)
    {
        m_background_shape.setOutlineThickness(0.f);
        m_is_choosen = false;
    }

}

sf::FloatRect gui::TaskBox::getGlobalBounds() const
{
    return m_background_shape.getGlobalBounds();
}

int gui::TaskBox::getIndex() const
{
    return m_task_index;
}

TaskStatus gui::TaskBox::getStatus() const
{
    return m_current_status;
}

bool gui::TaskBox::isChoosen() const
{
    return m_is_choosen;
}


void gui::KanbanCollumn::setUpLayout()
{
    m_layout_box.setPosition(sf::Vector2f{m_title_box.getGlobalBounds().position.x +
        TaskItSettings::BIG_PADDING,
        m_title_box.getGlobalBounds().position.y +
        m_title_box.getGlobalBounds().size.y +
        TaskItSettings::BIG_PADDING});
}

gui::KanbanCollumn::KanbanCollumn(std::string title, const UsedMaterials& materials)
{
    m_body_box.setSize(sf::Vector2f{TaskItSizes::COLLUMN_SIZE_X, TaskItSizes::COLLUMN_SIZE_Y});
    m_body_box.setFillColor(TaskItColors::COLLUMN);

    m_title_box.setSize(sf::Vector2f{TaskItSizes::COLLUMN_SIZE_X, TaskItSizes::COLLUMN_TITLE_SIZE_Y});
    m_title_box.setFillColor(TaskItColors::COLLUMN_HEADER);

    m_indicator_box.setSize(sf::Vector2f{TaskItSizes::COLLUMN_SIZE_X, TaskItSizes::COLLUMN_INDICATOR_SIZE_Y});

    m_header_text = std::make_unique<sf::Text>(*materials.global_font);
    m_header_text->setFont(*materials.global_font);
    m_header_text->setCharacterSize(10);
    m_header_text->setFillColor(TaskItColors::COLLUMN_HEADER_TEXT);
    m_header_text->setString(std::move(title));

    m_layout_box.setSize(sf::Vector2f{TaskItSizes::COLLUMN_SIZE_X - TaskItSettings::MEDIUM_PADDING,
        TaskItSizes::COLLUMN_SIZE_Y - TaskItSettings::MEDIUM_PADDING});
    setUpLayout();

    m_max_task_in_collumn = m_layout_box.getGlobalBounds().size.y / (TaskItSizes::TASK_SIZE_Y + TaskItSettings::BIG_PADDING);
    std::cout << "Max task: " << m_max_task_in_collumn << '\n';
}

void gui::KanbanCollumn::setPosition(sf::Vector2f pos)
{
    m_body_box.setPosition(pos);
    m_title_box.setPosition(pos);
    m_indicator_box.setPosition(pos);
    setUpLayout();

    sf::Vector2f boxCenter = m_title_box.getGlobalBounds().getCenter();
    sf::FloatRect textBounds = m_header_text->getGlobalBounds();

    m_header_text->setPosition(sf::Vector2f{ boxCenter.x - textBounds.size.x / 2.0f,  
        boxCenter.y - textBounds.size.y / 2.0f });
}

void gui::KanbanCollumn::setIndicatorColor(sf::Color color)
{
    m_indicator_box.setFillColor(color);
}

void gui::KanbanCollumn::setHeaderTitle(std::string str)
{
    m_header_text->setString(std::move(str));
}

void gui::KanbanCollumn::setCollumnTaskState(TaskStatus status)
{
    m_collumn_task_state = status;
}

sf::FloatRect gui::KanbanCollumn::getGlobalBounds() const
{
    return m_body_box.getGlobalBounds();
}

TaskStatus gui::KanbanCollumn::getCollumnTaskStatus() const
{
    return m_collumn_task_state;
}

void gui::KanbanCollumn::draw(sf::RenderWindow &r_wind) const
{
    r_wind.draw(m_body_box);
    r_wind.draw(m_title_box);
    r_wind.draw(m_indicator_box);
    r_wind.draw(*m_header_text);
}

void gui::KanbanCollumn::drawTasks(sf::RenderWindow &r_wind) const
{
    for(const auto& task : m_task_box_in_collumn)
    {
        if (task) {  
            task->draw(r_wind);
        }
    }
}

void gui::KanbanCollumn::addTaskToLayout(const Task &task, const int task_index, const UsedMaterials &materials)
{
    auto tempBox = std::make_shared<gui::TaskBox>(task, task_index, materials);
    switch (task.getStatus())
    {
    case TaskStatus::TO_DO:
        tempBox->setColor(TaskItColors::TASK_TODO);
        break;
    case TaskStatus::IN_PROGRESS:
        tempBox->setColor(TaskItColors::TASK_PROGRESS);
        break;
    case TaskStatus::DONE:
        tempBox->setColor(TaskItColors::TASK_DONE);
        break;
    default:
        break;
    }

    // add first task to collum
    if(m_task_box_in_collumn.empty())
    {
        tempBox->setPosition(m_layout_box.getGlobalBounds().position);
    }
    else
    {
        auto back_task = m_task_box_in_collumn.back();
        tempBox->setPosition(sf::Vector2f{back_task->getGlobalBounds().position.x,
            back_task->getGlobalBounds().position.y +
            back_task->getGlobalBounds().size.y +
            TaskItSettings::BIG_PADDING});
    }
    m_task_box_in_collumn.push_back(tempBox);
}

bool gui::KanbanCollumn::taskInColumn(int task_id)
{
    for(const auto& task : m_task_box_in_collumn)
    {
        if(task->getIndex() == task_id)
        {
            return true;
        }
    }
    return false;
}

std::vector<std::shared_ptr<gui::TaskBox>> &gui::KanbanCollumn::getAllTasks()
{
    return m_task_box_in_collumn;
}

void gui::KanbanCollumn::clearCollumn()
{
    m_task_box_in_collumn.clear();
}

gui::Button::Button(std::string button_text, const UsedMaterials &materials)
{
    m_button_shape.setRadius(10.f);
    m_button_shape.setFillColor(sf::Color{100, 100, 220, 255});

    m_plus_horizontal.setSize(sf::Vector2f{15, 3});
    m_plus_vertical.setSize(sf::Vector2f{3, 15});
    m_plus_horizontal.setOrigin(m_plus_horizontal.getGeometricCenter());
    m_plus_vertical.setOrigin(m_plus_vertical.getGeometricCenter());
}

void gui::Button::setPosition(sf::Vector2f pos)
{
    m_button_shape.setPosition(pos);

    sf::Vector2f center = m_button_shape.getPosition();
    m_plus_horizontal.setPosition(m_button_shape.getGlobalBounds().getCenter());
    m_plus_vertical.setPosition(m_button_shape.getGlobalBounds().getCenter());
}

sf::FloatRect gui::Button::getGlobalBounds() const
{
    return m_button_shape.getGlobalBounds();
}

void gui::Button::draw(sf::RenderWindow &r_wind) const
{
    r_wind.draw(m_button_shape);

    r_wind.draw(m_plus_horizontal);
    r_wind.draw(m_plus_vertical);
}
