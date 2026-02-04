# Architecture Overview - TaskIt

## 1. Overview
**TaskIt** is a cross-platform task management application with both CLI and GUI interfaces, built using modern C++. The project follows a modular architecture separating core business logic from presentation layers.

## 2. High-Level Architecture

```
TaskIt Application
├── CLI Interface (Console)
├── GUI Interface (SFML-based)
└── Core Engine (Shared)
```

## 3. Module Breakdown

### 3.1 Core Module (`src/core/`)
**Purpose**: Contains all business logic and data models independent of UI.

#### 3.1.1 Task Model (`task.h`, `task.cpp`)
- **Entity Class**: `Task`
- **Key Components**:
  - `m_task_title`: Task title (string)
  - `m_description`: Task description (string)
  - `m_task_status`: Status enum (TO_DO, IN_PROGRESS, DONE)
- **Status Conversion**:
  - `statusToString()`: Enum → Human-readable string
  - `stringToStatus()`: String → Enum (with validation)
- **Design Pattern**: Simple Data Object with getters/setters

#### 3.1.2 Task Manager (`task_manager.h`, `task_manager.cpp`)
- **Manager Class**: `TaskManager`
- **Key Components**:
  - `m_task_storage`: `unordered_map<int, Task>` for O(1) lookups
  - `m_next_task_id`: Auto-incrementing ID generator
- **Core Operations**:
  - CRUD operations (Create via `addTask`, Read via `getTaskById`/`getAllTasks`)
  - Status management via `changeTaskStatus`
  - View operations: `printAllTasks()`, `printTaskById()`
- **Design Pattern**: Repository/Manager pattern

#### 3.1.3 Input Parser (`parse_input.h`, `parse_input.cpp`)
- **Parser Functions**: `parseCommand()`, `runCommand()`
- **Features**:
  - Quote-aware tokenization (supports spaces in arguments)
  - Command validation and routing
  - Error handling with user-friendly messages
- **Supported Commands**:
  - `addtask "title" "description"`
  - `list`
  - `show id`
  - `status id (todo|progress|done)`
  - `quit`

### 3.2 CLI Module (`src/cli/`)
**Purpose**: Command-line interface layer.

#### 3.2.1 Main Loop (`main.cpp`)
- **REPL (Read-Eval-Print Loop)** pattern
- **Flow**: Prompt → Input → Parse → Execute → Display
- **Error Handling**: Comprehensive exception catching at top level
- **Exit Strategy**: Graceful shutdown on "quit" command

### 3.3 GUI Module (`src/gui/`)
**Purpose**: Graphical user interface using SFML.

#### 3.3.1 Window Management (`MainWindow.h`, `MainWindow.cpp`)
- **Class**: `MainWindow`
- **SFML Integration**: Wraps `sf::RenderWindow`
- **Game Loop Pattern**:
  ```
  while(window.isOpen())
      handleEvents()
      update()
      render()
  ```
- **Current State**: Basic window setup (GUI logic not fully implemented)

### 3.4 Testing Module (`tests/`)
**Purpose**: Unit and integration testing using Google Test.

#### 3.4.1 Test Coverage (`test_core.cpp`)
- **Parser Tests**: Command parsing with various input formats
- **Task Tests**: Object construction and state management
- **TaskManager Tests**: Storage, ID generation, and operations
- **Integration Tests**: CLI command execution flows
- **Edge Cases**: Invalid inputs, error conditions, boundary values

## 4. Data Flow

### 4.1 CLI Data Flow
```
User Input → parseCommand() → Command Tokens → runCommand()
    ↓
TaskManager Operation → Task Storage Update → Output Display
```

### 4.2 Data Persistence
- **Current**: In-memory only (no file/database persistence)
- **Storage**: `unordered_map<int, Task>` during runtime
- **Lifetime**: Data lost on application exit

## 5. Design Patterns and Principles

### 5.1 Applied Patterns
1. **Separation of Concerns**: UI, Business Logic, Data layers separated
2. **Single Responsibility**: Each class has clear, focused purpose
3. **Repository Pattern**: `TaskManager` abstracts data access
4. **Command Pattern**: Parser translates strings to operations
5. **REPL Pattern**: CLI follows classic command-line interface pattern

### 5.2 SOLID Principles Compliance
- **Single Responsibility**: Each class/module has one reason to change
- **Open/Closed**: New commands can be added without modifying parser logic
- **Liskov Substitution**: N/A (minimal inheritance)
- **Interface Segregation**: Small, focused interfaces
- **Dependency Inversion**: High-level modules don't depend on low-level details

## 6. Key Design Decisions

### 6.1 Storage Choice
- **`unordered_map<int, Task>`**: Fast O(1) lookups by ID
- **Auto-incrementing IDs**: Simple, predictable, no collisions
- **Trade-off**: No ordering, but `list` command maintains insertion order

### 6.2 Error Handling Strategy
- **User Level**: Friendly error messages with usage examples
- **System Level**: Exception catching with graceful degradation
- **Validation**: Input validation before operations

### 6.3 Quote Handling
- **Smart Parsing**: Supports quoted arguments with spaces
- **Limitation**: No escape character support for quotes within quotes

## 7. Dependencies

### 7.1 Core Dependencies
- **C++ Standard Library**: Containers, strings, I/O
- **No external dependencies** for core functionality

### 7.2 Optional Dependencies
- **SFML**: Only for GUI compilation
- **Google Test**: Only for test compilation

## 8. Build Configuration
```
Project Structure:
TaskIt/
├── src/
│   ├── cli/      # Command-line interface
│   ├── core/     # Business logic
│   └── gui/      # Graphical interface
├── tests/        # Unit tests
└── (build system files)
```

## 9. Extensibility Points

### 9.1 Easy to Extend
1. **New Commands**: Add to `runCommand()` switch statement
2. **New Task Fields**: Extend `Task` class and manager methods
3. **New UI**: Add new interface module following existing patterns
4. **Persistence**: Add serialization to `TaskManager`

### 9.2 Planned/Obvious Extensions
1. **File Storage**: JSON/XML serialization for task persistence
2. **GUI Features**: Visual task lists, drag-and-drop status changes
3. **Advanced Features**: Tags, priorities, due dates, search/filter
4. **Network**: Multi-user/sync capabilities

## 10. Testing Strategy

### 10.1 Test Types
- **Unit Tests**: Individual functions/classes
- **Integration Tests**: Command parsing + execution
- **Edge Cases**: Invalid inputs, boundary conditions

### 10.2 Test Tools
- **Google Test**: Primary testing framework
- **Output Capture**: Testing console output/errors
- **Mocking**: N/A (minimal dependencies)

## 11. Known Limitations

### 11.1 Current Version
- No data persistence between sessions
- Basic GUI skeleton only (no task visualization)
- Limited error recovery
- No undo/redo functionality
- Single-user, local-only

### 11.2 Scalability Considerations
- In-memory storage limits based on available RAM
- Linear time for `printAllTasks()` (O(n))
- No pagination for large task lists

## 12. Conclusion

TaskIt demonstrates clean architectural separation between:
1. **Domain Layer** (`Task`, `TaskManager`)
2. **Application Layer** (`parse_input`, command routing)
3. **Presentation Layer** (CLI, GUI)

The architecture supports:
- **Multiple frontends** (CLI, GUI) sharing core logic
- **Testability** through clear interfaces
- **Maintainability** through modular design
- **Extensibility** through well-defined extension points

This foundation allows for future evolution into a fully-featured task management system with minimal architectural changes.