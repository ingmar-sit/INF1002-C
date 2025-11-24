#include "hfile.h"  // Include the header file for declarations and struct definition
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.c" //helper functions
#include "haf.c"    //delete + sorting
#include "ing.c"    // query + sumary
#include "ingex.c"  //open, show all + insert
#include "aq.c"    // update + save

// Global variables
StudentRecord records[MAX_RECORDS];
int numRecords = 0;

// Helper function to convert command string to command type
int getCommandType(const char* command) {
    // Exact match commands
    if (strcmp(command, "OPEN") == 0) return 1;
    if (strcmp(command, "SHOW ALL") == 0) return 2;
    if (strcmp(command, "SAVE") == 0) return 7;
    if (strcmp(command, "SHOW SUMMARY") == 0) return 11;
    if (strcmp(command, "SHOW ALL WITH GRADES") == 0) return 12;

    // Sorting commands
    if (strcmp(command, "SHOW ALL SORT BY ID") == 0 ||
        strcmp(command, "SHOW ALL SORT BY ID ASCENDING") == 0) return 8;
    if (strcmp(command, "SHOW ALL SORT BY ID DESCENDING") == 0) return 9;
    if (strcmp(command, "SHOW ALL SORT BY MARK") == 0 ||
        strcmp(command, "SHOW ALL SORT BY MARK ASCENDING") == 0) return 10;
    if (strcmp(command, "SHOW ALL SORT BY MARK DESCENDING") == 0) return 11;

    // Partial match commands
    if (strncmp(command, "INSERT", 6) == 0) return 3;
    if (strncmp(command, "QUERY", 5) == 0) return 4;
    if (strncmp(command, "UPDATE", 6) == 0) return 5;
    if (strncmp(command, "DELETE", 6) == 0) return 6;
    if (strncmp(command, "SHOW GPA ID=", 12) == 0) return 13;

    return 0;  // Invalid command
}

int main() {
    char command[100];

    loadFromFile();
    while (1) {
        printf("P6_9: ");
        scanf(" %[^\n]", command);  // Read entire line

        int cmdType = getCommandType(command);

        switch (cmdType) {
        case 1:  // OPEN
            openDatabase();
            break;

        case 2:  // SHOW ALL
            showAllRecords();
            break;

        case 3:  // INSERT
            insertRecord(command);
            break;

        case 4:  // QUERY
            queryRecord(command);
            break;

        case 5:  // UPDATE
            updateRecord(command);
            break;

        case 6:  // DELETE
            deleteRecord(command);
            break;

        case 7:  // SAVE
            saveDatabase();
            break;

        case 8:  // SORT BY ID ASCENDING
            sortByID(0);
            break;

        case 9:  // SORT BY ID DESCENDING
            sortByID(1);
            break;

        case 10:  // SORT BY MARK ASCENDING
            sortByMark(0);
            break;

        case 11:  // SORT BY MARK DESCENDING
            sortByMark(1);
            break;

        case 12:  // SHOW SUMMARY
            showSummary();
            break;

        case 13:  // SHOW ALL WITH GRADES
            showAllWithGrades();
            break;

        case 14:  // SHOW GPA ID=
            showGPAForID(command);
            break;

        default:  // Invalid command
            printf("Invalid command.\n");
            break;
        }
    }

    return 0;
}