#include <stdio.h>
#include <stdlib.h>/*For malloc*/
#include "STD_TYPES.h"
#include "List.h"


static void Stringcpy(uint8 *str1, const uint8 *str2) {
    while (*str2 != '\0') {
        *str1 = *str2;
        str1++;
        str2++;
    }
    *str1 = '\0'; // Null-terminate
}




// function to create and add a node to the linked list that deals with account data base
EN_DB LINKEDLIST_CreateIt(ST_DBNode * * Copy_ppstructHead, uint8 * Copy_pu8AccountNumber, f32 Copy_f32Balance,uint8 Copy_u8State) {
    ST_DBNode *New_Node = (ST_DBNode *)malloc(sizeof(ST_DBNode));
    static ST_DBNode * current;
    if (New_Node == NULL) {
        return DATA_ERROR; // Memory allocation failed
    }

    // Initialize the new node
    New_Node->nextptr = NULL;
    //initialize the data inTo new node
    New_Node->Account_database.balance = Copy_f32Balance;
    New_Node->Account_database.state =Copy_u8State;
    Stringcpy(New_Node->Account_database.primaryAccountNumber,Copy_pu8AccountNumber);

    if (*Copy_ppstructHead== NULL) {
        // The list is empty, so this node becomes the head
        *Copy_ppstructHead = New_Node;
        current= New_Node;
    } else {
        // Add the new node to the end of the list
        current->nextptr = New_Node;
    }

    // Update the current pointer to the new node
    current = New_Node;

    return DATA_OK;
}



// function to create and add a node to the linked list &deals with trans. database
EN_DB LINKEDLIST_TDCreateIt(ST_TDNode * * Copy_ppstructHead,uint32 Copy_u32TransactionSeqNum,uint8 * Copy_Transaction_Date,f32 Copy_f32TranAmount,uint32 Copy_u8state,f32 Copy_f32MaxAmount,char * Copy_CardName,uint8 * Copy_au8PAN,uint8 * Copy_ExpirationDate){
    ST_TDNode *New_Node = (ST_TDNode *)malloc(sizeof(ST_TDNode));
    static ST_TDNode * current;
    if (New_Node == NULL) {
        return DATA_ERROR; // Memory allocation failed
    }
    // Initialize the new node
    New_Node->nextptr = NULL;
    //initialize the data inTo new node
    New_Node->Transaction_Data.TransactionSequenceNumber = Copy_u32TransactionSeqNum;
    New_Node->Transaction_Data.transState = Copy_u8state ;
    Stringcpy(New_Node->Transaction_Data.cardHolderData.AccountNumber_Prim , Copy_au8PAN);
    Stringcpy((uint8*)New_Node->Transaction_Data.cardHolderData.CardHolderName ,(uint8*) Copy_CardName);
    Stringcpy(New_Node->Transaction_Data.cardHolderData.Account_ExpirationDate , Copy_ExpirationDate);
    Stringcpy(New_Node->Transaction_Data.terminalData.Transaction_Date , Copy_Transaction_Date);
    New_Node->Transaction_Data.terminalData.Max_Trans_Amount = Copy_f32MaxAmount;
    New_Node->Transaction_Data.terminalData.Trans_Amount = Copy_f32TranAmount;


    if (*Copy_ppstructHead== NULL) {
        // The list is empty, so this node becomes the head
        *Copy_ppstructHead = New_Node;
        current= New_Node;
    } else {
        // Add the new node to the end of the list
        current->nextptr = New_Node;
    }

    // Update the current pointer to the new node
    current = New_Node;

    return DATA_OK;
}

// Function to print all nodes in the linked list (Used For Testing)
void printAllNodes(const ST_TDNode *head) {
    const ST_TDNode *New_Node = head; // Start with the head of the list

    while (New_Node != NULL) { // Traverse until the end of the list
    	  	  	printf("Transaction Sequence Number: %d\n", New_Node->Transaction_Data.TransactionSequenceNumber);
    		    printf("Card Holder Name: %s\n", New_Node->Transaction_Data.cardHolderData.CardHolderName);
    		    printf("Account Expiration Date: %s\n", New_Node->Transaction_Data.cardHolderData.Account_ExpirationDate);
    		    printf("Account Number: %s\n", New_Node->Transaction_Data.cardHolderData.AccountNumber_Prim);
    		    printf("Transaction Date: %s\n", New_Node->Transaction_Data.terminalData.Transaction_Date);
    		    printf("Max Transaction Amount: %.2f\n", New_Node->Transaction_Data.terminalData.Max_Trans_Amount);
    		    printf("Transaction Amount: %.2f\n", New_Node->Transaction_Data.terminalData.Trans_Amount);

    		    // Print the transaction state as a string
    		    switch (New_Node->Transaction_Data.transState) {
    		        case APPROVED:
    		            printf("Transaction State: APPROVED\n");
    		            break;
    		        case DECLINED_INSUFFECIENT_FUND:
    		            printf( "Transaction State: DECLINED_INSUFFECIENT_FUND\n");
    		            break;
    		        case DECLINED_STOLEN_CARD:
    		            printf("Transaction State: DECLINED_STOLEN_CARD\n");
    		            break;
    		        case FRAUD_CARD:
    		            printf("Transaction State: FRAUD_CARD\n");
    		            break;
    		        case INTERNAL_SERVER_ERROR:
    		            printf("Transaction State: INTERNAL_SERVER_ERROR\n");
    		            break;
    		        default:
    		            printf("Transaction State: UNKNOWN\n");
    		            break;
    		    }

        printf("-----------------------------\n");

        New_Node= New_Node->nextptr; // Move to the next node
    }
}



ST_TDNode * LinkedLists_u8LastNode(ST_TDNode **head) {
    ST_TDNode *current = *head;

   /* Traverse the list */
    while (current->nextptr != NULL) {
        current = current->nextptr;
    }

    return current; /*Returns current that points to the last node*/
}



