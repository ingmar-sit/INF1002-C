#ifndef HFILE_H
#define HFILE_H

#define MAX_RECORDS 100
#define FILENAME "Sample-CMS.txt"

// Struct to hold student data
typedef struct {
    int id;
    char name[100];
    char programme[100];
    float mark;
} StudentRecord;

// Global array to hold records
extern StudentRecord records[MAX_RECORDS];
extern int numRecords;

// Function declarations (prototypes)
void openDatabase();
void showAllRecords();
void insertRecord();
void queryRecord();
void updateRecord();
void deleteRecord();
void saveDatabase();
void sortByID(int descending);
void sortByMark(int descending);
void showSummary();
int findRecordIndexByID(int id);
void loadFromFile();
void saveToFile();

#endif // HFILE_H