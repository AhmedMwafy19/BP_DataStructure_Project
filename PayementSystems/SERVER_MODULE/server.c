#include <stdio.h>
#include "STD_TYPES.h"
#include "server.h"
#include "list.h"
#include "FileHandling.h"

extern ST_DBNode *START;
extern ST_TDNode *START_TD;
static sint8 StringCmp(const uint8 *str1, const uint8 *str2) {
    while (*str1 && (*str1 == *str2)) {
        str1++;
        str2++;
    }
    return *(uint8 *)str1 - *(uint8 *)str2;
}

static void Stringcpy(uint8 *str1, const uint8 *str2) {
    while (*str2 != '\0') {
        *str1 = *str2;
        str1++;
        str2++;
    }
    *str1 = '\0'; // Null-terminate
}

EN_TRANS_STATE SERVER_RecieveTransactionData(ST_TRANSACTION * Copy_pstruyctTransData) {
    if (Copy_pstruyctTransData == NULL) {
        return INTERNAL_SERVER_ERROR;
    }

    // Check if account number exists in database
    ST_CARDDATA Local_psCard;
    Stringcpy(Local_psCard.AccountNumber_Prim, Copy_pstruyctTransData->cardHolderData.AccountNumber_Prim);
    Stringcpy(Local_psCard.Account_ExpirationDate, Copy_pstruyctTransData->cardHolderData.Account_ExpirationDate);
    Stringcpy(Local_psCard.CardHolderName, Copy_pstruyctTransData->cardHolderData.CardHolderName);

    if (SERVER_IsValidAccount(&Local_psCard) == ACCOUNT_NOT_FOUND) {
        Copy_pstruyctTransData->transState = FRAUD_CARD;
        if (SERVER_SaveTransaction(Copy_pstruyctTransData) == SAVING_FAILED) {
            return SAVING_FAILED;
        }
        return FRAUD_CARD;
    }

    // Find the account node in the database
    ST_DBNode *accountNode = START;
    while (accountNode != NULL) {
        if (StringCmp((uint8 *)Copy_pstruyctTransData->cardHolderData.AccountNumber_Prim, (uint8 *)accountNode->Account_database.primaryAccountNumber) == 0) {
            break;
        }
        accountNode = accountNode->nextptr;
    }

    if (accountNode == NULL) {
        Copy_pstruyctTransData->transState = FRAUD_CARD;
        return FRAUD_CARD;
    }

    // Check if card is available
    ST_ACCOUNTS_DB Local_psAcc;
    Local_psAcc.balance = accountNode->Account_database.balance;
    Local_psAcc.state = accountNode->Account_database.state;
    Stringcpy(Local_psAcc.primaryAccountNumber, accountNode->Account_database.primaryAccountNumber);

    if (SERVER_IsBlockedAccount(&Local_psAcc) == BLOCKED_ACCOUNT) {
        Copy_pstruyctTransData->transState = DECLINED_STOLEN_CARD;
        if (SERVER_SaveTransaction(Copy_pstruyctTransData) == SAVING_FAILED) {
            return SAVING_FAILED;
        }
        return DECLINED_STOLEN_CARD;
    }

    // Check if transaction amount is available in account
    ST_TERMINALDATA Local_psTerminal;
    Stringcpy(Local_psTerminal.Transaction_Date, Copy_pstruyctTransData->terminalData.Transaction_Date);
    Local_psTerminal.Trans_Amount = Copy_pstruyctTransData->terminalData.Trans_Amount;
    Local_psTerminal.Max_Trans_Amount = Copy_pstruyctTransData->terminalData.Max_Trans_Amount;

    if (SERVER_IsAmountAvailable(&Local_psTerminal,accountNode) == LOW_BALANCE) {
        Copy_pstruyctTransData->transState = DECLINED_INSUFFECIENT_FUND;
        if (SERVER_SaveTransaction(Copy_pstruyctTransData) == SAVING_FAILED) {
            return SAVING_FAILED;
        }
        return DECLINED_INSUFFECIENT_FUND;
    }
    accountNode->Account_database.balance -= Local_psTerminal.Trans_Amount;
    //change the balance
    FH_WriteAccountsData();


    // Save Data
    Copy_pstruyctTransData->transState = APPROVED ;
    if (SERVER_SaveTransaction(Copy_pstruyctTransData) == SAVING_FAILED) {
        return SAVING_FAILED;
    }

    return APPROVED;
}

EN_SERVER_ERROR SERVER_IsValidAccount(ST_CARDDATA * Copy_pstructCardData) {
    if (Copy_pstructCardData != NULL) {
        ST_DBNode *current = START;

        // Traverse the list
        while (current != NULL) {
            if (StringCmp((uint8 *)Copy_pstructCardData->AccountNumber_Prim, (uint8 *)current->Account_database.primaryAccountNumber) == 0) {
                return SERVER_OK; // Item found
            }
            current = current->nextptr;
        }
    }
    return ACCOUNT_NOT_FOUND; // Indicate item was not found
}

EN_SERVER_ERROR SERVER_IsBlockedAccount(ST_ACCOUNTS_DB * Copy_pstructAccountReference) {
    if (Copy_pstructAccountReference != NULL) {
        if (Copy_pstructAccountReference->state == BLOCKED) {
            return BLOCKED_ACCOUNT;
        } else if (Copy_pstructAccountReference->state == RUNNING) {
            return SERVER_OK;
        }
    }
    return BLOCKED_ACCOUNT; // Indicate account is blocked or invalid
}

EN_SERVER_ERROR SERVER_IsAmountAvailable(ST_TERMINALDATA * Copy_pstructTermData,ST_ACCOUNTS_DB * Copy_pstructAccountReference) {
    if (Copy_pstructTermData != NULL) {

                if (Copy_pstructTermData->Trans_Amount > Copy_pstructAccountReference->balance) {
                    return LOW_BALANCE;
                }
                return SERVER_OK;

    }
    return LOW_BALANCE; // Indicate insufficient funds or invalid data
}

EN_SERVER_ERROR SERVER_SaveTransaction(ST_TRANSACTION * Copy_pstructTransData) {
    uint32 Local_u32Seq;
    ST_TDNode * current = START_TD;
     Local_u32Seq = current->Transaction_Data.TransactionSequenceNumber;
    while(current->nextptr != NULL){
    	Local_u32Seq = current->Transaction_Data.TransactionSequenceNumber;
    	current=current->nextptr;
    }
    if (Copy_pstructTransData != NULL) {
        Copy_pstructTransData->TransactionSequenceNumber = ++Local_u32Seq; /*To increase sequence number by once every time*/
        if (FH_WriteTransactionData(Copy_pstructTransData) != FILEHANDLING_SUCCESS) {
            return SAVING_FAILED;
        }
    }
    else
    {
        return SAVING_FAILED;
    }
    return SERVER_OK;
}

void SERVER_ListSavedTransactions(void) {
    printAllNodes(START_TD);
}
