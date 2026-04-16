#include "task.h"

std::optional<std::string> statusToString(TaskStatus status)
{
    switch (status)
    {
    case TaskStatus::TO_DO:
        return "todo";
    case TaskStatus::IN_PROGRESS:
        return "progress";
    case TaskStatus::DONE:
        return "done";
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
    {
        setTimeCreate();
    }

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

std::string Task::getTimeCreate() const
{
    std::ostringstream oss;
    oss << std::put_time(&m_create_time, "%d-%m-%Y %H:%M:%S");
    return oss.str();
}

void Task::setTimeCreate()
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
}