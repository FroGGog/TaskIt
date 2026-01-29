#include <iostream>

#include "task_manager.h"
#include "parse_input.h"

int main() // NOLINT(bugprone-exception-escape)
{
    try {
        TaskManager manager;
        std::string user_info;
        while(user_info != "quit")
        {            
            std::cout << "TaskIt>";
            std::getline(std::cin, user_info);
            runCommand(manager, user_info);
        }
        return 0;
    }
    catch (const std::bad_alloc&) {
        std::cerr << "Bad_alloc error.\n";
        return 1;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
    catch (...) {
        std::cerr << "Unknown error.\n";
        return 1;
    }
}
