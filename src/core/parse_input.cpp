#include "parse_input.h"

constexpr std::string_view Trim(std::string_view text_line)
{
    size_t iter_start = text_line.find_first_not_of(" \t\r\n");
    if (iter_start == std::string_view::npos) {
        return "";
    }
    size_t iter_end = text_line.find_last_not_of(" \t\r\n");
    return text_line.substr(iter_start, iter_end - iter_start + 1);
}

void runCommand(TaskManager &manager, std::string_view text_line)
{
    auto fullCommand = parseCommand(text_line);

    if(fullCommand.empty())
    {
        std::cerr << "\ntaskit> Unknow command (type help to get more info)\n";
        return;
    }

    if(fullCommand[0] == "addtask")
    {
        if(fullCommand.size() != 3)
        {
            std::cerr << "taskit> Usage: addtask \"title\" \"description\"\n";
            return;
        }
        else
        {
            manager.addTask(std::string(fullCommand[1]), std::string(fullCommand[2]));
        }
    }
    else if(fullCommand[0] == "list")
    {
        manager.printAllTasks();
    }
    else if(fullCommand[0] == "show")
    {
        if(fullCommand.size() != 2)
        {
            std::cerr << "taskit> Usage: show \"id\"\n";
            return;
        }
        else
        {
            try
            {
                manager.printTaskById(std::stoi(fullCommand[1]));
            }
            catch (const std::invalid_argument& e)
            {
                std::cout << "taskit> Usage: show \"id(int)\"\n";
            }
        }
    }
    else if(fullCommand[0] == "quit")
    {
        return;
    }
    else
    {
        std::cerr << "taskit> Unknow command (type help to get more info)\n";
    }
}

std::vector<std::string> parseCommand (std::string_view input)
{
    std::vector<std::string> tokens;
    std::string current;
    bool in_quotes = false;

    for (size_t i = 0; i < input.size(); ++i) {
        char c = input[i];

        if (c == '"') {
            in_quotes = !in_quotes;
            continue;
        }
        else if (std::isspace(static_cast<unsigned char>(c)) && !in_quotes) {
            if (!current.empty()) {
                tokens.push_back(std::move(current));
                current.clear();
            }
        }
        else {
            current += c;
        }
    }

    if (!current.empty()) {
        tokens.push_back(std::move(current));
    }

    return tokens;
}
