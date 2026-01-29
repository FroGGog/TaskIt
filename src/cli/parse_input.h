#pragma once

#include <string>
#include <iostream>
#include <vector>
#include <cctype>

#include "task_manager.h"

constexpr std::string_view Trim(std::string_view text_line);

void runCommand(TaskManager& manager, std::string_view text_line);

std::vector<std::string> parseCommand (std::string_view input);