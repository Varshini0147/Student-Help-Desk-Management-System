# Student Help Desk Management System

A menu-driven **C command-line project** demonstrating a **Priority Queue** and an **Action Stack** applied to a student-service help desk workflow.

---

## Features
- **Submit Student Requests**: Records student name, category (e.g., ID card, Lab access, Fees, Scholarship, General), description, and priority.
- **Priority-Driven Scheduling**: Urgent requests take precedence (**High > Medium > Low**).
- **FIFO Tie-Breaking**: Requests with identical priority strictly follow First-In, First-Out (arrival order).
- **Recent Action History**: Stack-based LIFO log tracking submissions and processed requests with `PUSH`, `POP`, `PEEK`, and `DISPLAY`.
- **Robust Input & EOF Handling**:
  - Validates and rejects empty or whitespace-only strings.
  - Automatically flushes excess characters when input exceeds buffer bounds.
  - Cleanly detects End-of-File (EOF / Ctrl+D / Ctrl+Z) without infinite loops.
- **Automated Test Suite**: Dedicated automated unit test suites for both data structures.

---

## Data Structures

### 1. Priority Queue (`PriorityQueue`)
* **Underlying Implementation**: Contiguous array (`Request items[MAX_REQUESTS]`, capacity 100).
* **Urgency Levels**: `HIGH` (3) > `MEDIUM` (2) > `LOW` (1).
* **Insertion Logic (`pq_insert`)**:
  - Employs a backward insertion sort.
  - Comparison condition: `q->items[i - 1].priority < r.priority` (strict inequality).
  - Shifts elements with strictly lower priority to higher array indices.
  - Stops immediately when encountering an item with equal or higher priority.
  - **FIFO Guarantee**: Because equal-priority elements never shift past each other, earlier arrivals always stay closer to the front (index 0).
* **Processing (`pq_process_next`)**:
  - Always extracts `q->items[0]` (highest priority, earliest arrival).
  - Shifts remaining elements left by 1 index, maintaining order in $O(n)$ time.
* **Safety Bounds**:
  - Overflow check: Prevents inserting when `q->size >= MAX_REQUESTS`.
  - Underflow check: Safe handling when `q->size == 0`.

### 2. Action Stack (`ActionStack`)
* **Underlying Implementation**: Contiguous array (`char items[MAX_ACTIONS][ACTION_TEXT]`, capacity 100).
* **LIFO Discipline**: Last-In, First-Out action tracking.
* **Operations**:
  - `stack_push(s, action)`: Pushes a descriptive action string onto the stack ($O(1)$).
  - `stack_pop(s, out)`: Removes and retrieves the topmost action string into `out` ($O(1)$).
  - `stack_peek(s, out)`: Reads the topmost action string into `out` without removing it ($O(1)$).
  - `stack_display(s)`: Iterates from `top` down to `0`, displaying the recent actions in chronological reverse.
* **Safety Bounds**:
  - Overflow check: Prevents pushing when `s->top >= MAX_ACTIONS - 1`.
  - Underflow check: Returns 0 safely when `s->top < 0`.

---

## Project Structure
```text
Student_Help_Desk/
├── include/
│   ├── request.h              # Request struct, Priority enum, helper prototypes
│   ├── priority_queue.h       # PriorityQueue struct and API prototypes
│   └── stack.h                # ActionStack struct and API prototypes
├── src/
│   ├── main.c                 # CLI menu driver and user interaction
│   ├── request.c              # Input reading, validation, buffer flushing
│   ├── priority_queue.c       # Priority Queue implementation
│   └── stack.c                # Action Stack implementation
├── tests/
│   ├── test_priority_queue.c  # Automated unit tests for Priority Queue
│   ├── test_stack.c           # Automated unit tests for Action Stack
│   └── test-cases.md          # Manual test scenarios and expected outputs
├── docs/
│   └── demo-script.md         # Step-by-step presentation script
├── build.bat                  # Windows build script (supports GCC and MSVC)
├── Makefile                   # Linux/macOS Makefile
└── README.md                  # Project documentation
```

---

## Build and Compilation Instructions

### 1. Windows

#### Option A: Using `build.bat` (Recommended)
Open Command Prompt or PowerShell in the project directory:
```bat
REM Compile application and test suites
build.bat

REM Compile and automatically run all tests
build.bat test
```
*Note: `build.bat` automatically detects GCC in `PATH`. If GCC is not present, it automatically detects and compiles with MSVC (`cl.exe`).*

#### Option B: Manual Compilation with GCC (MinGW-w64)
```powershell
gcc -std=c99 -Wall -Wextra -Iinclude src/main.c src/request.c src/priority_queue.c src/stack.c -o helpdesk.exe
```

#### Option C: Manual Compilation with MSVC (Developer Command Prompt)
```cmd
cl /nologo /W4 /Iinclude src\main.c src\request.c src\priority_queue.c src\stack.c /Fe:helpdesk.exe
```

---

### 2. Linux / macOS

#### Option A: Using `make` (Recommended)
```bash
# Build main application and test binaries
make

# Build and execute all unit tests
make test

# Clean build artifacts
make clean
```

#### Option B: Manual Compilation with GCC / Clang
```bash
gcc -std=c99 -Wall -Wextra -Iinclude src/main.c src/request.c src/priority_queue.c src/stack.c -o helpdesk
chmod +x helpdesk
./helpdesk
```

---

## Running the Application
* **Windows**: `.\helpdesk.exe`
* **Linux / macOS**: `./helpdesk`

---

## Testing

### Automated Unit Tests
The project includes two standalone automated test runners:

1. **Priority Queue Test Suite (`tests/test_priority_queue.c`)**:
   - `test_empty_queue`: Verifies `pq_is_empty` and safe rejection on empty queue dequeue.
   - `test_priority_ordering`: Verifies `HIGH > MEDIUM > LOW` regardless of insertion order.
   - `test_fifo_tie_breaking`: Verifies multiple same-priority requests preserve strict arrival order.
   - `test_interleaved_priority_fifo`: Tests complex interleaving of mixed priorities and arrival orders.
   - `test_full_queue_and_overflow`: Inserts 100 items, verifies overflow rejection at 101, and validates complete drainage.

2. **Stack Test Suite (`tests/test_stack.c`)**:
   - `test_empty_stack_and_underflow`: Verifies top = -1, safe POP underflow, and safe PEEK.
   - `test_push_and_peek`: Verifies PEEK retrieves top item without modifying top index.
   - `test_pop_lifo`: Verifies strict LIFO order across multiple push/pop cycles.
   - `test_overflow`: Inserts 100 actions, verifies overflow rejection at 101, and checks full drain.

**Compiling and Running Tests Manually**:
```bash
# Priority Queue Tests
gcc -std=c99 -Wall -Wextra -Iinclude tests/test_priority_queue.c src/priority_queue.c src/request.c -o test_priority_queue
./test_priority_queue

# Stack Tests
gcc -std=c99 -Wall -Wextra -Iinclude tests/test_stack.c src/stack.c -o test_stack
./test_stack
```

---

## Limitations
- In-memory storage: all requests and actions are reset upon application exit.
- Static capacity: up to 100 pending requests and 100 recent action log entries.
- Designed as an academic prototype demonstrating fundamental data structures.

---

## Team
- **Tejal Surjit** — CLI and Menu Module
- **Varshini S** — Priority Queue Module
- **Surabhi Sangam** — Stack Module
