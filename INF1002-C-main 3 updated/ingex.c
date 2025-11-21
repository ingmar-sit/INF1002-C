#include "hfile.h"  // Include the header file for declarations and struct definition
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.c"


//Open file
void openDatabase() {
    loadFromFile();
    printf("CMS: The database file \"%s\" is successfully opened.\n", FILENAME);
}
/*
void loadFromFile() {
    FILE* file = fopen(FILENAME, "r");
    if (!file) {
        printf("CMS: Error opening the file.\n");
        return;
    }

    while (fscanf(file, "%d %99s %99s %f",
        &records[numRecords].id,
        records[numRecords].name,
        records[numRecords].programme,
        &records[numRecords].mark) == 4) {
        numRecords++;
    }
    fclose(file);
}
*/

void loadFromFile() {
    FILE* file = fopen(FILENAME, "r");
    if (!file) {
        printf("CMS: Error opening the file.\n");
        return;
    }

    char line[256];
    // Reset numRecords to 0 to avoid accumulation from previous loads
    numRecords = 0;

    // Skip the metadata lines (Database Name, Authors, Table Name)
    fgets(line, sizeof(line), file);  // "Database Name: P6_9 CMS"
    fgets(line, sizeof(line), file);  // "Authors: INGMAR AQIF HAFIZHAH HONG MING"
    fgets(line, sizeof(line), file);  // Blank line
    fgets(line, sizeof(line), file);  // "Table Name: StudentRecords"

    // Skip the header line (ID Name Programme Mark)
    fgets(line, sizeof(line), file);  // "ID Name Programme Mark"

    // Read each student record from the file using fixed-width format
    while (fgets(line, sizeof(line), file)) {
        if (strlen(line) < 10) continue;  // Skip empty lines

        char idStr[11], nameStr[21], progStr[31], markStr[20];

        // Extract fixed-width columns (based on your format)
        // ID: positions 0-9 (10 chars)
        // Name: positions 10-29 (20 chars)
        // Programme: positions 30-59 (30 chars)
        // Mark: positions 60+ (remaining)

        strncpy(idStr, line, 10);
        idStr[10] = '\0';

        strncpy(nameStr, line + 10, 20);
        nameStr[20] = '\0';

        strncpy(progStr, line + 30, 30);
        progStr[30] = '\0';

        strcpy(markStr, line + 60);

        // Trim trailing spaces from all fields
        int len;

        // Trim ID
        len = strlen(idStr);
        while (len > 0 && idStr[len - 1] == ' ') idStr[--len] = '\0';

        // Trim Name
        len = strlen(nameStr);
        while (len > 0 && nameStr[len - 1] == ' ') nameStr[--len] = '\0';

        // Trim Programme
        len = strlen(progStr);
        while (len > 0 && progStr[len - 1] == ' ') progStr[--len] = '\0';

        // Convert and store
        records[numRecords].id = atoi(idStr);
        strcpy(records[numRecords].name, nameStr);
        strcpy(records[numRecords].programme, progStr);
        records[numRecords].mark = atof(markStr);

        numRecords++;
    }

    fclose(file);
}

//Show all records
void showAllRecords() {
    printf("CMS: Here are all the records found in the table \"StudentRecords\".\n");
    printf("%-10s%-20s%-30s%s\n", "ID", "Name", "Programme", "Mark");
    for (int i = 0; i < numRecords; i++) {
        printf("%-10d%-20s%-30s%.2f\n", records[i].id, records[i].name, records[i].programme, records[i].mark);
    }
}

//Insert record
void insertRecord(char* command) {
    int id;
    float mark;
    char name[100], programme[100];

    // Look for each field inside the command text
    char* idPtr = strstr(command, "ID=");
    char* namePtr = strstr(command, "Name=");
    char* progPtr = strstr(command, "Programme=");
    char* markPtr = strstr(command, "Mark=");

    // Extract ID (always numeric)
    if (idPtr) {
        sscanf(idPtr + 3, "%d", &id);
    }
    else {
        printf("CMS: Missing ID field.\n");
        return;
    }

    // Check for duplicate
    if (findRecordIndexByID(id) != -1) {
        printf("CMS: The record with ID=%d already exists.\n", id);
        return;
    }

    // Extract Name (text until 'Programme=')
    if (namePtr && progPtr) {
        int len = progPtr - (namePtr + 5);
        strncpy(name, namePtr + 5, len);
        name[len] = '\0';   // terminate string
    }
    else {
        printf("CMS: Missing Name or Programme field.\n");
        return;
    }

    // Extract Programme (text until 'Mark=')
    if (progPtr && markPtr) {
        int len = markPtr - (progPtr + 10);
        strncpy(programme, progPtr + 10, len);
        programme[len] = '\0';
    }
    else {
        printf("CMS: Missing Programme or Mark field.\n");
        return;
    }

    // Extract Mark (float)
    if (markPtr) {
        sscanf(markPtr + 5, "%f", &mark);
    }
    else {
        printf("CMS: Missing Mark field.\n");
        return;
    }

    // Store parsed data
    records[numRecords].id = id;
    strcpy(records[numRecords].name, name);
    strcpy(records[numRecords].programme, programme);
    records[numRecords].mark = mark;
    numRecords++;

    printf("CMS: A new record with ID=%d is successfully inserted.\n", id);
}
