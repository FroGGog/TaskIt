#include <iostream>

#include "task_manager.h"

int main() // NOLINT(bugprone-exception-escape)
{
    try {
        TaskManager test_manager;
        test_manager.addTask("Test", "description");
        std::cout << test_manager.getTaskById(1).getTitle() << '\n';
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
