#include "STD_TYPES.h"
#include <stdio.h>
#include "FileHandling.h"
#include "List.h"

ST_DBNode *START = NULL;

EN_FH FH_LoadAccounts(void) {
	FILE *file = fopen("DataBase.txt", "r"); /*Open DataBase.txt which contains the information for reading only */
    if (file == NULL) {
        return FILEHANDLING_FAILED;
    }

    // Temporary variables to read data
    uint8 Local_u8accountNumber[20];
    f32 Local_f32Balance;
    uint32 Local_u32state;

    while (fscanf(file, "%19[^,],%f,%lu\n", Local_u8accountNumber, &Local_f32Balance, &Local_u32state) == 3) {
    	printf("Read accountNumber: %s, balance: %.2f, state: %lu\n", Local_u8accountNumber, Local_f32Balance,Local_u32state);
        // Insert the new node into the linked list
        LINKEDLIST_CreateIt(&START,Local_u8accountNumber,Local_f32Balance,Local_u32state);/*Call a function in list.h to move data
         from file into linkedlist format so it's used in program */
    }

    fclose(file);/*Closing the file*/
    return FILEHANDLING_SUCCESS;
}
