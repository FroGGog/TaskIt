#include <gtest/gtest.h>
#include <string>
#include <vector>
#include "task.h"
#include "task_manager.h"
#include "parse_input.h" 

// === Parser test ===
TEST(ParseCommandTest, EmptyInput) {
    auto tokens = parseCommand("");
    EXPECT_TRUE(tokens.empty());
}

TEST(ParseCommandTest, OnlySpaces) {
    auto tokens = parseCommand("   \t  ");
    EXPECT_TRUE(tokens.empty());
}

TEST(ParseCommandTest, SimpleCommand) {
    auto tokens = parseCommand("list");
    ASSERT_EQ(tokens.size(), 1);
    EXPECT_EQ(tokens[0], "list");
}

TEST(ParseCommandTest, AddTaskWithQuotes) {
    auto tokens = parseCommand(R"(addtask "Clean room" "Very dirty")");
    ASSERT_EQ(tokens.size(), 3);
    EXPECT_EQ(tokens[0], "addtask");
    EXPECT_EQ(tokens[1], "Clean room");
    EXPECT_EQ(tokens[2], "Very dirty");
}

TEST(ParseCommandTest, MixedQuotedAndUnquoted) {
    auto tokens = parseCommand(R"(addtask Buy "Milk and bread")");
    ASSERT_EQ(tokens.size(), 3);
    EXPECT_EQ(tokens[1], "Buy");
    EXPECT_EQ(tokens[2], "Milk and bread");
}

TEST(ParseCommandTest, EscapedQuotesNotSupportedButHandledAsNormal) {
    auto tokens = parseCommand(R"(addtask "He said "Hello"" "World")");
    EXPECT_GE(tokens.size(), 2);
}

// === TaskManager Tests ===
TEST(TaskManagerTest, AddTask_CreatesTaskWithCorrectData) {
    TaskManager manager;
    manager.addTask("Fix bug", "In parser module");

    ASSERT_EQ(manager.getAllTasks().size(), 1);
    const auto& task = manager.getTaskById(1);
    EXPECT_EQ(task.getTitle(), "Fix bug");
    EXPECT_EQ(task.getDescription(), "In parser module");
    EXPECT_EQ(task.getStatus(), TaskStatus::TO_DO);
}

TEST(TaskManagerTest, AddMultipleTasks_GeneratesUniqueIds) {
    TaskManager manager;
    manager.addTask("A", "B");
    manager.addTask("C", "D");

    auto tasks = manager.getAllTasks();
    ASSERT_EQ(tasks.size(), 2);

    std::set<int> ids;
    for (const auto& [id, _] : tasks) {
        ids.insert(id);
    }
    EXPECT_EQ(ids.size(), 2);
}

TEST(TaskManagerTest, GetTaskById_ThrowsOnInvalidId) {
    TaskManager manager;
    manager.addTask("Valid", "Task");

    EXPECT_THROW(manager.getTaskById(0), std::out_of_range);
    EXPECT_THROW(manager.getTaskById(999), std::out_of_range);
}

// === CLI integration tests ===
TEST(CLIIntegrationTest, RunCommand_AddTask_ValidInput) {
    TaskManager manager;
    runCommand(manager, R"(addtask "Test title" "Test desc")");

    ASSERT_EQ(manager.getAllTasks().size(), 1);
    const auto& task = manager.getTaskById(1);
    EXPECT_EQ(task.getTitle(), "Test title");
    EXPECT_EQ(task.getDescription(), "Test desc");
}

TEST(CLIIntegrationTest, RunCommand_AddTask_InvalidArgCount_ShowsError) {
    TaskManager manager;
    runCommand(manager, "addtask \"Only title\"");
    EXPECT_EQ(manager.getAllTasks().size(), 0);
}

TEST(CLIIntegrationTest, RunCommand_List_PrintsNothingWhenEmpty) {
    TaskManager manager;
    EXPECT_NO_THROW(runCommand(manager, "list"));
}

TEST(CLIIntegrationTest, RunCommand_UnknownCommand_ShowsError) {
    TaskManager manager;
    EXPECT_NO_THROW(runCommand(manager, "unknowncmd"));
}