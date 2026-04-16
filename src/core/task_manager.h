#pragma once

#include <unordered_map>
#include <iostream>
#include <filesystem>
#include <fstream>

#include "task.h"
#include "../lib/json.hpp"

class TaskManager
{
public:
    TaskManager();

    Task& addTask(const std::string& title, const std::string& description = " ");

    //getters
    [[nodiscard]] Task& getTaskById(int task_id);
    [[nodiscard]] std::unordered_map<int, Task>&  getAllTasks();

    void deleteTaskById(int task_id);

    //setters
    void changeTaskStatus(int task_id, TaskStatus status);

    // json stuff
    void loadTasks();
    void saveTasks();


    void printAllTasks() const;
    void printTaskById(int task_id) const;

private:
    
    std::unordered_map<int, Task> m_task_storage;

    int m_next_task_id = 1;

};

