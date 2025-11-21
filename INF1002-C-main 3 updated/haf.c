//delete + sort + unique gpa function by Hafiii

#include "hfile.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.c"

// =================DELETE FUNCTION==================== //

//delete Record
void deleteRecord(char* command) {
    int id;
    char* idPtr = strstr(command, "ID=");
    if (idPtr != NULL) {
        if (sscanf(idPtr + 3, "%d", &id) !=1) {
            printf("CMS: Invalid input. Please provide an ID number.\n");
            return;}
    }
    else {
        printf("CMS: Missing ID field.\n");
        return;
    }

    int index = findRecordIndexByID(id);
    if (index == -1) {
        printf("CMS: The record with ID=%d does not exist.\n", id);
        return;
    }

    //show record to be deleted, so user knows info to delete
    printf("Record to be deleted:\n");
    printf("%-10s%-20s%-30s%s\n", "ID", "Name", "Programme", "Mark");
    printf("%-10d%-20s%-30s%.2f\n",
           records[index].id,
           records[index].name,
           records[index].programme,
           records[index].mark);

    //confirm deletion
    printf("CMS: Are you sure you want to delete record with ID=%d? Type \"Y\" to Confirm or type \"N\" to cancel.\n", id);
    char confirm[10];
    printf("P6_9: ");
    fflush(stdout);  //ensure prompt is displayed immediately
    scanf(" %[^\n]s", confirm);

    if (confirm[0] == 'Y' || confirm[0] == 'y') {
        // Shift all records after the deleted one
        for (int i = index; i < numRecords - 1; i++) {
            records[i] = records[i + 1];
        }
        numRecords--;
        printf("CMS: The record with ID=%d is successfully deleted.\n", id);
    } else {
        printf("CMS: The deletion is cancelled.\n");
    }
}

//==================SORTING FUNCTION===========================//

//comparison function for sorting by ID (ascending)
int compareByIDAsc(const void* a, const void* b) {
    StudentRecord* recA = (StudentRecord*)a;
    StudentRecord* recB = (StudentRecord*)b;
    return (recA->id - recB->id);
}

//comparison function for sorting by ID (descending)
int compareByIDDesc(const void* a, const void* b) {
    StudentRecord* recA = (StudentRecord*)a;
    StudentRecord* recB = (StudentRecord*)b;
    return (recB->id - recA->id);
}

//comparison function for sorting by Mark (ascending)
int compareByMarkAsc(const void* a, const void* b) {
    StudentRecord* recA = (StudentRecord*)a;
    StudentRecord* recB = (StudentRecord*)b;
    if (recA->mark < recB->mark) return -1;
    if (recA->mark > recB->mark) return 1;
    return 0;
}

//comparison function for sorting by Mark (descending)
int compareByMarkDesc(const void* a, const void* b) {
    StudentRecord* recA = (StudentRecord*)a;
    StudentRecord* recB = (StudentRecord*)b;
    if (recB->mark < recA->mark) return -1;
    if (recB->mark > recA->mark) return 1;
    return 0;
}

//sort by ID
void sortByID(int descending) {
    if (numRecords == 0) {
        printf("CMS: No records to sort.\n");
        return;
    }

    //sort the records array
    if (descending) {
        qsort(records, numRecords, sizeof(StudentRecord), compareByIDDesc);
    } else {
        qsort(records, numRecords, sizeof(StudentRecord), compareByIDAsc);
    }

    //display sorted records
    printf("CMS: Here are all the records sorted by ID (%s).\n",
           descending ? "descending" : "ascending");
    printf("%-10s%-20s%-30s%s\n", "ID", "Name", "Programme", "Mark");
    for (int i = 0; i < numRecords; i++) {
        printf("%-10d%-20s%-30s%.2f\n",
               records[i].id,
               records[i].name,
               records[i].programme,
               records[i].mark);
    }
}

//sort by Mark
void sortByMark(int descending) {
    if (numRecords == 0) {
        printf("CMS: No records to sort.\n");
        return;
    }

    //sort the records array
    if (descending) {
        qsort(records, numRecords, sizeof(StudentRecord), compareByMarkDesc);
    } else {
        qsort(records, numRecords, sizeof(StudentRecord), compareByMarkAsc);
    }

    //display sorted records
    printf("CMS: Here are all the records sorted by Mark (%s).\n",
           descending ? "descending" : "ascending");
    printf("%-10s%-20s%-30s%s\n", "ID", "Name", "Programme", "Mark");
    for (int i = 0; i < numRecords; i++) {
        printf("%-10d%-20s%-30s%.2f\n",
               records[i].id,
               records[i].name,
               records[i].programme,
               records[i].mark);
    }
}
//comparison function for sorting by Name (ascending A-Z)
int compareByNameAsc(const void* a, const void* b) {
    StudentRecord* recA = (StudentRecord*)a;
    StudentRecord* recB = (StudentRecord*)b;
    return strcmp(recA->name, recB->name);
}

//comparison function for sorting by Name (descending Z-A)
int compareByNameDesc(const void* a, const void* b) {
    StudentRecord* recA = (StudentRecord*)a;
    StudentRecord* recB = (StudentRecord*)b;
    // Note the swap of A and B here to reverse the order
    return strcmp(recB->name, recA->name);
}

//sort by Name
void sortByName(int descending) {
    if (numRecords == 0) {
        printf("CMS: No records to sort.\n");
        return;
    }

    //sort the records array
    if (descending) {
        qsort(records, numRecords, sizeof(StudentRecord), compareByNameDesc);
    } else {
        qsort(records, numRecords, sizeof(StudentRecord), compareByNameAsc);
    }

    //display sorted records
    printf("CMS: Here are all the records sorted by Name (%s).\n",
           descending ? "descending" : "ascending");
    printf("%-10s%-20s%-30s%s\n", "ID", "Name", "Programme", "Mark");
    for (int i = 0; i < numRecords; i++) {
        printf("%-10d%-20s%-30s%.2f\n",
               records[i].id,
               records[i].name,
               records[i].programme,
               records[i].mark);
    }
}
//==================GPA FUNCTIONSSSSS==================================//

/* Converts a numerical mark to a 4.0 GPA scale.
 * * * Based on the grading scale:
 * * >= 85.0: 4.0 (A)
 * * >= 80.0: 3.7 (A-)
 * * >= 77.0: 3.3 (B+)
 * * >= 73.0: 3.0 (B)
 * * >= 69.0: 2.7 (B-) <-- Fits 69.80 example
 * * >= 66.0: 2.3 (C+)
 * * >= 63.0: 2.0 (C) <-- Fits 63.40 example
 * * >= 60.0: 1.7 (C-)
 * * >= 50.0: 1.0 (D)
 * * < 50.0: 0.0 (F) */

float getGPAFromMark(float mark) {
    if (mark >= 85.0) return 4.0;
    if (mark >= 80.0) return 3.7;
    if (mark >= 77.0) return 3.3;
    if (mark >= 73.0) return 3.0;
    if (mark >= 69.0) return 2.7;
    if (mark >= 66.0) return 2.3;
    if (mark >= 63.0) return 2.0;
    if (mark >= 60.0) return 1.7;
    if (mark >= 50.0) return 1.0;
    return 0.0;
}

//Uses the same scale as getGPAFromMark. //
const char* getGradeFromMark(float mark) {
    if (mark >= 85.0) return "A";
    if (mark >= 80.0) return "A-";
    if (mark >= 77.0) return "B+";
    if (mark >= 73.0) return "B";
    if (mark >= 69.0) return "B-";
    if (mark >= 66.0) return "C+";
    if (mark >= 63.0) return "C";
    if (mark >= 60.0) return "C-";
    if (mark >= 50.0) return "D";
    return "F";
}

// Implements the "SHOW ALL WITH GRADES" command.
//Displays all student records with their corresponding
 //letter grade, GPA, and the class average GPA.
void showAllWithGrades() {
    if (numRecords == 0) {
        printf("CMS: No records found.\n");
        return;
    }

    printf("CMS: Here are all records with letter grades and GPA.\n");
    // New header with Grade and GPA column//
    printf("%-10s%-20s%-30s%-10s%-8s%s\n", "ID", "Name", "Programme", "Mark", "Grade", "GPA");

    float totalGPA = 0;
    for (int i = 0; i < numRecords; i++) {
        float mark = records[i].mark;
        const char* grade = getGradeFromMark(mark);
        float gpa = getGPAFromMark(mark);
        totalGPA += gpa;

        // Print the new row format //
        printf("%-10d%-20s%-30s%-10.2f%-8s%.1f\n",
            records[i].id,
            records[i].name,
            records[i].programme,
            mark,
            grade,
            gpa);
    }

    // Print the class average GPA//
    if (numRecords > 0) {
        printf("Class Average GPA: %.2f\n", totalGPA / numRecords);
    }
}

// Implements the "SHOW GPA ID=..." command.
// Parses the ID from the input string and displays
//the grade and GPA for that specific student.
void showGPAForID(char* input) {
    int id;

    // Parse the ID from the command "SHOW GPA ID=..."//
    if (sscanf(input, "SHOW GPA ID=%d", &id) != 1) {
        printf("CMS: Invalid command format. Use SHOW GPA ID=1234567\n");
        return;
    }

    int index = findRecordIndexByID(id);
    if (index == -1) {
        printf("CMS: The record with ID=%d does not exist.\n", id);
        return;
    }

    // Get the data for the specific student//
    float mark = records[index].mark;
    const char* grade = getGradeFromMark(mark);
    float gpa = getGPAFromMark(mark);

    printf("CMS: Grade and GPA for Student ID=%d\n", id);
    printf("Name: %s\n", records[index].name);
    printf("Mark: %.2f\n", mark);
    printf("Grade: %s\n", grade);
    printf("GPA: %.1f\n", gpa);
}