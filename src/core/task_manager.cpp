#include "task_manager.h"

void TaskManager::addTask(const std::string& title, const std::string& description)
{
    m_task_storage.insert({m_next_task_id, Task(title, description)});
    m_next_task_id++;
}

Task &TaskManager::getTaskById(int task_id)
{
    return m_task_storage.at(task_id);
}
