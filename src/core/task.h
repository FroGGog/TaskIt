#pragma once

#include <string>
#include <cstdint>
#include <iostream>
#include <optional>

enum class TaskStatus: std::uint8_t {TO_DO, IN_PROGRESS, DONE};

std::optional<std::string> statusToString(TaskStatus status);
std::optional<TaskStatus> stringToStatus(std::string_view str_status);

class Task
{
public:
    Task(std::string task_title, std::string description);

    // setters
    void setTitle(std::string new_title);
    void setDescription(std::string new_description);
    void setStatus(TaskStatus new_status);

    // getters
    [[nodiscard]] std::string_view getTitle() const;
    [[nodiscard]] std::string_view getDescription() const;
    [[nodiscard]] TaskStatus getStatus() const;

private:

    std::string m_task_title;
    std::string m_description;
    TaskStatus m_task_status = TaskStatus::TO_DO;
};