#include "task.h"


Task::Task(std::string task_title, std::string description)
    : m_task_title(std::move(task_title)),
      m_description(std::move(description))
    {}

// setters
void Task::setTitle(std::string new_title)
{
    m_task_title = std::move(new_title);
}

void Task::setDescription(std::string new_description)
{
    m_description = std::move(new_description);
}

void Task::setStatus(TaskStatus new_status)
{
    m_task_status = new_status;
}

// getters
std::string_view Task::getTitle() const
{
    return m_task_title;
}

std::string_view Task::getDescription() const
{
    return m_description;
}

TaskStatus Task::getStatus() const
{
    return m_task_status;
}
