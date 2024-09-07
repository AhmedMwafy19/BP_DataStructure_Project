#include "STD_TYPES.h"
#include <stdio.h>
#include <stdlib.h> /* For malloc */
#include "FileHandling.h"
#include "List.h"

ST_DBNode *START = NULL;
ST_TDNode *START_TD = NULL;

static void Stringcpy(uint8 *str1, const uint8 *str2) {
    while (*str2 != '\0') {
        *str1 = *str2;
        str1++;
        str2++;
    }
    *str1 = '\0'; // Null-terminate
}

EN_FH FH_LoadAccounts(void) {
	freeList_DB(&START);
    FILE *file = fopen("DataBase.txt", "r"); /* Open DataBase.txt for reading */
    if (file == NULL) {
        perror("Error opening file");
        return FILEHANDLING_FAILED;
    }

    // Temporary variables to read data
    uint8 Local_u8accountNumber[20];
    f32 Local_f32Balance;
    uint32 Local_u32state;

    while (fscanf(file, "%19[^,],%f,%lu\n", Local_u8accountNumber, &Local_f32Balance, &Local_u32state) == 3) {
        if (LINKEDLIST_CreateIt(&START, Local_u8accountNumber, Local_f32Balance, Local_u32state) != DATA_OK) {
            fclose(file);
            return FILEHANDLING_FAILED;
        }
    }

    fclose(file); /* Close the file */
    return FILEHANDLING_SUCCESS;
}

EN_FH FH_LoadTransactions(void) {
	freeList_TD(&START_TD);
    FILE *file = fopen("Transactions_DB.txt", "r"); /* Open Transactions_DB.txt for reading */
    if (file == NULL) {
        perror("Error opening file");
        return FILEHANDLING_FAILED;
    }

    // Temporary variables to read data
    uint32 Local_u32TransactionSeqNum;
    uint8 Transaction_Date[11];
    f32 Local_f32TranAmount;
    uint32 Local_u8state;
    f32 Local_f32MaxAmount;
    uint8 Local_CardName[25];
    uint8 Local_au8PAN[20];
    uint8 Account_ExpirationDate[6];

    while (fscanf(file, "%lu %10s %f %lu %f %19s %24s %5s\n",
                  &Local_u32TransactionSeqNum,
                  Transaction_Date,
                  &Local_f32TranAmount,
                  &Local_u8state,
                  &Local_f32MaxAmount,
				  Local_au8PAN,
				  Local_CardName,
                  Account_ExpirationDate) == 8) {
        if (LINKEDLIST_TDCreateIt(&START_TD, Local_u32TransactionSeqNum,
                                  Transaction_Date,
                                  Local_f32TranAmount,
                                  Local_u8state,
                                  Local_f32MaxAmount,
                                  Local_CardName,
                                  Local_au8PAN,
                                  Account_ExpirationDate) != DATA_OK) {
            fclose(file);
            return FILEHANDLING_FAILED;
        }
    }

    fclose(file); /* Close the file */
    return FILEHANDLING_SUCCESS;
}

EN_FH FH_WriteTransactionData(ST_TRANSACTION * Copy_pstructTransactionData) {
    if (Copy_pstructTransactionData == NULL) {
        return FILEHANDLING_FAILED;
    }

    ST_TDNode *Local_pstructCurrent = LinkedLists_u8LastNode(&START_TD);
    if (Local_pstructCurrent == NULL) {
        return FILEHANDLING_FAILED; // Unable to get last node
    }

    // Create a new node
    ST_TDNode *New_Node = (ST_TDNode *)malloc(sizeof(ST_TDNode));
    if (New_Node == NULL) {
        perror("Error allocating memory");
        return FILEHANDLING_FAILED; // Memory allocation failed
    }

    // Initialize the new node
    New_Node->nextptr = NULL;

    // Copy transaction data to new node
    New_Node->Transaction_Data.TransactionSequenceNumber = Copy_pstructTransactionData->TransactionSequenceNumber;
    Stringcpy((uint8 *)New_Node->Transaction_Data.cardHolderData.CardHolderName,
              (uint8*) Copy_pstructTransactionData->cardHolderData.CardHolderName);
    Stringcpy((uint8 *)New_Node->Transaction_Data.cardHolderData.Account_ExpirationDate,
              (uint8*) Copy_pstructTransactionData->cardHolderData.Account_ExpirationDate);
    Stringcpy((uint8 *)New_Node->Transaction_Data.cardHolderData.AccountNumber_Prim,
              (uint8*)Copy_pstructTransactionData->cardHolderData.AccountNumber_Prim);
    Stringcpy((uint8*)New_Node->Transaction_Data.terminalData.Transaction_Date,
              (uint8*) Copy_pstructTransactionData->terminalData.Transaction_Date);
    New_Node->Transaction_Data.terminalData.Max_Trans_Amount = Copy_pstructTransactionData->terminalData.Max_Trans_Amount;
    New_Node->Transaction_Data.terminalData.Trans_Amount = Copy_pstructTransactionData->terminalData.Trans_Amount;
    New_Node->Transaction_Data.transState = Copy_pstructTransactionData->transState;

    // Adding the new node to the end of the linked list
    if (START_TD == NULL) {
        START_TD = New_Node;
    } else {
        Local_pstructCurrent->nextptr = New_Node;
    }

    // Write the transaction data to the file
    FILE *file = fopen("Transactions_DB.txt", "a");
    if (file == NULL) {
        perror("Error opening file");
        free(New_Node); // Free allocated memory on failure
        return FILEHANDLING_FAILED;
    }
    fprintf(file, "%lu %s %.2f %u %.2f %s %s %s\n",
               New_Node->Transaction_Data.TransactionSequenceNumber,
               New_Node->Transaction_Data.terminalData.Transaction_Date,
               New_Node->Transaction_Data.terminalData.Trans_Amount,
               New_Node->Transaction_Data.transState,
               New_Node->Transaction_Data.terminalData.Max_Trans_Amount,
               New_Node->Transaction_Data.cardHolderData.AccountNumber_Prim,
               New_Node->Transaction_Data.cardHolderData.CardHolderName,
               New_Node->Transaction_Data.cardHolderData.Account_ExpirationDate);    fflush(file);
    fclose(file);

    return FILEHANDLING_SUCCESS;
}
EN_FH FH_WriteAccountsData(void) {

    // Create a new node
    ST_DBNode * current ;
    // Initialize the new node
    current = START ;
    // Write the transaction data to the file
    FILE *file = fopen("DataBase.txt", "w");
    if (file == NULL) {
        perror("Error opening file");
      // Free allocated memory on failure
        return FILEHANDLING_FAILED;
    }

        while (current != NULL) {
            fprintf(file, "%19s,%.2f,%lu\n",
                    current->Account_database.primaryAccountNumber,
                    current->Account_database.balance,
                    current->Account_database.state);

            current = current->nextptr;
        }
    fclose(file);

    return FILEHANDLING_SUCCESS;

}
