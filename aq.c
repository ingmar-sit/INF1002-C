#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "hfile.h"
#include "common.c"

/**
 * isValidID - Validates that ID contains only numeric digits
 * @str: Input string to validate
 * Returns: 1 if valid (all digits), 0 if invalid or NULL/empty
 *
 * Purpose: Ensures student ID is a positive integer without special chars
 */
int isValidID(const char* str) {
    if (!str || !*str) return 0;

    while (*str) {
        //Accept values between 0 to 9 for the student id
        if (*str < '0' || *str > '9') return 0;
        str++;
    }
    return 1;
}

/**
 * isValidProgramme - Validates programme name string
 * @str: Input string to validate
 * Returns: 1 if valid, 0 if invalid or empty
 *
 * Rules: Only letters, spaces, and hyphens allowed
 *        No digits or special characters
 *
 * Examples: "Computer Science", "Applied-AI"
 */
int isValidProgramme(const char* str) {
    if (str == NULL || *str == '\0') {
        return 0;  //
    }

    for (int i = 0; str[i] != '\0'; i++) {
        // Reject if character is not alphabetic, space, or hyphen
        if (!isalpha(str[i]) && str[i] != ' ' && str[i] != '-') {
            return 0;
        }
    }
    return 1; 
}

/**
 * isValidName - Validates student name string
 * @str: Input string to validate
 * Returns: 1 if valid, 0 if invalid or empty
 *
 * Rules: Only letters, spaces, and hyphens allowed
 *        No digits or special characters
 *
 * Examples: "John Doe", "Mary-Jane"
 */
int isValidName(const char* str) {
    if (str == NULL || *str == '\0') {
        return 0;
    }

    for (int i = 0; str[i] != '\0'; i++) {
        // Reject if character is not alphabetic, space, or hyphen
        if (!isalpha(str[i]) && str[i] != ' ' && str[i] != '-') {
            return 0;
        }
    }
    return 1;
}

/**
 * isValidMark - Validates student mark (grade) value
 * @str: Input string to validate
 * Returns: 1 if valid, 0 if invalid or out of range
 *
 * Rules: Must be a valid floating-point number
 *        Must be in range [0.0, 100.0]
 *
 * Examples: "85.5", "100", "101.5", "abc"
 */
int isValidMark(const char* str) {
    if (str == NULL || *str == '\0') {
        return 0;  // Empty string
    }

    char* endptr;
    float mark = strtof(str, &endptr);

    // Check if entire string was successfully converted to float
    // (no garbage characters remaining)
    if (*endptr != '\0' && *endptr != ' ' && *endptr != '\n') {
        return 0;
    }

    // Verify mark is within acceptable range [0, 100]
    if (mark < 0.0f || mark > 100.0f) {
        return 0; 
    }

    return 1;  // Valid
}

 /**
  * extractFieldValue - Extracts field value until next field or end of string
  * Uses simple character-by-character copy with space detection
  */
void extractFieldValue(char* token, int offset, char* buffer) {
    char* src = token + offset;
    int i = 0;

    // Copy until space followed by field keyword, or end of string
    while (*src && i < 99) {
        // Look ahead: if this is a space, check if next starts with field keyword
        if (*src == ' ' && (i > 0)) {  // only check if we've copied at least 1 char
            // Peek at what comes after the space
            char* peek = src + 1;

            // Check if it starts with any field keyword
            if (strncmp(peek, "Name=", 5) == 0 ||
                strncmp(peek, "name=", 5) == 0 ||
                strncmp(peek, "Programme=", 10) == 0 ||
                strncmp(peek, "programme=", 10) == 0 ||
                strncmp(peek, "Mark=", 5) == 0 ||
                strncmp(peek, "mark=", 5) == 0) {
                break;
            }
        }

        buffer[i++] = *src++;
    }
    buffer[i] = '\0';

    // Trim trailing spaces
    while (i > 0 && buffer[i - 1] == ' ') {
        buffer[--i] = '\0';
    }
}
// ============================================================
// UPDATE RECORD FUNCTION
// ============================================================
/**
 * updateRecord - Updates existing student record fields
 * @input: Command string in format "ID=<id> [Name=<name>] [Programme=<prog>] [Mark=<mark>]"
 *
 * Supported fields: Name, Programme, Mark (case-insensitive)
 *
 * Features:
 * - Handles fields in any order (preserves input order during updates)
 * - Validates all input before modifying records
 * - Supports single or multiple field updates in one command
 * - Uses macro-based approach to reduce code duplication
 * - Returns detailed error messages for invalid data
 *
 * Validation rules:
 * - ID: Must be numeric and exist in database
 * - Name: Letters, spaces, and hyphens only (no digits)
 * - Programme: Letters, spaces, and hyphens only (no digits)
 * - Mark: Float value between 0-100
 *
 * Implementation uses case_type switching to handle field ordering:
 * - Determines which fields are present and their order in input
 * - Uses pointer comparison to find field positions efficiently
 * - Executes updates in the exact order they appear in command
 * - Early returns on validation failure to prevent partial updates
 *
 * Cases 1-3: Single field updates (Name, Programme, or Mark only)
 * Cases 4-9: Two field updates (all combinations of ordering)
 * Cases 10-15: Three field updates (all permutations of Name, Programme, Mark)
 */
void updateRecord(char* input) {
    int id, index;
    char idStr[20];
    char buffer[100];

    // Remove trailing newline
    input[strcspn(input, "\n")] = 0;

    // ===== PARSE AND VALIDATE ID =====
    char* idToken = strstr(input, "ID=");
    if (!idToken) {
        printf("CMS: Invalid command format.\n");
        return;
    }
    sscanf(idToken + 3, "%19s", idStr);
    if (!isValidID(idStr)) {
        printf("CMS: Invalid ID format. ID must contain only numbers.\n");
        return;
    }
    id = atoi(idStr);

    // ===== CHECK IF RECORD EXISTS =====
    index = findRecordIndexByID(id);
    if (index == -1) {
        printf("CMS: The record with ID=%d does not exist.\n", id);
        return;
    }

    // ===== LOCATE ALL FIELD TOKENS =====
    char* nameToken = strstr(input, "Name=");
    if (!nameToken) nameToken = strstr(input, "name=");

    char* progToken = strstr(input, "Programme=");
    if (!progToken) progToken = strstr(input, "programme=");

    char* markToken = strstr(input, "Mark=");
    if (!markToken) markToken = strstr(input, "mark=");

    // ===== DETERMINE FIELD ORDER =====
    int case_type = 0;
    int field_count = (nameToken != NULL) + (progToken != NULL) + (markToken != NULL);

    if (field_count == 0) {
        printf("CMS: No field specified to update.\n");
        return;
    }
    else if (field_count == 1) {
        if (nameToken) case_type = 1;
        else if (progToken) case_type = 2;
        else case_type = 3;
    }
    else if (field_count == 2) {
        if (nameToken && progToken) {
            case_type = (nameToken < progToken) ? 4 : 5;
        }
        else if (nameToken && markToken) {
            case_type = (nameToken < markToken) ? 6 : 7;
        }
        else {
            case_type = (progToken < markToken) ? 8 : 9;
        }
    }
    else { // field_count == 3
        char* fields[3] = { nameToken, progToken, markToken };
        int indices[3] = { 0, 1, 2 };

        for (int i = 0; i < 2; i++) {
            for (int j = i + 1; j < 3; j++) {
                if (fields[j] < fields[i]) {
                    char* tmp = fields[i];
                    fields[i] = fields[j];
                    fields[j] = tmp;
                    int t = indices[i];
                    indices[i] = indices[j];
                    indices[j] = t;
                }
            }
        }

        if (indices[0] == 0 && indices[1] == 1 && indices[2] == 2) case_type = 10;
        else if (indices[0] == 0 && indices[1] == 2 && indices[2] == 1) case_type = 11;
        else if (indices[0] == 1 && indices[1] == 0 && indices[2] == 2) case_type = 12;
        else if (indices[0] == 1 && indices[1] == 2 && indices[2] == 0) case_type = 13;
        else if (indices[0] == 2 && indices[1] == 0 && indices[2] == 1) case_type = 14;
        else case_type = 15;
    }

    // ===== MACRO FOR EFFICIENT FIELD UPDATES =====
#define UPDATE_NAME \
    extractFieldValue(nameToken, 5, buffer); \
    if (!isValidName(buffer)) { \
        printf("CMS: Invalid Name. Name must contain only letters, spaces, and hyphens.\n"); \
        return; \
    } \
    strcpy(records[index].name, buffer)

#define UPDATE_PROGRAMME \
    extractFieldValue(progToken, 10, buffer); \
    if (!isValidProgramme(buffer)) { \
        printf("CMS: Invalid Programme name. Programme must contain only letters, spaces, and hyphens.\n"); \
        return; \
    } \
    strcpy(records[index].programme, buffer)

#define UPDATE_MARK \
    extractFieldValue(markToken, 5, buffer); \
    if (!isValidMark(buffer)) { \
        printf("CMS: Invalid Mark format. Mark must be a number between 0 and 100.\n"); \
        return; \
    } \
    sscanf(markToken + 5, "%f", &records[index].mark)

    // ===== EXECUTE UPDATES BASED ON FIELD ORDER =====
    switch (case_type) {
    case 1: UPDATE_NAME; break;
    case 2: UPDATE_PROGRAMME; break;
    case 3: UPDATE_MARK; break;
    case 4: UPDATE_NAME; UPDATE_PROGRAMME; break;
    case 5: UPDATE_PROGRAMME; UPDATE_NAME; break;
    case 6: UPDATE_NAME; UPDATE_MARK; break;
    case 7: UPDATE_MARK; UPDATE_NAME; break;
    case 8: UPDATE_PROGRAMME; UPDATE_MARK; break;
    case 9: UPDATE_MARK; UPDATE_PROGRAMME; break;
    case 10: UPDATE_NAME; UPDATE_PROGRAMME; UPDATE_MARK; break;
    case 11: UPDATE_NAME; UPDATE_MARK; UPDATE_PROGRAMME; break;
    case 12: UPDATE_PROGRAMME; UPDATE_NAME; UPDATE_MARK; break;
    case 13: UPDATE_PROGRAMME; UPDATE_MARK; UPDATE_NAME; break;
    case 14: UPDATE_MARK; UPDATE_NAME; UPDATE_PROGRAMME; break;
    case 15: UPDATE_MARK; UPDATE_PROGRAMME; UPDATE_NAME; break;
    }

    printf("CMS: The record with ID=%d is successfully updated.\n", id);

#undef UPDATE_NAME
#undef UPDATE_PROGRAMME
#undef UPDATE_MARK
}
// ============================================================
// SAVE DATABASE FUNCTION
// ============================================================

/**
 * saveDatabase - Writes all student records to file
 *
 * File format: Fixed-width columns with metadata header
 * - Metadata: Database name, authors, table name
 * - Column headers: ID, Name, Programme, Mark
 * - Data rows: Student records with proper alignment
 *
 * File location: Defined by FILENAME constant (Our case would be "P6_9-CMS.txt")
 *
 * Note: Uses fprintf() with format specifiers for fixed-width formatting
 *       %-10d = 10-char field for ID (left-aligned)
 *       %-20s = 20-char field for Name
 *       %-30s = 30-char field for Programme
 *       %.2f = Mark with 2 decimal places
 */
void saveDatabase() {
    // 1. Check if there are records to save
    if (numRecords == 0) {
        printf("CMS: No records to save.\n");
        return;
    }

    // 2. Try to open the file
    FILE* file = fopen(FILENAME, "w");
    if (!file) {
        printf("CMS: Error saving the file.\n");
        return;
    }

    // ===== WRITE METADATA HEADER =====
    fprintf(file, "Database Name: P6_9\n");
    fprintf(file, "Authors: Hafi, Ingmar, Aqif\n");
    fprintf(file, "\n");
    fprintf(file, "Table Name: StudentRecords\n");

    // ===== WRITE COLUMN HEADERS =====
    // Format: Fixed-width columns for alignment
    fprintf(file, "%-10s%-20s%-30s%s\n", "ID", "Name", "Programme", "Mark");

    // 3. Validate and write each record
    int recordsWritten = 0;
    for (int i = 0; i < numRecords; i++) {
        // Check if record data is valid before writing
        if (records[i].id <= 0) {
            printf("CMS: Warning - Invalid ID skipped.\n");
            continue;
        }

        if (strlen(records[i].name) == 0) {
            printf("CMS: Warning - Empty name skipped.\n");
            continue;
        }

        if (records[i].mark < 0 || records[i].mark > 100) {
            printf("CMS: Warning - Invalid mark skipped.\n");
            continue;
        }

        // Write the record with fixed-width formatting for alignment
        int result = fprintf(file, "%-10d%-20s%-30s%.2f\n",
            records[i].id,
            records[i].name,
            records[i].programme,
            records[i].mark);

        // Check if fprintf succeeded
        if (result < 0) {
            printf("CMS: Error writing record.\n");
            fclose(file);
            return;
        }

        recordsWritten++;
    }

    // 4. Check if file write succeeded
    if (fflush(file) != 0) {
        printf("CMS: Error flushing file.\n");
        fclose(file);
        return;
    }

    fclose(file);
    printf("CMS: The database file \"%s\" is successfuly saved.\n", FILENAME);

}