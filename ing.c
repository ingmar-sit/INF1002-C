#include "hfile.h"  // Include the header file for declarations and struct definition
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.c"


//Query Record
void queryRecord() {
    int id;
    printf("Enter Student ID to query: ");
    scanf("%d", &id);

    int index = findRecordIndexByID(id);
    if (index == -1) {
        printf("CMS: The record with ID=%d does not exist.\n", id);
    }
    else {
        printf("CMS: The record with ID=%d is found in the data table.\n", id);
        printf("ID\tName\t\tProgramme\t\tMark\n");
        printf("%d\t%-10s\t%-10s\t%.2f\n", records[index].id, records[index].name, records[index].programme, records[index].mark);
    }
}

//Summary Statistics
void showSummary() {
    if (numRecords == 0) {
        printf("CMS: No records found.\n");
        return;
    }

    int totalStudents = numRecords;
    float totalMarks = 0;
    float highestMark = records[0].mark, lowestMark = records[0].mark;
    char highestName[100], lowestName[100];

    for (int i = 0; i < numRecords; i++) {
        totalMarks += records[i].mark;
        if (records[i].mark > highestMark) {
            highestMark = records[i].mark;
            strcpy(highestName, records[i].name);
        }
        if (records[i].mark < lowestMark) {
            lowestMark = records[i].mark;
            strcpy(lowestName, records[i].name);
        }
    }

    float avgMark = totalMarks / totalStudents;

    printf("CMS: Summary\n");
    printf("Total students: %d\n", totalStudents);
    printf("Average mark: %.2f\n", avgMark);
    printf("Highest mark: %.2f (Student: %s)\n", highestMark, highestName);
    printf("Lowest mark: %.2f (Student: %s)\n", lowestMark, lowestName);
}
