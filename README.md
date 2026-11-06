# Book Reading Challenge Tracker — CA1

**Course:** CAP455 — Object-Oriented Programming using C++  
**Programme / Session:** MCA | Academic Session 2026–27  
**Student:** Yashodip Prabhakar Deore  
**Reg/Roll Number:** 12620514 / [42]  
**Faculty / Project Guide:** Prof. Adarsh Singh  
**Institution:** LPU, Computer Application

## Purpose
A menu-driven C++ console application for recording a reader's books within reading challenges, tracking pages read, searching entries, and viewing a basic progress summary.

## CA1 features
- Add a reading record with a unique Entry ID.
- View all records and search by Entry ID.
- Validate required text, page counts, and duplicate IDs.
- Calculate progress percentage and reading status.
- Display a summary of completed, in-progress, and not-started records.
- In-memory fixed array of up to 100 `ReadingEntry` objects.

## C++ concepts demonstrated
Class and objects, encapsulation, static data member, inline getters, friend display function, array of objects, object pointers, `this` pointer, and `std::string`.

## Build and run
From the project root:

```bash
g++ -std=c++11 -Wall -Wextra -pedantic src/main.cpp -o book_tracker
./book_tracker
```

On Windows with MinGW:

```powershell
g++ -std=c++11 -Wall -Wextra -pedantic src/main.cpp -o book_tracker.exe
.ook_tracker.exe
```

## Repository structure
- `docs/` — proposal, data dictionary, UML, flowchart, checklist
- `src/` — C++ source code
- `data/` — reserved for later persistent data files
- `tests/` — test cases and sample run evidence

## CA1 storage limitation
Records exist only while the program is running. File persistence, update/delete, inheritance, operator overloading, dynamic allocation, exceptions, templates, STL algorithms, and backup/export are planned for later CAs as appropriate to the supplied course guide.

## Important scope note
CA1 records the reader, book, author, challenge, total pages, and pages read. It does not yet provide accounts, dates/deadlines, leaderboards, multi-user networking, or permanent storage. Confirm any additional faculty-specific scenario requirements before submission.
