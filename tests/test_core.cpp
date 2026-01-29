// tests/test_core.cpp
#include <gtest/gtest.h>
#include "task.h"        
#include "task_manager.h"

TEST(TaskManagerTest, AddAndGetTask) 
{
    TaskManager manager;
    manager.addTask("Test", "Description");

    const auto& task = manager.getTaskById(1);
    EXPECT_EQ(task.getTitle(), "Test");
    EXPECT_EQ(task.getDescription(), "Description");
}

TEST(TaskManager, GetTaskCount)
{
    TaskManager manager;
    manager.addTask("Test1", "Description");
    manager.addTask("Test2", "Description");
    manager.addTask("Test3", "Description");

    const auto& task_count = manager.getAllTasks().size();
    const auto& tasks = manager.getAllTasks();
    EXPECT_EQ(task_count, 3);
    EXPECT_EQ(tasks.at(1).getTitle(), "Test1");
    EXPECT_EQ(tasks.at(3).getTitle(), "Test3");
}

