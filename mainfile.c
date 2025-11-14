#include "hfile.h"  // Include the header file for declarations and struct definition
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "haf.c"
#include "ing.c"
#include "ingex.c"
#include "aq.c"
#include "common.c"

// Global variables
StudentRecord records[MAX_RECORDS];
int numRecords = 0;

int main() {
    char command[100];

    // Load data from file when the program starts
    loadFromFile();

    while (1) {
        printf("P6_9: ");
        scanf(" %[^\n]s", command);  // Read entire line

        if (strcmp(command, "OPEN") == 0) {
            openDatabase();
        }
        else if (strcmp(command, "SHOW ALL") == 0) {
            showAllRecords();
        }
        else if (strncmp(command, "INSERT", 6) == 0) {
            insertRecord();
        }
        else if (strncmp(command, "QUERY", 5) == 0) {
            queryRecord();
        }
        else if (strncmp(command, "UPDATE", 6) == 0) {
            updateRecord();
        }
        else if (strncmp(command, "DELETE", 6) == 0) {
            deleteRecord();
        }
        else if (strcmp(command, "SAVE") == 0) {
            saveDatabase();
        }
        else if (strncmp(command, "SHOW ALL SORT BY ID", 19) == 0) {
            sortByID(0);  // Ascending by default
        }
        else if (strncmp(command, "SHOW ALL SORT BY MARK", 21) == 0) {
            sortByMark(0);  // Ascending by default
        }
        else if (strcmp(command, "SHOW SUMMARY") == 0) {
            showSummary();
        }
        else {
            printf("Invalid command.\n");
        }
    }

    return 0;
}