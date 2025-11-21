#pragma once
#include "hfile.h"  // Include the header file for declarations and struct definition
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


//Helper Function: Find Record Index by ID
int findRecordIndexByID(int id) {
    for (int i = 0; i < numRecords; i++) {
        if (records[i].id == id) {
            return i;
        }
    }
    return -1;  // Not found
}
