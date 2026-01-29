#pragma once

#include <vector>

#include "task.h"

class TaskManager
{
public:
    TaskManager() = default;

    void addTask(const Task& add_task);

    [[nodiscard]] Task& getTaskById(int task_id);

private:
    
    std::vector<Task> m_task_storage;

};

