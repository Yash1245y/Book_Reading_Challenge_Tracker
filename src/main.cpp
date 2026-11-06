#include <iostream>
#include <string>
#include <limits>
#include <cctype>

using namespace std;

const int MAX_BOOKS = 100;

class ReadingEntry {
private:
    string entryId;
    string readerName;
    string bookTitle;
    string authorName;
    string challengeName;
    int pagesRead;
    int totalPages;
    static int totalEntries;

public:
    ReadingEntry() {
        entryId = "";
        readerName = "";
        bookTitle = "";
        authorName = "";
        challengeName = "";
        pagesRead = 0;
        totalPages = 0;
    }

    void setDetails(const string& id, const string& reader,
                    const string& title, const string& author,
                    const string& challenge, int read, int total) {
        this->entryId = id;
        this->readerName = reader;
        this->bookTitle = title;
        this->authorName = author;
        this->challengeName = challenge;
        this->pagesRead = read;
        this->totalPages = total;
        ++totalEntries;
    }

    inline string getEntryId() const { return entryId; }
    inline string getReaderName() const { return readerName; }
    inline string getBookTitle() const { return bookTitle; }
    inline string getChallengeName() const { return challengeName; }
    inline int getPagesRead() const { return pagesRead; }
    inline int getTotalPages() const { return totalPages; }

    int getProgressPercent() const {
        if (totalPages == 0) return 0;
        return (pagesRead * 100) / totalPages;
    }

    string getStatus() const {
        if (pagesRead == totalPages) return "Completed";
        if (pagesRead == 0) return "Not Started";
        return "In Progress";
    }

    static int getTotalEntries() { return totalEntries; }

    friend void displayEntry(const ReadingEntry& entry);
};

int ReadingEntry::totalEntries = 0;

void displayEntry(const ReadingEntry& entry) {
    cout << "\n----------------------------------------\n";
    cout << "Entry ID       : " << entry.entryId << '\n';
    cout << "Reader         : " << entry.readerName << '\n';
    cout << "Book Title     : " << entry.bookTitle << '\n';
    cout << "Author         : " << entry.authorName << '\n';
    cout << "Challenge      : " << entry.challengeName << '\n';
    cout << "Pages Read     : " << entry.pagesRead << " / " << entry.totalPages << '\n';
    cout << "Progress       : " << entry.getProgressPercent() << "%\n";
    cout << "Reading Status : " << entry.getStatus() << '\n';
    cout << "----------------------------------------\n";
}

bool isBlank(const string& value) {
    for (char ch : value) {
        if (!isspace(static_cast<unsigned char>(ch))) return false;
    }
    return true;
}

string readRequiredText(const string& prompt) {
    string value;
    while (true) {
        cout << prompt;
        getline(cin, value);
        if (!isBlank(value)) return value;
        cout << "Input cannot be empty. Please try again.\n";
    }
}

int readIntInRange(const string& prompt, int minimum, int maximum) {
    int value;
    while (true) {
        cout << prompt;
        if (cin >> value && value >= minimum && value <= maximum) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        }
        cout << "Enter a whole number from " << minimum << " to " << maximum << ".\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

int findEntryIndex(const ReadingEntry entries[], int count, const string& id) {
    for (int i = 0; i < count; ++i) {
        if (entries[i].getEntryId() == id) return i;
    }
    return -1;
}

void addReadingEntry(ReadingEntry entries[], int& count) {
    if (count >= MAX_BOOKS) {
        cout << "Storage is full. No more than " << MAX_BOOKS << " entries can be stored.\n";
        return;
    }

    cout << "\n--- Add Reading Record ---\n";
    string id = readRequiredText("Entry ID: ");
    if (findEntryIndex(entries, count, id) != -1) {
        cout << "Duplicate entry ID. Record was not added.\n";
        return;
    }

    string reader = readRequiredText("Reader name: ");
    string title = readRequiredText("Book title: ");
    string author = readRequiredText("Author name: ");
    string challenge = readRequiredText("Challenge name: ");

    int total = readIntInRange("Total pages in book (1-10000): ", 1, 10000);
    int read = readIntInRange("Pages read so far (0-total pages): ", 0, total);

    ReadingEntry* nextEntry = &entries[count];
    nextEntry->setDetails(id, reader, title, author, challenge, read, total);
    ++count;

    cout << "Reading record added successfully.\n";
}

void viewAllEntries(const ReadingEntry entries[], int count) {
    cout << "\n--- All Reading Records ---\n";
    if (count == 0) {
        cout << "No reading records available.\n";
        return;
    }
    for (int i = 0; i < count; ++i) displayEntry(entries[i]);
}

void searchEntry(const ReadingEntry entries[], int count) {
    cout << "\n--- Search Reading Record ---\n";
    if (count == 0) {
        cout << "No records to search.\n";
        return;
    }
    string id = readRequiredText("Enter entry ID: ");
    int index = findEntryIndex(entries, count, id);
    if (index == -1) {
        cout << "No reading record found for ID: " << id << '\n';
    } else {
        const ReadingEntry* foundEntry = &entries[index];
        displayEntry(*foundEntry);
    }
}

void showSummary(const ReadingEntry entries[], int count) {
    cout << "\n--- Reading Challenge Summary ---\n";
    if (count == 0) {
        cout << "No reading records available for summary.\n";
        return;
    }

    int completed = 0;
    int inProgress = 0;
    int notStarted = 0;
    int totalPagesRead = 0;
    int totalPagesInBooks = 0;

    for (int i = 0; i < count; ++i) {
        const string status = entries[i].getStatus();
        if (status == "Completed") ++completed;
        else if (status == "In Progress") ++inProgress;
        else ++notStarted;
        totalPagesRead += entries[i].getPagesRead();
        totalPagesInBooks += entries[i].getTotalPages();
    }

    cout << "Total reading records : " << count << '\n';
    cout << "Completed books       : " << completed << '\n';
    cout << "In-progress books     : " << inProgress << '\n';
    cout << "Not-started books     : " << notStarted << '\n';
    cout << "Pages read            : " << totalPagesRead << '\n';
    cout << "Total book pages      : " << totalPagesInBooks << '\n';
    cout << "Objects initialised   : " << ReadingEntry::getTotalEntries() << '\n';
}

int main() {
    ReadingEntry entries[MAX_BOOKS];
    int count = 0;
    int choice;

    do {
        cout << "\n========================================\n";
        cout << "     BOOK READING CHALLENGE TRACKER\n";
        cout << "========================================\n";
        cout << "1. Add reading record\n";
        cout << "2. View all reading records\n";
        cout << "3. Search record by Entry ID\n";
        cout << "4. View challenge summary\n";
        cout << "5. Exit\n";
        cout << "Enter your choice: ";

        if (!(cin >> choice)) {
            cout << "Invalid menu input. Enter a number from 1 to 5.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice) {
            case 1: addReadingEntry(entries, count); break;
            case 2: viewAllEntries(entries, count); break;
            case 3: searchEntry(entries, count); break;
            case 4: showSummary(entries, count); break;
            case 5: cout << "Thank you for using Book Reading Challenge Tracker.\n"; break;
            default: cout << "Choose a valid option from 1 to 5.\n";
        }
    } while (choice != 5);

    return 0;
}
