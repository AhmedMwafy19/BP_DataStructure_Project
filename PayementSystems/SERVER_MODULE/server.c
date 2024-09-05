#include <stdio.h>
#include "STD_TYPES.h"
#include "ErrorState.h"
#include "card.h"
#include "terminal.h"
#include "server.h"
#include "list.h"
#include "FileHandling.h"

extern ST_DBNode * START ;
extern ST_TDNode * START_TD;
ST_DBNode * current = NULL;

static sint8 StringCmp(const unsigned char *str1, const unsigned char *str2) {
    while (*str1 && (*str1 == *str2)) {
        str1++;
        str2++;
    }
    return *(unsigned char *)str1 - *(unsigned char *)str2;
}

static void Stringcpy(uint8 *str1, const uint8 *str2) {
    while (*str2 != '\0') {
        *str1 = *str2;
        str1++;
        str2++;
    }
    *str1 = '\0'; // Null-terminate
}




EN_TRANS_STATE SERVER_RecieveTransactionData(ST_TRANSACTION * Copy_pstruyctTransData){

	if(Copy_pstruyctTransData != NULL){
		/*See if account number exist in data base*/
		ST_CARDDATA  Local_psCard;
		Stringcpy(Local_psCard.AccountNumber_Prim , Copy_pstruyctTransData->cardHolderData.AccountNumber_Prim);
		Stringcpy(Local_psCard.Account_ExpirationDate , Copy_pstruyctTransData->cardHolderData.Account_ExpirationDate);
		Stringcpy(Local_psCard.CardHolderName , Copy_pstruyctTransData->cardHolderData.CardHolderName);

		if(SERVER_IsValidAccount(&Local_psCard) == ACCOUNT_NOT_FOUND){
			Copy_pstruyctTransData->transState = FRAUD_CARD;
			return FRAUD_CARD;
		}
		/*Check if card is available*/
		ST_ACCOUNTS_DB  Local_psAcc;
		Local_psAcc.balance = current->Account_database.balance;
		Local_psAcc.state = current->Account_database.state;
		Stringcpy(Local_psAcc.primaryAccountNumber , current->Account_database.primaryAccountNumber);

		if(SERVER_IsBlockedAccount(&Local_psAcc ) == BLOCKED_ACCOUNT){
			Copy_pstruyctTransData->transState = DECLINED_STOLEN_CARD;
			return DECLINED_STOLEN_CARD;
		}
		/*Check if transaction amount is available in account*/
		ST_TERMINALDATA  Local_psTerminal ;
		Stringcpy(Local_psTerminal.Transaction_Date , Copy_pstruyctTransData->terminalData.Transaction_Date);
		Local_psTerminal . Trans_Amount = Copy_pstruyctTransData->terminalData.Trans_Amount;
		Local_psTerminal . Max_Trans_Amount = Copy_pstruyctTransData->terminalData.Max_Trans_Amount;

		if(SERVER_IsAmountAvailable(&Local_psTerminal)==LOW_BALANCE){
			Copy_pstruyctTransData->transState = DECLINED_INSUFFECIENT_FUND;
			return DECLINED_INSUFFECIENT_FUND;
		}
		/*Save Data */
		if(SERVER_SaveTransaction(Copy_pstruyctTransData)==SAVING_FAILED){
			return SAVING_FAILED;
		}
	}
	else {
		return INTERNAL_SERVER_ERROR;
	}
	return APPROVED;
}
EN_SERVER_ERROR SERVER_IsValidAccount(ST_CARDDATA * Copy_pstructCardData){
	if(Copy_pstructCardData != NULL){

		current = START ;

		// Traverse the list
	    while (current != NULL) {
	        if(StringCmp((uint8 *)Copy_pstructCardData->AccountNumber_Prim,(uint8 *) current->Account_database.primaryAccountNumber)==1){
	            return SERVER_OK; // Item found
	        }
	        current=current->nextptr;
	    }

	}
	else {
		return ACCOUNT_NOT_FOUND;
	}
	return SERVER_OK;
}


EN_SERVER_ERROR SERVER_IsBlockedAccount(ST_ACCOUNTS_DB * Copy_pstructAccountReference){
	if(Copy_pstructAccountReference != NULL){
	if(Copy_pstructAccountReference->state == BLOCKED){
		return BLOCKED_ACCOUNT;
	}
	else if(Copy_pstructAccountReference->state == RUNNING){
		return SERVER_OK;
	}
	}
	else {
		return BLOCKED_ACCOUNT;
	}
	return SERVER_OK;
}

EN_SERVER_ERROR SERVER_IsAmountAvailable(ST_TERMINALDATA * Copy_pstructTermData){

	if (Copy_pstructTermData == NULL) {
	        return LOW_BALANCE;
	    }
	    if (Copy_pstructTermData->Trans_Amount > current->Account_database.balance) {
	        return LOW_BALANCE;
	    }

	    return SERVER_OK;
}
EN_SERVER_ERROR SERVER_SaveTransaction(ST_TRANSACTION * Copy_pstructTransData){
	static uint32 Local_u32Seq = 1 ;
	Copy_pstructTransData->TransactionSequenceNumber = Local_u32Seq ;
	Local_u32Seq++;
	if(Copy_pstructTransData!=NULL){
	FH_WriteTransactionData(Copy_pstructTransData);
	}
	else
	{
		return SAVING_FAILED;
	}
	return SERVER_OK;
}
void SERVER_ListSavedTransactions(void){
   printAllNodes(START_TD);
}
