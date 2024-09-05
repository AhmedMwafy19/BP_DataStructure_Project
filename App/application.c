
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

	 	 ST_CARDDATA cardData;
	     ST_TERMINALDATA terminalData;
	     ST_TRANSACTION transaction;
	     EN_TRANS_STATE transactionStatus;

	    /* Get card data from the user */
	  CARD_GetCardExpiryDate(&cardData);
	  CARD_GetCardHolderName(&cardData);
	  CARD_GetCardPAN(&cardData);
	   /* Get terminal data from the user  */
	  TERMINAL_SetMaxAmount(&terminalData, MAX_AMOUNT);
	  TERMINAL_GetTransactionAmount(&terminalData);
	  TERMINAL_GetTransactionDate(&terminalData);
printf("1");
	     /*  Prepare the transaction  */
	    Stringcpy(transaction.cardHolderData.CardHolderName , cardData.CardHolderName);
	    Stringcpy(transaction.cardHolderData.AccountNumber_Prim , cardData.AccountNumber_Prim);
	    Stringcpy(transaction.cardHolderData.Account_ExpirationDate , cardData.Account_ExpirationDate);
	    Stringcpy(transaction.terminalData.Transaction_Date , terminalData.Transaction_Date);
	    transaction.terminalData.Max_Trans_Amount = terminalData.Max_Trans_Amount;
	    printf("2");

	    transaction.terminalData.Trans_Amount =terminalData.Trans_Amount;

	 /* Process the transaction through the server */
	    printf("3");
	    transactionStatus = SERVER_RecieveTransactionData(&transaction);
	    printf("4");
	     /* Check the transaction status and print appropriate message */
	     switch (transactionStatus) {
	         case APPROVED:
	             printf("Transaction successful.\n");
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



