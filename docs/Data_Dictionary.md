# Data Dictionary — Book Reading Challenge Tracker (CA1)

| Attribute | Data type | Visibility | Purpose and validation |
|---|---|---|---|
| `entryId` | `std::string` | Private | Unique reading-record identifier; required and must not duplicate an existing ID. |
| `readerName` | `std::string` | Private | Name of the reader; required. |
| `bookTitle` | `std::string` | Private | Title of the book; required. |
| `authorName` | `std::string` | Private | Book author; required. |
| `challengeName` | `std::string` | Private | Reading challenge/group label; required. |
| `pagesRead` | `int` | Private | Pages completed so far; whole number from 0 through `totalPages`. |
| `totalPages` | `int` | Private | Total pages in the book; whole number from 1 through 10,000. |
| `totalEntries` | `int` | Private static | Class-level count of records initialised during the current execution. |

## Derived values
- **Progress percentage:** `(pagesRead * 100) / totalPages` using integer arithmetic.
- **Status:** `Not Started` when pages read is 0; `Completed` when pages read equals total pages; otherwise `In Progress`.

## Unique key
`entryId` is the unique key. The program searches the populated array before adding a record and rejects duplicate IDs.

## CA1 storage
A fixed array stores up to 100 `ReadingEntry` objects. Records are in memory only and are lost when the program exits.
