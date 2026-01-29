#include "task_manager.h"

void TaskManager::addTask(const std::string& title, const std::string& description)
{
    Task temp_task{title, description};
    m_task_storage.insert({temp_task.getId(), temp_task});
}

Task &TaskManager::getTaskById(int task_id)
{
    return m_task_storage.at(task_id);
}
