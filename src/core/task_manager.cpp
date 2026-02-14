#include "task_manager.h"

Task& TaskManager::addTask(const std::string& title, const std::string& description)
{
    int newID = m_next_task_id++;
    auto [it, inserted] = m_task_storage.emplace(newID, Task(title, description));
    return it->second;
}

Task &TaskManager::getTaskById(int task_id)
{
    return m_task_storage.at(task_id);
}

std::unordered_map<int, Task>& TaskManager::getAllTasks()
{
    return m_task_storage;
}

void TaskManager::changeTaskStatus(int task_id, TaskStatus status)
{
    m_task_storage.at(task_id).setStatus(status);
}

void TaskManager::printAllTasks() const
{
    for(const auto& task : m_task_storage)
    {
        std::cout << "ID: " << task.first << " | Title: " << task.second.getTitle() << " | Description: " << task.second.getDescription() 
                  << " | Status: " << statusToString(task.second.getStatus()).value() << '\n';
    }
}

void TaskManager::printTaskById(int task_id) const
{
    try
    {
        const auto& task = m_task_storage.at(task_id);
        std::cout << "ID: " <<  task_id << " | Title: " << task.getTitle() << " | Description: " << task.getDescription()
                  << " | Status: " << statusToString(task.getStatus()).value() << "\n";
    }
    catch(const std::out_of_range& e)
    {
        std::cerr << "taskit> No task with such ID\n";
    }
}
