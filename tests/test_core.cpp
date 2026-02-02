#include <gtest/gtest.h>
#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <set>    
#include "task.h"
#include "task_manager.h"
#include "parse_input.h"

// Вспомогательная функция для проверки подстроки
bool Contains(const std::string& str, const std::string& substr) {
    return str.find(substr) != std::string::npos;
}

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

// === Task Tests ===
TEST(TaskTest, ConstructorInitializesFields) {
    Task task("Title", "Description");
    EXPECT_EQ(task.getTitle(), "Title");
    EXPECT_EQ(task.getDescription(), "Description");
    EXPECT_EQ(task.getStatus(), TaskStatus::TO_DO);
}

TEST(TaskTest, SettersChangeFields) {
    Task task("Old Title", "Old Desc");
    task.setTitle("New Title");
    task.setDescription("New Desc");
    task.setStatus(TaskStatus::DONE);

    EXPECT_EQ(task.getTitle(), "New Title");
    EXPECT_EQ(task.getDescription(), "New Desc");
    EXPECT_EQ(task.getStatus(), TaskStatus::DONE);
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

TEST(TaskManagerTest, ChangeTaskStatus_UpdatesStatus) {
    TaskManager manager;
    manager.addTask("Task A", "Desc A");
    manager.changeTaskStatus(1, TaskStatus::DONE);
    EXPECT_EQ(manager.getTaskById(1).getStatus(), TaskStatus::DONE);
}

TEST(TaskManagerTest, PrintAllTasks_Empty_NoCrash) {
    TaskManager manager;
    EXPECT_NO_THROW(manager.printAllTasks());
}

// Тест для перехвата вывода printAllTasks
TEST(TaskManagerTest, PrintAllTasks_Output) {
    TaskManager manager;
    manager.addTask("Task 1", "Description 1");
    manager.addTask("Task 2", "Description 2");

    // Перехватываем std::cout
    testing::internal::CaptureStdout();
    manager.printAllTasks();
    std::string output = testing::internal::GetCapturedStdout();

    // Проверяем, что в выводе есть нужные данные
    EXPECT_TRUE(Contains(output, "Task 1"));
    EXPECT_TRUE(Contains(output, "Description 1"));
    EXPECT_TRUE(Contains(output, "Task 2"));
    EXPECT_TRUE(Contains(output, "Description 2"));
    // Проверяем, что статус по умолчанию To Do
    EXPECT_TRUE(Contains(output, "To Do"));
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
    // Перехватываем std::cerr
    testing::internal::CaptureStderr();
    runCommand(manager, "addtask \"Only title\""); // Только заголовок
    std::string error_output = testing::internal::GetCapturedStderr();
    EXPECT_EQ(manager.getAllTasks().size(), 0);
    EXPECT_TRUE(Contains(error_output, "Usage: addtask")); // Проверяем сообщение об ошибке

    // Проверим случай без аргументов
    testing::internal::CaptureStderr();
    runCommand(manager, "addtask"); // Без аргументов
    error_output = testing::internal::GetCapturedStderr();
    EXPECT_EQ(manager.getAllTasks().size(), 0);
    EXPECT_TRUE(Contains(error_output, "Usage: addtask"));
}

TEST(CLIIntegrationTest, RunCommand_List_PrintsNothingWhenEmpty) {
    TaskManager manager;
    EXPECT_NO_THROW(runCommand(manager, "list"));
}

TEST(CLIIntegrationTest, RunCommand_UnknownCommand_ShowsError) {
    TaskManager manager;
    testing::internal::CaptureStderr();
    runCommand(manager, "unknowncmd");
    std::string error_output = testing::internal::GetCapturedStderr();
    EXPECT_TRUE(Contains(error_output, "Unknow command"));
}

// --- Новые тесты для команд show и status ---

// --- Тесты для команды show ---
TEST(CLIIntegrationTest, RunCommand_Show_ExistingTask_PrintsDetails) {
    TaskManager manager;
    manager.addTask("Show Test", "This task should be shown.");
    manager.getTaskById(1).setStatus(TaskStatus::IN_PROGRESS); // Меняем статус для проверки

    testing::internal::CaptureStdout();
    runCommand(manager, "show 1");
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_TRUE(Contains(output, "ID: 1"));
    EXPECT_TRUE(Contains(output, "Show Test"));
    EXPECT_TRUE(Contains(output, "This task should be shown."));
    EXPECT_TRUE(Contains(output, "In Progress"));
}

TEST(CLIIntegrationTest, RunCommand_Show_NonExistentTask_ShowsError) {
    TaskManager manager;
    manager.addTask("Another Task", "Desc");
    testing::internal::CaptureStderr();
    runCommand(manager, "show 5");
    std::string error_output = testing::internal::GetCapturedStderr();
    EXPECT_TRUE(Contains(error_output, "No task with such ID"));
}

TEST(CLIIntegrationTest, RunCommand_Show_InvalidIdFormat_ShowsError) {
    TaskManager manager;
    manager.addTask("Task", "Desc");
    testing::internal::CaptureStderr();
    runCommand(manager, "show invalid_id");
    std::string error_output = testing::internal::GetCapturedStderr();
    EXPECT_TRUE(Contains(error_output, "Usage: show \"id(int)\""));
}

TEST(CLIIntegrationTest, RunCommand_Show_MissingId_ShowsError) {
    TaskManager manager;
    manager.addTask("Task", "Desc");
    testing::internal::CaptureStderr();
    runCommand(manager, "show");
    std::string error_output = testing::internal::GetCapturedStderr();
    EXPECT_TRUE(Contains(error_output, "Usage: show \"id\""));
}

TEST(CLIIntegrationTest, RunCommand_Show_TooManyArgs_ShowsError) {
    TaskManager manager;
    manager.addTask("Task", "Desc");
    testing::internal::CaptureStderr();
    runCommand(manager, "show 1 extra_arg");
    std::string error_output = testing::internal::GetCapturedStderr();
    EXPECT_TRUE(Contains(error_output, "Usage: show \"id\""));
}


// --- Тесты для команды status ---
TEST(CLIIntegrationTest, RunCommand_Status_ExistingTask_ChangesStatus) {
    TaskManager manager;
    manager.addTask("Status Test", "This task's status will change.");
    EXPECT_EQ(manager.getTaskById(1).getStatus(), TaskStatus::TO_DO);

    runCommand(manager, "status 1 todo");
    EXPECT_EQ(manager.getTaskById(1).getStatus(), TaskStatus::TO_DO);

    runCommand(manager, "status 1 progress");
    EXPECT_EQ(manager.getTaskById(1).getStatus(), TaskStatus::IN_PROGRESS);

    runCommand(manager, "status 1 done");
    EXPECT_EQ(manager.getTaskById(1).getStatus(), TaskStatus::DONE);

    // Проверим, что статус "none" также работает (хотя и с предупреждением)
    testing::internal::CaptureStderr();
    runCommand(manager, "status 1 none");
    std::string warning_output = testing::internal::GetCapturedStderr();
    // После вызова none статус должен быть NONE
    EXPECT_EQ(manager.getTaskById(1).getStatus(), TaskStatus::NONE);
    EXPECT_TRUE(Contains(warning_output, "No such status")); // Проверяем предупреждение
}

TEST(CLIIntegrationTest, RunCommand_Status_NonExistentTask_ShowsError) {
    TaskManager manager;
    manager.addTask("Another Task", "Desc");
    testing::internal::CaptureStderr();
    runCommand(manager, "status 5 done");
    std::string error_output = testing::internal::GetCapturedStderr();
    EXPECT_TRUE(Contains(error_output, "at"));
}

TEST(CLIIntegrationTest, RunCommand_Status_InvalidIdFormat_ShowsError) {
    TaskManager manager;
    manager.addTask("Task", "Desc");
    testing::internal::CaptureStderr();
    runCommand(manager, "status invalid_id done");
    std::string error_output = testing::internal::GetCapturedStderr();
    EXPECT_TRUE(Contains(error_output, "Usage: status \"id(int)\" \"status(todo, progress, done)\""));
}

TEST(CLIIntegrationTest, RunCommand_Status_InvalidStatusFormat_ShowsError) {
    TaskManager manager;
    manager.addTask("Task", "Desc");
    testing::internal::CaptureStderr();
    runCommand(manager, "status 1 invalid_status");
    std::string error_output = testing::internal::GetCapturedStderr();
    EXPECT_TRUE(Contains(error_output, "No such status")); // Сообщение из stringToStatus
}

TEST(CLIIntegrationTest, RunCommand_Status_MissingArgs_ShowsError) {
    TaskManager manager;
    manager.addTask("Task", "Desc");
    testing::internal::CaptureStderr();
    runCommand(manager, "status");
    std::string error_output = testing::internal::GetCapturedStderr();
    EXPECT_TRUE(Contains(error_output, "Usage: status \"id\" \"(todo, progress, done)\""));

    testing::internal::CaptureStderr();
    runCommand(manager, "status 1");
    error_output = testing::internal::GetCapturedStderr();
    EXPECT_TRUE(Contains(error_output, "Usage: status \"id\" \"(todo, progress, done)\""));
}

TEST(CLIIntegrationTest, RunCommand_Status_TooManyArgs_ShowsError) {
    TaskManager manager;
    manager.addTask("Task", "Desc");
    testing::internal::CaptureStderr();
    runCommand(manager, "status 1 todo extra_arg");
    std::string error_output = testing::internal::GetCapturedStderr();
    EXPECT_TRUE(Contains(error_output, "Usage: status \"id\" \"(todo, progress, done)\""));
}

// --- Дополнительные тесты для внутренних функций ---
TEST(StringToStatusTest, ValidStatusesConvertCorrectly) {
    EXPECT_EQ(stringToStatus("todo"), TaskStatus::TO_DO);
    EXPECT_EQ(stringToStatus("progress"), TaskStatus::IN_PROGRESS);
    EXPECT_EQ(stringToStatus("done"), TaskStatus::DONE);
}

TEST(StringToStatusTest, InvalidStatusReturnsNoneAndPrintsError) {
    testing::internal::CaptureStderr();
    TaskStatus status = stringToStatus("invalid");
    std::string error_output = testing::internal::GetCapturedStderr();
    EXPECT_EQ(status, TaskStatus::NONE);
    EXPECT_TRUE(Contains(error_output, "No such status"));
}

TEST(StatusToStringTest, AllStatusesConvertToStringCorrectly) {
    EXPECT_EQ(statusToString(TaskStatus::TO_DO), "To Do");
    EXPECT_EQ(statusToString(TaskStatus::IN_PROGRESS), "In Progress");
    EXPECT_EQ(statusToString(TaskStatus::DONE), "Done");
    EXPECT_EQ(statusToString(TaskStatus::NONE), "None"); // Проверка default case
}