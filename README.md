# Book Reading Challenge Tracker

A menu-driven C++ console application for tracking reading progress in named challenges.

## CA2 features
- Create and display reading records.
- Search persistent records by Entry ID using `seekg()`.
- Update pages-read progress using `seekp()`.
- Store fixed-size binary records in `data/reading_entries.dat`.
- Preserve records after closing and restarting the application.
- Validate required text, unique IDs, page ranges, and fixed storage capacities.
- Calculate progress percentage and reading status.

## Requirements
- C++11-compatible compiler (GCC/MinGW recommended).

## Build and run (Windows PowerShell)
Run these commands from the project root:

```powershell
New-Item -ItemType Directory -Force -Path .\data
g++ -std=c++11 -Wall -Wextra -pedantic .\src\main.cpp -o .\book_tracker.exe
.\book_tracker.exe
```

## Build and run (Linux/macOS)
Run from the project root:

```bash
mkdir -p data
g++ -std=c++11 -Wall -Wextra -pedantic src/main.cpp -o book_tracker
./book_tracker
```

The working directory must be the project root because the runtime path is `data/reading_entries.dat`.

## Repository structure
- `src/main.cpp` — application source.
- `data/` — runtime binary file (created/used by the application).
- `docs/` — proposal, report, diagrams and documentation.
- `tests/` — test plan and verification record.

## Storage note
The program stores `ReadingEntryRecord` fixed-size structures, not raw `ReadingEntry` objects. The application class contains `std::string`, whose internal representation must not be written directly as raw binary bytes. The binary structure is intended for compatible builds of this project and is not a portable database format.

## Current scope
This phase supports adding, viewing, searching, and updating pages-read progress. Record deletion, full metadata editing, database connectivity, multi-user access, reminders, leaderboards, and network collaboration are not included.
