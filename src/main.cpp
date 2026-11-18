#include <iostream>
#include <fstream>
#include <string>
#include <limits>
#include <cctype>
#include <cstring>
#include <sstream>
#include <cstdlib>
#include <cstddef>

using namespace std;

const char DATA_FILE[] = "data/reading_entries.dat";
const int MAX_BOOKS = 100;

struct ReadingEntryRecord {
    char entryId[32];
    char readerName[64];
    char bookTitle[128];
    char authorName[64];
    char challengeName[64];
    int pagesRead;
    int totalPages;
};

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
    static int liveObjects;

    static bool copyText(char* destination, size_t capacity, const string& value) {
        if (value.size() >= capacity) return false;
        memset(destination, 0, capacity);
        memcpy(destination, value.c_str(), value.size() + 1);
        return true;
    }

public:
    ReadingEntry()
        : entryId(""), readerName(""), bookTitle(""), authorName(""),
          challengeName(""), pagesRead(0), totalPages(0) {
        ++liveObjects;
    }

    ReadingEntry(const string& id, const string& reader, const string& title,
                 const string& author, const string& challenge,
                 int read, int total)
        : entryId(id), readerName(reader), bookTitle(title), authorName(author),
          challengeName(challenge), pagesRead(read), totalPages(total) {
        ++totalEntries;
        ++liveObjects;
    }

    ReadingEntry(const ReadingEntry& other)
        : entryId(other.entryId), readerName(other.readerName),
          bookTitle(other.bookTitle), authorName(other.authorName),
          challengeName(other.challengeName), pagesRead(other.pagesRead),
          totalPages(other.totalPages) {
        ++totalEntries;
        ++liveObjects;
    }

    ~ReadingEntry() {
        --liveObjects;
    }

    void setDetails(const string& id, const string& reader,
                    const string& title, const string& author,
                    const string& challenge, int read, int total) {
        if (entryId.empty() && !id.empty()) ++totalEntries;
        this->entryId = id;
        this->readerName = reader;
        this->bookTitle = title;
        this->authorName = author;
        this->challengeName = challenge;
        this->pagesRead = read;
        this->totalPages = total;
    }

    inline string getEntryId() const { return entryId; }
    inline string getReaderName() const { return readerName; }
    inline string getBookTitle() const { return bookTitle; }
    inline string getAuthorName() const { return authorName; }
    inline string getChallengeName() const { return challengeName; }
    inline int getPagesRead() const { return pagesRead; }
    inline int getTotalPages() const { return totalPages; }

    int getProgressPercent() const {
        return totalPages == 0 ? 0 : (pagesRead * 100) / totalPages;
    }

    string getStatus() const {
        if (pagesRead == 0) return "Not Started";
        if (pagesRead == totalPages) return "Completed";
        return "In Progress";
    }

    bool toRecord(ReadingEntryRecord& record) const {
        memset(&record, 0, sizeof(record));
        if (!copyText(record.entryId, sizeof(record.entryId), entryId) ||
            !copyText(record.readerName, sizeof(record.readerName), readerName) ||
            !copyText(record.bookTitle, sizeof(record.bookTitle), bookTitle) ||
            !copyText(record.authorName, sizeof(record.authorName), authorName) ||
            !copyText(record.challengeName, sizeof(record.challengeName), challengeName)) {
            return false;
        }
        record.pagesRead = pagesRead;
        record.totalPages = totalPages;
        return true;
    }

    static int getTotalEntries() { return totalEntries; }
    static int getLiveObjects() { return liveObjects; }

    friend void displayEntry(const ReadingEntry& entry);
};

int ReadingEntry::totalEntries = 0;
int ReadingEntry::liveObjects = 0;

void displayEntry(const ReadingEntry& entry) {
    cout << "\n----------------------------------------\n"
         << "Entry ID       : " << entry.entryId << '\n'
         << "Reader         : " << entry.readerName << '\n'
         << "Book Title     : " << entry.bookTitle << '\n'
         << "Author         : " << entry.authorName << '\n'
         << "Challenge      : " << entry.challengeName << '\n'
         << "Pages Read     : " << entry.pagesRead << " / " << entry.totalPages << '\n'
         << "Progress       : " << entry.getProgressPercent() << "%\n"
         << "Reading Status : " << entry.getStatus() << '\n'
         << "----------------------------------------\n";
}

bool isBlank(const string& value) {
    for (size_t i = 0; i < value.size(); ++i) {
        if (!isspace(static_cast<unsigned char>(value[i]))) return false;
    }
    return true;
}

string readRequiredText(const string& prompt) {
    string value;
    while (true) {
        cout << prompt;
        if (!getline(cin, value)) {
            cout << "\nInput ended. Exiting.\n";
            exit(EXIT_SUCCESS);
        }
        if (!isBlank(value)) return value;
        cout << "Input cannot be empty. Please try again.\n";
    }
}

int readIntInRange(const string& prompt, int minimum, int maximum) {
    string line;
    while (true) {
        cout << prompt;
        if (!getline(cin, line)) {
            cout << "\nInput ended. Exiting.\n";
            exit(EXIT_SUCCESS);
        }
        istringstream input(line);
        long long value;
        char extra;
        if ((input >> value) && !(input >> extra) &&
            value >= minimum && value <= maximum) {
            return static_cast<int>(value);
        }
        cout << "Enter a whole number from " << minimum << " to " << maximum << ".\n";
    }
}

bool validFieldLengths(const string& id, const string& reader,
                       const string& title, const string& author,
                       const string& challenge) {
    return id.size() < sizeof(ReadingEntryRecord::entryId) &&
           reader.size() < sizeof(ReadingEntryRecord::readerName) &&
           title.size() < sizeof(ReadingEntryRecord::bookTitle) &&
           author.size() < sizeof(ReadingEntryRecord::authorName) &&
           challenge.size() < sizeof(ReadingEntryRecord::challengeName);
}

bool ensureDataFile() {
    ifstream existing(DATA_FILE, ios::binary);
    if (existing.good()) return true;
    existing.close();
    ofstream created(DATA_FILE, ios::binary);
    return created.good();
}

bool readRecordAt(fstream& file, streamoff position, ReadingEntryRecord& record) {
    file.clear();
    file.seekg(position, ios::beg);
    if (!file) return false;
    file.read(reinterpret_cast<char*>(&record), sizeof(record));
    return file.gcount() == static_cast<streamsize>(sizeof(record));
}

bool loadEntries(ReadingEntry entries[], int& count) {
    ifstream file(DATA_FILE, ios::binary);
    if (!file) return false;

    count = 0;
    ReadingEntryRecord record = {};
    while (file.read(reinterpret_cast<char*>(&record), sizeof(record))) {
        if (count >= MAX_BOOKS) {
            cout << "Data file exceeds the in-memory limit of " << MAX_BOOKS << " records.\n";
            return false;
        }
        if (record.totalPages < 1 || record.totalPages > 10000 ||
            record.pagesRead < 0 || record.pagesRead > record.totalPages ||
            record.entryId[0] == '\0') {
            cout << "Invalid record detected in the data file. Loading stopped.\n";
            return false;
        }
        ReadingEntry* next = &entries[count];
        next->setDetails(record.entryId, record.readerName, record.bookTitle,
                         record.authorName, record.challengeName,
                         record.pagesRead, record.totalPages);
        ++count;
    }
    return file.eof() && file.gcount() == 0;
}

// Search the binary file by fixed-record position using seekg().
streamoff findEntryPosition(fstream& file, const string& id) {
    file.clear();
    file.seekg(0, ios::end);
    streamoff fileSize = file.tellg();
    if (fileSize < 0 ||
        fileSize % static_cast<streamoff>(sizeof(ReadingEntryRecord)) != 0) return -2;

    for (streamoff pos = 0; pos < fileSize;
         pos += static_cast<streamoff>(sizeof(ReadingEntryRecord))) {
        ReadingEntryRecord record = {};
        if (!readRecordAt(file, pos, record)) return -2;
        if (id == record.entryId) return pos;
    }
    return -1;
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
    string reader = readRequiredText("Reader name: ");
    string title = readRequiredText("Book title: ");
    string author = readRequiredText("Author name: ");
    string challenge = readRequiredText("Challenge name: ");

    if (!validFieldLengths(id, reader, title, author, challenge)) {
        cout << "One or more text fields exceed binary storage capacity. Record rejected.\n";
        return;
    }

    int total = readIntInRange("Total pages in book (1-10000): ", 1, 10000);
    int read = readIntInRange("Pages read so far (0-total pages): ", 0, total);

    fstream file(DATA_FILE, ios::in | ios::out | ios::binary);
    if (!file) {
        cout << "Cannot open the binary data file.\n";
        return;
    }

    streamoff found = findEntryPosition(file, id);
    if (found == -2) {
        cout << "Data file is incomplete or corrupted. No record added.\n";
        return;
    }
    if (found >= 0) {
        cout << "Duplicate entry ID. Record was not added.\n";
        return;
    }

    ReadingEntry* next = &entries[count];
    next->setDetails(id, reader, title, author, challenge, read, total);
    ReadingEntryRecord record = {};
    if (!next->toRecord(record)) {
        next->setDetails("", "", "", "", "", 0, 0);
        cout << "Could not convert record for binary storage.\n";
        return;
    }

    file.clear();
    file.seekp(0, ios::end);
    file.write(reinterpret_cast<const char*>(&record), sizeof(record));
    file.flush();
    if (!file.good()) {
        next->setDetails("", "", "", "", "", 0, 0);
        cout << "File write failed. Record was not added to the active collection.\n";
        return;
    }
    ++count;
    cout << "Reading record saved successfully.\n";
}

void viewAllEntries(const ReadingEntry entries[], int count) {
    cout << "\n--- All Reading Records ---\n";
    if (count == 0) {
        cout << "No reading records available.\n";
        return;
    }
    for (int i = 0; i < count; ++i) displayEntry(entries[i]);
    cout << "Total records: " << count << '\n';
}

void searchEntry(const ReadingEntry entries[], int count) {
    cout << "\n--- Search Reading Record (seekg) ---\n";
    string id = readRequiredText("Enter entry ID: ");
    fstream file(DATA_FILE, ios::in | ios::binary);
    if (!file) {
        cout << "Cannot open the binary data file.\n";
        return;
    }
    streamoff position = findEntryPosition(file, id);
    if (position == -2) {
        cout << "Data file is incomplete or corrupted.\n";
        return;
    }
    if (position == -1) {
        cout << "No reading record found for ID: " << id << '\n';
        return;
    }
    int index = findEntryIndex(entries, count, id);
    if (index >= 0) {
        const ReadingEntry* result = &entries[index];
        displayEntry(*result);
    } else {
        cout << "Record exists in file but is not loaded in memory.\n";
    }
}

void updatePagesRead(ReadingEntry entries[], int count) {
    cout << "\n--- Update Reading Progress (seekp) ---\n";
    string id = readRequiredText("Enter entry ID to update: ");
    fstream file(DATA_FILE, ios::in | ios::out | ios::binary);
    if (!file) {
        cout << "Cannot open the binary data file.\n";
        return;
    }
    streamoff position = findEntryPosition(file, id);
    if (position == -2) {
        cout << "Data file is incomplete or corrupted.\n";
        return;
    }
    if (position == -1) {
        cout << "Reading record not found.\n";
        return;
    }

    ReadingEntryRecord record = {};
    if (!readRecordAt(file, position, record)) {
        cout << "Could not read the selected record.\n";
        return;
    }
    cout << "Book: " << record.bookTitle << "\nCurrent progress: "
         << record.pagesRead << " / " << record.totalPages << '\n';
    int newPagesRead = readIntInRange("New pages read (0-total pages): ",
                                      0, record.totalPages);
    if (readIntInRange("Confirm update (1 = yes, 0 = no): ", 0, 1) == 0) {
        cout << "Update cancelled.\n";
        return;
    }

    streamoff fieldOffset = position +
        static_cast<streamoff>(offsetof(ReadingEntryRecord, pagesRead));
    file.clear();
    file.seekp(fieldOffset, ios::beg);
    if (!file) {
        cout << "Could not position file for update.\n";
        return;
    }
    file.write(reinterpret_cast<const char*>(&newPagesRead), sizeof(newPagesRead));
    file.flush();
    if (!file.good()) {
        cout << "Progress update failed.\n";
        return;
    }

    int index = findEntryIndex(entries, count, id);
    if (index >= 0) {
        ReadingEntry* current = &entries[index];
        current->setDetails(record.entryId, record.readerName, record.bookTitle,
                            record.authorName, record.challengeName,
                            newPagesRead, record.totalPages);
    }
    cout << "Reading progress updated successfully.\n";
}

void showSummary(const ReadingEntry entries[], int count) {
    cout << "\n--- Reading Challenge Summary ---\n";
    if (count == 0) {
        cout << "No reading records available for summary.\n";
        return;
    }
    int completed = 0, inProgress = 0, notStarted = 0;
    long long totalPagesRead = 0, totalPagesInBooks = 0;
    for (int i = 0; i < count; ++i) {
        string status = entries[i].getStatus();
        if (status == "Completed") ++completed;
        else if (status == "In Progress") ++inProgress;
        else ++notStarted;
        totalPagesRead += entries[i].getPagesRead();
        totalPagesInBooks += entries[i].getTotalPages();
    }
    cout << "Total reading records : " << count << '\n'
         << "Completed books       : " << completed << '\n'
         << "In-progress books     : " << inProgress << '\n'
         << "Not-started books     : " << notStarted << '\n'
         << "Pages read            : " << totalPagesRead << '\n'
         << "Total book pages      : " << totalPagesInBooks << '\n'
         << "Records initialised   : " << ReadingEntry::getTotalEntries() << '\n';
}

void displayMenu() {
    cout << "\n========================================\n"
         << "     BOOK READING CHALLENGE TRACKER\n"
         << "========================================\n"
         << "1. Add reading record\n"
         << "2. View all reading records\n"
         << "3. Search record by Entry ID (seekg)\n"
         << "4. Update pages read (seekp)\n"
         << "5. View challenge summary\n"
         << "0. Exit\n"
         << "========================================\n";
}

int main() {
    if (!ensureDataFile()) {
        cerr << "Cannot create/open " << DATA_FILE << ".\n"
             << "Create the data directory in the project root first.\n";
        return 1;
    }

    ReadingEntry entries[MAX_BOOKS];
    int count = 0;
    if (!loadEntries(entries, count)) {
        cerr << "Unable to load records. Check the binary file and record capacity.\n";
        return 1;
    }

    cout << "Saved reading records loaded: " << count << '\n'
         << "Data file: " << DATA_FILE << '\n';

    int choice;
    do {
        displayMenu();
        choice = readIntInRange("Enter choice (0-5): ", 0, 5);
        switch (choice) {
            case 1: addReadingEntry(entries, count); break;
            case 2: viewAllEntries(entries, count); break;
            case 3: searchEntry(entries, count); break;
            case 4: updatePagesRead(entries, count); break;
            case 5: showSummary(entries, count); break;
            case 0: cout << "Exiting. Records remain saved in the data file.\n"; break;
        }
    } while (choice != 0);
    return 0;
}
