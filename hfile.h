#ifndef HFILE_H
#define HFILE_H

#define MAX_RECORDS 100
#define FILENAME "P6_9-CMS.txt"

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
void insertRecord(char* command);
void queryRecord(char* command);
void updateRecord(char* input);
void deleteRecord(char* command);
void saveDatabase();
void sortByID(int descending);
void sortByMark(int descending);
void showSummary();
int findRecordIndexByID(int id);
int loadFromFile();
void saveToFile();
void showAllWithGrades();
void showGPAForID(char* input);
float getGPAFromMark(float mark);
const char* getGradeFromMark(float mark);
int isValidID(const char* str);
int isValidProgramme(const char* str);
int isValidName(const char* str);
int isValidMark(const char* str);

#endif // HFILE_H