#include "task_manager.h"

TaskManager::TaskManager()
{
    std::cout << "Load task manager\n";
    loadTasks();
}

Task &TaskManager::addTask(const std::string &title, const std::string &description)
{
    int newID = m_next_task_id++;
    auto [it, inserted] = m_task_storage.emplace(newID, Task(title, description));
    return it->second;
}

Task &TaskManager::getTaskById(int task_id)
{
    return m_task_storage.at(task_id);
}

void TaskManager::deleteTaskById(int task_id)
{
    m_task_storage.erase(task_id);
}

std::unordered_map<int, Task>& TaskManager::getAllTasks()
{
    return m_task_storage;
}

void TaskManager::changeTaskStatus(int task_id, TaskStatus status)
{
    m_task_storage.at(task_id).setStatus(status);
}

void TaskManager::loadTasks()
{
    namespace fs = std::filesystem;
    using json = nlohmann::json;

    fs::path jsonPath = "src/data/save.json";

    if(fs::exists(jsonPath))
    {
        std::ifstream f(jsonPath.c_str());   
        json jsonArray;
        f >> jsonArray;

        for (const auto& taskJson : jsonArray) 
        {
            std::string title = taskJson["title"];
            std::string descr = taskJson["description"];
            std::string status = taskJson["status"];

            Task& newTask = addTask(title, descr);

            newTask.setStatus(stringToStatus(status).value());

            std::cout << "Added task: " + title + " " + descr + " " + status << '\n';
            
        }
    }
    else
    {
        std::cout << "First start, no save.json file\n";
    }
}

void TaskManager::saveTasks()
{
    namespace fs = std::filesystem;
    nlohmann::json jsonArray = nlohmann::json::array();
    fs::path jsonPath = "src/data/save.json";

    for(const auto& [id, task] : m_task_storage)
    {
        nlohmann::json taskJson;
        taskJson["title"] = task.getTitle();
        taskJson["description"] = task.getDescription();
        taskJson["status"] = statusToString(task.getStatus()).value_or("todo");
        taskJson["date_create"] = task.getTimeCreate();

        jsonArray.push_back(taskJson);
    }
    std::ofstream ofs(jsonPath);
    ofs << jsonArray.dump(4);
    std::cout << "Saved to file\n";
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
