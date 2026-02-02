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

void TaskManager::changeTaskStatus(int task_id, TaskStatus status)
{
    m_task_storage.at(task_id).setStatus(status);
}

void TaskManager::printAllTasks() const
{
    for(const auto& task : m_task_storage)
    {
        std::cout << "ID: " << task.first << " | Title: " << task.second.getTitle() << " | Description: " << task.second.getDescription() 
                  << " | Status: " << statusToString(task.second.getStatus()) << '\n';
    }
}

void TaskManager::printTaskById(int task_id) const
{
    try
    {
        const auto& task = m_task_storage.at(task_id);
        std::cout << "ID: " <<  task_id << " | Title: " << task.getTitle() << " | Description: " << task.getDescription()
                  << " | Status: " << statusToString(task.getStatus()) << "\n";
    }
    catch(const std::out_of_range& e)
    {
        std::cerr << "taskit> No task with such ID\n";
    }
}
