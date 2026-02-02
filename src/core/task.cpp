#include "task.h"

std::optional<std::string> statusToString(TaskStatus status)
{
    switch (status)
    {
    case TaskStatus::TO_DO:
        return "To Do";
    case TaskStatus::IN_PROGRESS:
        return "In Progress";
    case TaskStatus::DONE:
        return "Done";
    default:
        return std::nullopt;
    }
}

std::optional<TaskStatus> stringToStatus(std::string_view str_status)
{
    if(str_status == "todo")
    {
        return TaskStatus::TO_DO;
    }
    else if (str_status == "progress")
    {
        return TaskStatus::IN_PROGRESS;
    }
    else if (str_status == "done")
    {
        return TaskStatus::DONE;
    }
    else
    {
        return std::nullopt;
    }
}

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
