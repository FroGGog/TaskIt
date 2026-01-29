// tests/test_core.cpp
#include <gtest/gtest.h>
#include <stdexcept>
#include <set>

#include "task.h"        
#include "task_manager.h"



TEST(TaskManagerTest, AddAndGetTask) 
{
    TaskManager manager;
    manager.addTask("Test", "Description");

    const auto& task = manager.getTaskById(1);
    EXPECT_EQ(task.getTitle(), "Test");
    EXPECT_EQ(task.getDescription(), "Description");
    EXPECT_EQ(task.getStatus(), TaskStatus::TO_DO);
}

TEST(TaskManager, GetTaskCount)
{
    TaskManager manager;
    manager.addTask("Test1", "Description");
    manager.addTask("Test2", "Description");
    manager.addTask("Test3", "Description");

    const auto& task_count = manager.getAllTasks().size();
    EXPECT_EQ(task_count, 3);
}

TEST(TaskManager, GetTaskById_WithInvalidId_ThrowsException)
{
    TaskManager manager;
    EXPECT_THROW(manager.getTaskById(100), std::out_of_range);
}

TEST(TaskManager, AddMultipleTasks_GeneratesUniqueIds)
{
    TaskManager manager;
    const int num_tasks = 10;

    for(auto i = 0; i < num_tasks; ++i)
    {
        manager.addTask("Test", "test");
    }

    std::set<int> unique_ids;
    for(const auto& iter : manager.getAllTasks())
    {
        unique_ids.insert(iter.first);
    }
    EXPECT_EQ(manager.getAllTasks().size(), num_tasks);
    EXPECT_EQ(unique_ids.size(), num_tasks);
}

