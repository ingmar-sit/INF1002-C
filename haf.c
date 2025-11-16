//delete + sort function by Hafiii

#include "hfile.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.c"

// =================DELETE FUNCTION==================== //

//delete Record
void deleteRecord() {
    int id;
    printf("Enter Student ID to delete: ");
    scanf("%d", &id);

    int index = findRecordIndexByID(id);
    if (index == -1) {
        printf("CMS: The record with ID=%d does not exist.\n", id);
        return;
    }

    //show record to be deleted
    printf("Record to be deleted:\n");
    printf("ID\tName\t\tProgramme\t\tMark\n");
    printf("%d\t%-10s\t%-10s\t%.2f\n",
           records[index].id,
           records[index].name,
           records[index].programme,
           records[index].mark);

    //confirm deletion
    printf("CMS: Are you sure you want to delete record with ID=%d? Type \"Y\" to Confirm or type \"N\" to cancel.\n", id);
    char confirm[10];
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

//==================SORTING FUNCTION======================//

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
    printf("ID\tName\t\tProgramme\t\tMark\n");
    for (int i = 0; i < numRecords; i++) {
        printf("%d\t%-10s\t%-10s\t%.2f\n",
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
    printf("ID\tName\t\tProgramme\t\tMark\n");
    for (int i = 0; i < numRecords; i++) {
        printf("%d\t%-10s\t%-10s\t%.2f\n",
               records[i].id,
               records[i].name,
               records[i].programme,
               records[i].mark);
    }
}