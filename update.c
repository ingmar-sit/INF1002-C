#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "hfile.h"
#include "common.c"

// Validation function for ID (must be all digits)
int isValidID(const char* str) {
        if (!str || !*str) return 0;

    while (*str) {
        if (*str < '0' || *str > '9') return 0;
        str++;
    }

    return 1;
}

// Validation function for Programme (no digits, only letters, spaces, and hyphens)
int isValidProgramme(const char* str) {
    if (str == NULL || *str == '\0') {
        return 0;  // Empty string
    }

    for (int i = 0; str[i] != '\0'; i++) {
        if (!isalpha(str[i]) && str[i] != ' ' && str[i] != '-') {
            return 0;  // Contains invalid character
        }
    }
    return 1;  // Valid
}

// Validation function for Name (no digits, only letters, spaces, and hyphens)
int isValidName(const char* str) {
    if (str == NULL || *str == '\0') {
        return 0;  // Empty string
    }

    for (int i = 0; str[i] != '\0'; i++) {
        if (!isalpha(str[i]) && str[i] != ' ' && str[i] != '-') {
            return 0;  // Contains invalid character
        }
    }
    return 1;  // Valid
}

// Validation function for Mark (must be a valid float)
int isValidMark(const char* str) {
    if (str == NULL || *str == '\0') {
        return 0;  // Empty string
    }

    char* endptr;
    float mark = strtof(str, &endptr);

    // Check if conversion was successful and entire string was consumed
    if (*endptr != '\0' && *endptr != ' ' && *endptr != '\n') {
        return 0;  // Invalid float
    }

    // Optional: Check mark range (e.g., 0-100)
    if (mark < 0.0f || mark > 100.0f) {
        return 0;  // Out of valid range
    }

    return 1;  // Valid
}

// Update record with input validation
void updateRecord(char* input) {
    int id;
    char idStr[20];
    char markStr[20];

    input[strcspn(input, "\n")] = 0;

    // Parse ID
    char* idToken = strstr(input, "ID=");
    if (!idToken) {
        printf("CMS: Invalid command format.\n");
        return;
    }

    // Extract ID string for validation
    sscanf(idToken + 3, "%19s", idStr);

    // Validate ID
    if (!isValidID(idStr)) {
        printf("CMS: Invalid ID format. ID must contain only numbers.\n");
        return;
    }

    // Convert to integer after validation
    id = atoi(idStr);

    // Find record
    int index = findRecordIndexByID(id);
    if (index == -1) {
        printf("CMS: The record with ID=%d does not exist.\n", id);
        return;
    }

    // Update field based on what's provided
    char* token;

    if ((token = strstr(input, "Mark="))) {
        // Extract mark string for validation
        sscanf(token + 5, "%19s", markStr);

        // Validate mark
        if (!isValidMark(markStr)) {
            printf("CMS: Invalid Mark format. Mark must be a number between 0 and 100.\n");
            return;
        }

        sscanf(token + 5, "%f", &records[index].mark);
    }
    else if ((token = strstr(input, "Programme="))) {
        char programme[100];
        sscanf(token + 10, "%99[^\n]", programme);

        // Remove trailing spaces
        int len = strlen(programme);
        while (len > 0 && programme[len - 1] == ' ') {
            programme[--len] = '\0';
        }

        // Validate programme
        if (!isValidProgramme(programme)) {
            printf("CMS: Invalid Programme name. Programme must contain only letters, spaces, and hyphens.\n");
            return;
        }

        strcpy(records[index].programme, programme);
    }
    else if ((token = strstr(input, "Name="))) {
        char name[100];
        sscanf(token + 5, "%99[^\n]", name);

        // Remove trailing spaces
        int len = strlen(name);
        while (len > 0 && name[len - 1] == ' ') {
            name[--len] = '\0';
        }

        // Validate name
        if (!isValidName(name)) {
            printf("CMS: Invalid Name format. Name must contain only letters, spaces, and hyphens.\n");
            return;
        }

        strcpy(records[index].name, name);
    }
    else {
        printf("CMS: No field specified to update.\n");
        return;
    }

    printf("CMS: The record with ID=%d is successfully updated.\n", id);
}

// Save

void saveDatabase() {
    FILE* file = fopen(FILENAME, "w");
    if (!file) {
        printf("CMS: Error saving the file.\n");
        return;
    }

    // Write the metadata
    fprintf(file, "Database Name: Sample-CMS\n");
    fprintf(file, "Authors: Jim\n");
    fprintf(file, "\n");
    fprintf(file, "Table Name: StudentRecords\n");

    // Write the table headers
    fprintf(file, "%-10s%-20s%-30s%s\n", "ID", "Name", "Programme", "Mark");

    // Write each student record
    for (int i = 0; i < numRecords; i++) {
        fprintf(file, "%-10d%-20s%-30s%.2f\n",
            records[i].id,
            records[i].name,
            records[i].programme,
            records[i].mark);
    }

    fclose(file);
}
