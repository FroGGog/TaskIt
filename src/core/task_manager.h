#pragma once

#include <unordered_map>
#include <iostream>

#include "task.h"

class TaskManager
{
public:
    TaskManager() = default;

    void addTask(const std::string& title, const std::string& description = " ");

    //getters
    [[nodiscard]] Task& getTaskById(int task_id);
    [[nodiscard]] const std::unordered_map<int, Task>&  getAllTasks() const;

    void printAllTasks() const;

private:
    
    std::unordered_map<int, Task> m_task_storage;

    int m_next_task_id = 1;

};

