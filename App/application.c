
/*
 * application.c
 *
 *  Created on: Sep 5, 2024
 *      Author: Computec
 */
#include "STD_TYPES.h"
#include <stdio.h>
#include "card.h"
#include "terminal.h"
#include "server.h"
#include "list.h"
#include "FileHandling.h"
#include "application.h"
static void Stringcpy(uint8 *str1, const uint8 *str2) {
    while (*str2 != '\0') {
        *str1 = *str2;
        str1++;
        str2++;
    }
    *str1 = '\0'; // Null-terminate
}


 void appStart(void){

	 FH_LoadAccounts();
	 FH_LoadTransactions();

	     ST_CARDDATA cardData;
	     EN_CARDDATA status;
	     ST_TERMINALDATA terminalData;
	     EN_TERMINALDATA status1;
	     ST_TRANSACTION transaction;
	     EN_TRANS_STATE transactionStatus;

	    /* Get card data from the user */
	         status = CARD_GetCardHolderName(&cardData);
	         if (status != CARD_OK) {
	             printf("Error: Invalid card holder name.\n");
	             return;
	         }
	         printf("Card holder name entered successfully.\n");

	         // Get card expiration date
	         status = CARD_GetCardExpiryDate(&cardData);
	         if (status != CARD_OK) {
	             printf("Error: Invalid expiration date.\n");
	             return;
	         }
	         printf("Card expiration date entered successfully.\n");
	         // Get card PAN
	         status = CARD_GetCardPAN(&cardData);
	         if (status != CARD_OK) {
	             printf("Error: Invalid PAN.\n");
	             return;
	         }
	         printf("Card PAN entered successfully.\n");



	      status1 = TERMINAL_SetMaxAmount(&terminalData, MAX_AMOUNT);
	           if (status1 != TERMINAL_OK) {
	               printf("Error setting maximum transaction amount.\n");
	           }

	           // Get the transaction amount
	           status1 = TERMINAL_GetTransactionAmount(&terminalData);
	           if (status1 != TERMINAL_OK) {
	               printf("Error: Invalid transaction amount.\n");
	           }

	           // Get the transaction date
	           status1 = TERMINAL_GetTransactionDate(&terminalData);
	           if (status1 != TERMINAL_OK) {
	               printf("Error: Invalid transaction date.\n");
	           }

	           /*  Prepare the transaction  */
	    Stringcpy(transaction.cardHolderData.CardHolderName , cardData.CardHolderName);
	    Stringcpy(transaction.cardHolderData.AccountNumber_Prim , cardData.AccountNumber_Prim);
	    Stringcpy(transaction.cardHolderData.Account_ExpirationDate , cardData.Account_ExpirationDate);
	    Stringcpy(transaction.terminalData.Transaction_Date , terminalData.Transaction_Date);
	    transaction.terminalData.Max_Trans_Amount = terminalData.Max_Trans_Amount;


	    transaction.terminalData.Trans_Amount =terminalData.Trans_Amount;

	 /* Process the transaction through the server */
	    transactionStatus = SERVER_RecieveTransactionData(&transaction);
	     /* Check the transaction status and print appropriate message */
	     switch (transactionStatus) {
	         case APPROVED:
	             printf("/nTransaction successful.\n");
	             break;
	         case FRAUD_CARD:
	             printf("Transaction declined: Fraud card.\n");
	             break;
	         case DECLINED_STOLEN_CARD:
	             printf("Transaction declined: Stolen card.\n");
	             break;
	         case DECLINED_INSUFFECIENT_FUND:
	             printf("Transaction declined: Insufficient funds.\n");
	             break;
	         case INTERNAL_SERVER_ERROR:
	             printf("Transaction failed: Saving transaction data.\n");
	             break;
	         default:
	             printf("Transaction failed: Unknown error.\n");
	             break;
	     }

	     // List saved transactions
	     printf("\nSaved Transactions:\n");
	     SERVER_ListSavedTransactions();

 }



