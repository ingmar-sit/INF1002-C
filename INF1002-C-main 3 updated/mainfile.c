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

int main() {
    char command[100];

    // Load data from file when the program starts
    loadFromFile();

    while (1) {
        printf("P6_9: ");
        scanf(" %[^\n]", command);  // Read entire line

        //Open command
        if (strcmp(command, "OPEN") == 0) {
            openDatabase();
        }
        //Show all command
        else if (strcmp(command, "SHOW ALL") == 0) {
            showAllRecords();
        }
        //Insert command
        else if (strncmp(command, "INSERT", 6) == 0) {
            insertRecord(command);
        }
        //query command
        else if (strncmp(command, "QUERY", 5) == 0) {
            queryRecord(command);
        }
        //update command
        else if (strncmp(command, "UPDATE", 6) == 0) {
            updateRecord(command);
        }
        //delete command
        else if (strncmp(command, "DELETE", 6) == 0) {
            deleteRecord(command);
        }
        //save command
        else if (strcmp(command, "SAVE") == 0) {
            saveDatabase();
        }
        //sorting command
        else if (strcmp(command, "SHOW ALL SORT BY ID") == 0 ||
                 strcmp(command, "SHOW ALL SORT BY ID ASCENDING") == 0) {
            sortByID(0); // 0=ascending
        }
        else if (strcmp(command, "SHOW ALL SORT BY ID DESCENDING") == 0) {
            sortByID(1); // 1=descending
        }
        else if (strcmp(command, "SHOW ALL SORT BY MARK") == 0 ||
                 strcmp(command, "SHOW ALL SORT BY MARK ASCENDING") == 0) {
            sortByMark(0);  // 0=ascending
        }
        else if (strcmp(command, "SHOW ALL SORT BY MARK DESCENDING") == 0) {
            sortByMark(1); // 1=descending
        }
        else if (strcmp(command, "SHOW ALL SORT BY NAME") == 0 ||
                 strcmp(command, "SHOW ALL SORT BY NAME ASCENDING") == 0) {
            sortByName(0);  // 0=ascending
                 }
        else if (strcmp(command, "SHOW ALL SORT BY NAME DESCENDING") == 0) {
            sortByName(1); // 1=descending
        }
        //summary command
        else if (strcmp(command, "SHOW SUMMARY") == 0) {
            showSummary();
        }
        else if (strcmp(command, "SHOW ALL WITH GRADES") == 0) {
            showAllWithGrades();
        }
        else if (strncmp(command, "SHOW GPA ID=", 12) == 0) {
            showGPAForID(command);
        }
        else {
            printf("Invalid command.\n");
        }
    }

    return 0;
}