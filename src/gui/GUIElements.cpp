#include "GUIElements.h"

UsedMaterials::UsedMaterials()
    : task_box_font(std::make_shared<sf::Font>())
{
    if(!task_box_font->openFromFile("BrownieStencil-8O8MJ.ttf"))
    {
        std::cerr << "ERROR::OPENFROMFILE::FONT task_box_font\n";
    }
}

gui::TaskBox::TaskBox(const Task &task, const int task_index, const UsedMaterials &materials)
    : m_task_index(task_index)
{
    m_background_shape.setSize(sf::Vector2f{187.f, 90.f});
    m_background_shape.setFillColor(sf::Color{static_cast<std::uint8_t>(51),
         static_cast<std::uint8_t>(153), static_cast<std::uint8_t>(255), static_cast<std::uint8_t>(50)});
    m_text_title = std::make_unique<sf::Text>(*materials.task_box_font,
        task.getTitle(), 16);

    sf::Vector2f back_pos = m_background_shape.getGlobalBounds().position;
    float line_spacing = 7.f;

    m_text_title->setPosition(sf::Vector2f{back_pos.x + 10.f, back_pos.y + line_spacing});

    m_text_descr = std::make_unique<sf::Text>(*materials.task_box_font,
        task.getDescription(), 15);
    m_text_descr->setPosition(sf::Vector2f{back_pos.x + 10.f, m_text_title->getPosition().y + m_text_title->getCharacterSize() + line_spacing});

    m_text_create_time = std::make_unique<sf::Text>(*materials.task_box_font,
        getCurrentTime(), 15);
    m_text_create_time->setPosition(sf::Vector2f{ back_pos.x + 10.f, m_text_descr->getPosition().y  + m_text_descr->getCharacterSize() + line_spacing});
}

void gui::TaskBox::draw(sf::RenderWindow& r_win)
{
    r_win.draw(m_background_shape);
    r_win.draw(*m_text_title);
    r_win.draw(*m_text_descr);
    r_win.draw(*m_text_create_time);
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