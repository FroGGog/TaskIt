#include "task_manager.h"

void TaskManager::addTask(const Task& add_task)
{
    m_task_storage.push_back(add_task);
}

Task &TaskManager::getTaskById(int task_id)
{
    return m_task_storage.at(task_id - 1);
}
