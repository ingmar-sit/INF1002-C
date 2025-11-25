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
    if (strcmp(command, "SHOW SUMMARY") == 0) return 12;
    if (strcmp(command, "SHOW ALL WITH GRADES") == 0) return 13;

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
    if (strncmp(command, "SHOW GPA ID", 11) == 0) return 14;

    return 0;  // Invalid command
}

int main() {
    char command[100];

    printf("\n==============================================================\n");
    printf("                        DECLARATION                           \n");
    printf("==============================================================\n");
    printf("SIT's policy on copying does not allow the students to copy\n");
    printf("source code as well as assessment solutions from another person\n");
    printf("AI or other places. It is the students' responsibility to\n");
    printf("guarantee that their assessment solutions are their own work.\n");
    printf("Meanwhile, the students must also ensure that their work is not\n");
    printf("accessible by others. Where such plagiarism is detected, both\n");
    printf("of the assessments involved will receive ZERO mark.\n\n");

    printf("We hereby declare that:\n");
    printf("• We fully understand and agree to the abovementioned plagiarism policy.\n");
    printf("• We did not copy any code from others or from other places.\n");
    printf("• We did not share our codes with others or upload to any other places\n");
    printf("  for public access and will not do that in the future.\n");
    printf("• We agree that our project will receive Zero mark if there is any\n");
    printf("  plagiarism detected.\n");
    printf("• We agree that we will not disclose any information or material of\n");
    printf("  the group project to others or upload to any other places for public access.\n");
    printf("• We agree that we did not copy any code directly from AI generated sources\n\n");

    printf("Declared by: Group Name P6_9\n\n");
    printf("Team members:\n");
    printf("1. HAFIZHAH MUNIRAH BINTE MAZLAN\n");
    printf("2. MUHAMMAD AQIF BIN NOORAZMAN\n");
    printf("3. ONG SHI JIE INGMAR\n\n");
    printf("Date: 25/11/2025\n");
    printf("==============================================================\n\n");

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