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
    FILE *file = fopen("DataBase.txt", "r"); /* Open DataBase.txt for reading */
    if (file == NULL) {
        return FILEHANDLING_FAILED;
    }

    // Temporary variables to read data
    uint8 Local_u8accountNumber[20];
    f32 Local_f32Balance;
    uint32 Local_u32state;

    while (fscanf(file, "%19[^,],%f,%lu\n", Local_u8accountNumber, &Local_f32Balance, &Local_u32state) == 3) {
        // Insert the new node into the linked list
        LINKEDLIST_CreateIt(&START, Local_u8accountNumber, Local_f32Balance, Local_u32state); /* Function to move data from file into linked list */
    }

    fclose(file); /* Close the file */
    return FILEHANDLING_SUCCESS;
}

EN_FH FH_LoadTransactions(void) {
    FILE *file = fopen("Transactions_DB.txt", "r"); /* Open Transactions_DB.txt for reading */
    if (file == NULL) {
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

    while (fscanf(file, "%lu %10s %f %lu %f %24[^\t] %19s %5s\n",
                  &Local_u32TransactionSeqNum,
                  Transaction_Date,
                  &Local_f32TranAmount,
                  &Local_u8state,
                  &Local_f32MaxAmount,
                  Local_CardName,
                  Local_au8PAN,
                  Account_ExpirationDate) == 8) {
        // Insert the new node into the linked list
        LINKEDLIST_TDCreateIt(&START_TD, Local_u32TransactionSeqNum,
                              Transaction_Date,
                              Local_f32TranAmount,
                              Local_u8state,
                              Local_f32MaxAmount,
                              Local_CardName,
                              Local_au8PAN,
                              Account_ExpirationDate); /* Function to move data from file into linked list */
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
    if(START_TD==NULL){
    	START_TD = New_Node;
    }

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
    Local_pstructCurrent->nextptr = New_Node;
    New_Node->nextptr = NULL;

    // Write the transaction data to the file
    FILE *file = fopen("Transactions_DB.txt", "a");
    if (file == NULL) {
        perror("Error opening file");
        free(New_Node); // Free allocated memory on failure
        return FILEHANDLING_FAILED;
    }

    fprintf(file, "%lu\n", New_Node->Transaction_Data.TransactionSequenceNumber);
    fprintf(file, "%s\n", New_Node->Transaction_Data.cardHolderData.CardHolderName);
    fprintf(file, "%s\n", New_Node->Transaction_Data.cardHolderData.Account_ExpirationDate);
    fprintf(file, "%s\n", New_Node->Transaction_Data.cardHolderData.AccountNumber_Prim);
    fprintf(file, "%s\n", New_Node->Transaction_Data.terminalData.Transaction_Date);
    fprintf(file, "%.2f\n", New_Node->Transaction_Data.terminalData.Max_Trans_Amount);
    fprintf(file, "%.2f\n", New_Node->Transaction_Data.terminalData.Trans_Amount);

    // Print the transaction state as a string
    switch (New_Node->Transaction_Data.transState) {
        case APPROVED:
            fprintf(file, "Transaction State: APPROVED\n");
            break;
        case DECLINED_INSUFFECIENT_FUND:
            fprintf(file, "Transaction State: DECLINED_INSUFFECIENT_FUND\n");
            break;
        case DECLINED_STOLEN_CARD:
            fprintf(file, "Transaction State: DECLINED_STOLEN_CARD\n");
            break;
        case FRAUD_CARD:
            fprintf(file, "Transaction State: FRAUD_CARD\n");
            break;
        case INTERNAL_SERVER_ERROR:
            fprintf(file, "Transaction State: INTERNAL_SERVER_ERROR\n");
            break;
        default:
            fprintf(file, "Transaction State: UNKNOWN\n");
            break;
    }

    fflush(file);
    fclose(file);

    return FILEHANDLING_SUCCESS;
}
