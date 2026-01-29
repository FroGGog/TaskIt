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

const std::unordered_map<int, Task>& TaskManager::getAllTasks() const
{
    return m_task_storage;
}

void TaskManager::printAllTasks() const
{
    for(const auto& task : m_task_storage)
    {
        std::cout << task.second.getTitle() << ' ' << task.second.getDescription() << " status: " << statusToString(task.second.getStatus()) << '\n';
    }
}
