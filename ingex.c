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
    fgets(line, sizeof(line), file);  // "Database Name: Sample-CMS"
    fgets(line, sizeof(line), file);  // "Authors: Jim"
    fgets(line, sizeof(line), file);  // Blank line
    fgets(line, sizeof(line), file);  // "Table Name: StudentRecords"

    // Skip the header line (ID Name Programme Mark)
    fgets(line, sizeof(line), file);  // "ID Name Programme Mark"

    // Read each student record from the file
    while (fscanf(file, "%d\t%99[^\t]\t%99[^\t]\t%f",
        &records[numRecords].id,
        records[numRecords].name,
        records[numRecords].programme,
        &records[numRecords].mark) == 4) {
        numRecords++;  // Increment the number of records
    }

    fclose(file);
    //printf("Database loaded successfully with %d records.\n", numRecords);
}

//Show all records
void showAllRecords() {
    printf("CMS: Here are all the records found in the table \"StudentRecords\".\n");
    printf("ID\tName\t\tProgramme\t\tMark\n");
    for (int i = 0; i < numRecords; i++) {
        printf("%d\t%-10s\t%-10s\t%.2f\n", records[i].id, records[i].name, records[i].programme, records[i].mark);
    }
}
//Insert record
void insertRecord() {
    int id;
    printf("Enter Student ID: ");
    scanf("%d", &id);

    if (findRecordIndexByID(id) != -1) {
        printf("CMS: The record with ID=%d already exists.\n", id);
        return;
    }

    printf("Enter Name: ");
    scanf(" %[^\n]s", records[numRecords].name);
    printf("Enter Programme: ");
    scanf(" %[^\n]s", records[numRecords].programme);
    printf("Enter Mark: ");
    scanf("%f", &records[numRecords].mark);

    records[numRecords].id = id;
    numRecords++;

    printf("CMS: A new record with ID=%d is successfully inserted.\n", id);
}
