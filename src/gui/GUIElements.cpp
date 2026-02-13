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
    : m_task_index(task_index)
{
    m_background_shape.setSize(sf::Vector2f{187.f, 90.f});
    m_text_title = std::make_unique<sf::Text>(*materials.global_font,
        task.getTitle(), 16);
    m_text_title->setFillColor(TaskItColors::TASK_TEXT);

    m_text_descr = std::make_unique<sf::Text>(*materials.global_font,
        task.getDescription(), 15);
    m_text_descr->setFillColor(TaskItColors::TASK_TEXT);
    
    m_text_create_time = std::make_unique<sf::Text>(*materials.global_font,
        getCurrentTime(), 15);
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

std::string gui::TaskBox::getCurrentTime()
{
    auto now = std::chrono::system_clock::now();
    std::time_t now_time_t = std::chrono::system_clock::to_time_t(now);

    #if defined(_WIN32) || defined(_MSC_VER)
        // Windows: localtime_s(tm*, const time_t*)
        localtime_s(&m_create_time, &now_time_t);
    #else
        // POSIX: localtime_r(const time_t*, tm*)
        localtime_r(&now_time_t, &m_create_time);
    #endif

    std::ostringstream oss;
    oss << std::put_time(&m_create_time, "%d-%m-%Y %H:%M:%S");
    return oss.str();
    
}


gui::KanbanCollumn::KanbanCollumn(std::string title, const UsedMaterials& materials)
{
    m_body_box.setSize(sf::Vector2f{220, 550});
    m_body_box.setFillColor(TaskItColors::COLLUMN);

    m_title_box.setSize(sf::Vector2f{220, 30});
    m_title_box.setFillColor(TaskItColors::COLLUMN_HEADER);

    m_indicator_box.setSize(sf::Vector2f{220, 5});

    m_header_text = std::make_unique<sf::Text>(*materials.global_font);
    m_header_text->setFont(*materials.global_font);
    m_header_text->setCharacterSize(10);
    m_header_text->setFillColor(TaskItColors::COLLUMN_HEADER_TEXT);
    m_header_text->setString(std::move(title));
}

void gui::KanbanCollumn::setPosition(sf::Vector2f pos)
{
    m_body_box.setPosition(pos);
    m_title_box.setPosition(pos);
    m_indicator_box.setPosition(pos);

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

sf::FloatRect gui::KanbanCollumn::getGlobalBounds() const
{
    return m_body_box.getGlobalBounds();
}

void gui::KanbanCollumn::draw(sf::RenderWindow &r_wind) const
{
    r_wind.draw(m_body_box);
    r_wind.draw(m_title_box);
    r_wind.draw(m_indicator_box);
    r_wind.draw(*m_header_text);
}
