#include "hfile.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.c"

// FUNCTION: openDatabase()
// PURPOSE: Initialize the database by loading all records from the file
// APPROACH: We call loadFromFile() which handles reading the file and populating the records array. Once loaded, we print a confirmation message.
// WHY THIS WORKS: Separating file loading into its own function keeps the code modular and lets us reuse it in other parts (like reload operations)
void openDatabase() {
    int load = loadFromFile();
    if (load == 0){
        printf("CMS: The database file \"%s\" is successfully opened.\n", FILENAME);
    }
}

// FUNCTION: loadFromFile()
// PURPOSE: Read student records from a text file with fixed-width column formatting
//
// FILE FORMAT STRUCTURE:
// - Lines 0-3: Metadata (database name, authors, table name, blank line)
// - Line 4: Header row (ID Name Programme Mark)
// - Lines 5+: Actual student records in fixed-width format
//
// COLUMN POSITIONS (fixed-width):
// - ID: positions 0-9 (10 chars)
// - Name: positions 10-29 (20 chars)
// - Programme: positions 30-59 (30 chars)
// - Mark: positions 60+ (remaining)
//
// PARSING STRATEGY:
// 1. Open the file and check if it exists
// 2. Skip all metadata and header lines (first 5 lines total)
// 3. For each remaining line, use strncpy to extract data from fixed positions
// 4. Trim trailing spaces from extracted strings (important for clean data)
// 5. Convert strings to appropriate types (atoi for ID, atof for Mark)
// 6. Store in records array and increment counter
//
// WHY FIXED-WIDTH: The file uses padding/spacing instead of delimiters, so we have to extract by position. This avoids issues with spaces in names/programmes.
int loadFromFile() {
    FILE* file = fopen(FILENAME, "r");
    if (!file) {
        printf("CMS: Error opening the file.\n");
        return 1;
    }

    char line[256];
    numRecords = 0;  // Reset to avoid double-loading on subsequent calls

    // Skip metadata lines at the top of the file
    fgets(line, sizeof(line), file);  // "Database Name: P6_9 CMS"
    fgets(line, sizeof(line), file);  // "Authors: INGMAR AQIF HAFIZHAH HONG MING"
    fgets(line, sizeof(line), file);  // Blank line
    fgets(line, sizeof(line), file);  // "Table Name: StudentRecords"
    fgets(line, sizeof(line), file);  // "ID Name Programme Mark" (header)

    // Read each student record line by line
    while (fgets(line, sizeof(line), file)) {
        if (strlen(line) < 10) continue;  // Skip empty/malformed lines

        char idStr[11], nameStr[21], progStr[31], markStr[20];

        // Extract data from fixed column positions
        // Using strncpy ensures we don't read beyond the specified width
        strncpy(idStr, line, 10);
        idStr[10] = '\0';

        strncpy(nameStr, line + 10, 20);
        nameStr[20] = '\0';

        strncpy(progStr, line + 30, 30);
        progStr[30] = '\0';

        strcpy(markStr, line + 60);

        // Trim trailing spaces from each field
        // This is necessary because fixed-width format pads with spaces
        int len;

        len = strlen(idStr);
        while (len > 0 && idStr[len - 1] == ' ') idStr[--len] = '\0';

        len = strlen(nameStr);
        while (len > 0 && nameStr[len - 1] == ' ') nameStr[--len] = '\0';

        len = strlen(progStr);
        while (len > 0 && progStr[len - 1] == ' ') progStr[--len] = '\0';

        // Store the converted data into our records array
        records[numRecords].id = atoi(idStr);
        strcpy(records[numRecords].name, nameStr);
        strcpy(records[numRecords].programme, progStr);
        records[numRecords].mark = atof(markStr);

        numRecords++;
    }

    fclose(file);
	return 0;
}



// FUNCTION: showAllRecords()
// PURPOSE: Display all loaded student records in a formatted table
//
// FORMATTING APPROACH:
// - Use printf with format specifiers for column alignment
// - %-10s means left-aligned string in 10-character width
// - %-20s means left-aligned string in 20-character width, etc.
// - %.2f means float with 2 decimal places
// - Header row uses same widths to match data rows
//
// WHY THIS LAYOUT: The fixed widths match our file format, making it easy
// to verify data was loaded correctly and to compare with file contents.
void showAllRecords() {
    printf("CMS: Here are all the records found in the table \"StudentRecords\".\n");
    printf("%-10s%-20s%-30s%s\n", "ID", "Name", "Programme", "Mark");
    for (int i = 0; i < numRecords; i++) {
        printf("%-10d%-20s%-30s%.2f\n", records[i].id, records[i].name, records[i].programme, records[i].mark);
    }
}

/**
 * insertRecord - Adds a new student record to the database
 * @command: Command string in format "INSERT ID=<id> Name=<n> Programme=<prog> Mark=<mark>"
 *
 * Required fields: ID, Name, Programme, Mark (all must have values)
 *
 * Features:
 * - Validates all fields before inserting
 * - Checks for duplicate IDs and rejects them
 * - Handles fields in any order (Name/Programme can appear in any sequence)
 * - Properly trims leading and trailing spaces from extracted values
 * - Removes whitespace-only fields
 * - Returns specific error messages for invalid data
 *
 * Implementation approach:
 * - Locate all field tokens (ID=, Name=, Programme=, Mark=)
 * - Validate ID first and check for duplicates (fail-fast)
 * - Check all other required fields exist and have values (bitmask validation)
 * - Calculate field boundaries dynamically based on position
 * - Extract raw values and trim whitespace
 * - Validate extracted values against business rules
 * - Store record only after all validations pass
 *
 * Priority order:
 * - Check ID exists, is valid, and has value
 * - Check for duplicate ID (most important - reject early)
 * - Check other required fields exist AND have values
 * - Validate and extract Name, Programme, Mark with proper trimming
 * - Store record if all validations pass
 */
void insertRecord(char* command) {
    // ===== LOCATE ALL FIELD TOKENS =====
    char* idPtr = strstr(command, "ID=");
    char* namePtr = strstr(command, "Name=");
    char* progPtr = strstr(command, "Programme=");
    char* markPtr = strstr(command, "Mark=");

    // ===== PARSE AND VALIDATE ID =====
    // Check if ID key exists and has a value (not empty after "ID=")
    if (!idPtr || *(idPtr + 3) == '\0' || *(idPtr + 3) == ' ') {
        printf("CMS: Missing ID field.\n");
        return;
    }

    int id;
    char idStr[20];
    sscanf(idPtr + 3, "%19s", idStr);

    // Validate ID format (must be numeric)
    if (!isValidID(idStr)) {
        printf("CMS: Invalid ID format. ID must contain only numbers.\n");
        return;
    }

    id = atoi(idStr);

    // ===== CHECK FOR DUPLICATE ID FIRST =====
    // Fail fast - no point parsing other fields if ID already exists
    if (findRecordIndexByID(id) != -1) {
        printf("CMS: The record with ID=%d already exists.\n", id);
        return;
    }

    // ===== CHECK WHICH FIELDS ARE MISSING OR EMPTY =====
    // Use bitmask to efficiently track all missing fields at once
    // Bit 0 (1):  Name missing
    // Bit 1 (2):  Programme missing
    // Bit 2 (4):  Mark missing
    int missing = 0;
    if (!namePtr || *(namePtr + 5) == '\0' || *(namePtr + 5) == ' ') missing |= 1;
    if (!progPtr || *(progPtr + 10) == '\0' || *(progPtr + 10) == ' ') missing |= 2;
    if (!markPtr || *(markPtr + 5) == '\0' || *(markPtr + 5) == ' ') missing |= 4;

    // ===== SWITCH ON MISSING FIELDS BITMASK =====
    // Provides specific error message for exact combination of missing fields
    switch (missing) {
    case 0:
        // All fields present - continue to validation
        break;
    case 1:
        printf("CMS: Missing Name field.\n");
        return;
    case 2:
        printf("CMS: Missing Programme field.\n");
        return;
    case 3:
        printf("CMS: Missing Name and Programme fields.\n");
        return;
    case 4:
        printf("CMS: Missing Mark field.\n");
        return;
    case 5:
        printf("CMS: Missing Name and Mark fields.\n");
        return;
    case 6:
        printf("CMS: Missing Programme and Mark fields.\n");
        return;
    case 7:
        printf("CMS: Missing Name, Programme and Mark fields.\n");
        return;
    default:
        return;
    }

    float mark;
    char name[100], programme[100];
    char markStr[20];

    // ===== EXTRACT AND VALIDATE NAME =====
    // Calculate name length based on its position and the next field's position
    char nameRaw[100];
    int nameLen;
    char* nameEnd;

    // Find where name ends - look for the nearest field after Name=
    if (progPtr && (progPtr > namePtr)) {
        nameEnd = progPtr;
    }
    else if (markPtr && (markPtr > namePtr)) {
        nameEnd = markPtr;
    }
    else {
        nameEnd = namePtr + strlen(namePtr);
    }

    nameLen = nameEnd - (namePtr + 5);
    if (nameLen <= 0 || nameLen >= 100) {
        printf("CMS: Invalid Name field length.\n");
        return;
    }
    strncpy(nameRaw, namePtr + 5, nameLen);
    nameRaw[nameLen] = '\0';

    // Trim leading spaces from name
    int nameStart = 0;
    while (nameStart < nameLen && nameRaw[nameStart] == ' ') nameStart++;

    // Trim trailing spaces from name
    int nameEnd_idx = nameLen - 1;
    while (nameEnd_idx >= nameStart && nameRaw[nameEnd_idx] == ' ') nameEnd_idx--;

    // Check if name is all spaces
    if (nameStart > nameEnd_idx) {
        printf("CMS: Invalid Name field.\n");
        return;
    }

    // Copy trimmed name to final buffer
    int trimmedNameLen = nameEnd_idx - nameStart + 1;
    strncpy(name, nameRaw + nameStart, trimmedNameLen);
    name[trimmedNameLen] = '\0';

    // Validate name format (letters, spaces, hyphens only)
    if (!isValidName(name)) {
        printf("CMS: Invalid Name format. Name must contain only letters, spaces, and hyphens.\n");
        return;
    }

    // ===== EXTRACT AND VALIDATE PROGRAMME =====
    // Calculate programme length based on its position and the next field's position
    // Handle case where Name might be sandwiched between Programme and Mark
    char progRaw[100];
    int progLen;
    char* progEnd_ptr;

    // Find where programme ends - look for the nearest field after Programme=
    if (markPtr && (markPtr > progPtr)) {
        // Check if Name is sandwiched between Programme and Mark
        if (namePtr && (namePtr > progPtr) && (namePtr < markPtr)) {
            progEnd_ptr = namePtr;
        }
        else {
            progEnd_ptr = markPtr;
        }
    }
    else if (namePtr && (namePtr > progPtr)) {
        progEnd_ptr = namePtr;
    }
    else {
        progEnd_ptr = progPtr + strlen(progPtr);
    }

    progLen = progEnd_ptr - (progPtr + 10);
    if (progLen <= 0 || progLen >= 100) {
        printf("CMS: Invalid Programme field length.\n");
        return;
    }
    strncpy(progRaw, progPtr + 10, progLen);
    progRaw[progLen] = '\0';

    // Trim leading spaces from programme
    int progStart = 0;
    while (progStart < progLen && progRaw[progStart] == ' ') progStart++;

    // Trim trailing spaces from programme
    int progEnd_idx = progLen - 1;
    while (progEnd_idx >= progStart && progRaw[progEnd_idx] == ' ') progEnd_idx--;

    // Check if programme is all spaces
    if (progStart > progEnd_idx) {
        printf("CMS: Invalid Programme field.\n");
        return;
    }

    // Copy trimmed programme to final buffer
    int trimmedProgLen = progEnd_idx - progStart + 1;
    strncpy(programme, progRaw + progStart, trimmedProgLen);
    programme[trimmedProgLen] = '\0';

    // Validate programme format (letters, spaces, hyphens only)
    if (!isValidProgramme(programme)) {
        printf("CMS: Invalid Programme name. Programme must contain only letters, spaces, and hyphens.\n");
        return;
    }

    // ===== EXTRACT AND VALIDATE MARK =====
    sscanf(markPtr + 5, "%19s", markStr);

    // Validate mark format (must be float between 0-100)
    if (!isValidMark(markStr)) {
        printf("CMS: Invalid Mark format. Mark must be a number between 0 and 100.\n");
        return;
    }

    sscanf(markPtr + 5, "%f", &mark);

    // ===== STORE RECORD IN DATABASE =====
    // All validation passed - store the new record
    records[numRecords].id = id;
    strcpy(records[numRecords].name, name);
    strcpy(records[numRecords].programme, programme);
    records[numRecords].mark = mark;
    numRecords++;

    // ===== CONFIRM INSERTION =====
    printf("CMS: A new record with ID=%d is successfully inserted.\n", id);
}