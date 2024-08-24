#include <stdio.h>
#include <stdlib.h>/*For malloc*/
#include "STD_TYPES.h"
#include "List.h"


void Stringcpy(uint8 *str1, const uint8 *str2) {
    while (*str2 != '\0') {
        *str1 = *str2;
        str1++;
        str2++;
    }
    *str1 = '\0'; // Null-terminate
}


// function to create and add a node to the linked list
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

// Function to print all nodes in the linked list (Used For Testing)
void printAllNodes(const ST_DBNode *head) {
    const ST_DBNode *current = head; // Start with the head of the list

    while (current != NULL) { // Traverse until the end of the list
        printf("Account Number: %s\n", current->Account_database.primaryAccountNumber);
        printf("Balance: %.2f\n", current->Account_database.balance);

        switch (current->Account_database.state) {
            case RUNNING:
                printf("State: RUNNING\n");
                break;
            case BLOCKED:
                printf("State: BLOCKED\n");
                break;
            default:
                printf("State: UNKNOWN\n");
                break;
        }

        printf("-----------------------------\n");

        current = current->nextptr; // Move to the next node
    }
}

