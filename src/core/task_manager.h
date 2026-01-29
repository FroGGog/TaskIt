#pragma once

#include <unordered_map>

#include "task.h"

class TaskManager
{
public:
    TaskManager() = default;

    void addTask(const std::string& title, const std::string& description);

    [[nodiscard]] Task& getTaskById(int task_id);

private:
    
    std::unordered_map<int, Task> m_task_storage;

};

