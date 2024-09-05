/*
 * server.h
 *
 *  Created on: Aug 24, 2024
 *      Author: Computec
 */

#ifndef SERVER_H_//File guard to protect the project from multiple inclusions
#define SERVER_H_

#include "card.h"
#include "terminal.h"

typedef enum
 {
 APPROVED, DECLINED_INSUFFECIENT_FUND, DECLINED_STOLEN_CARD,
 FRAUD_CARD, INTERNAL_SERVER_ERROR
 }EN_TRANS_STATE;

 typedef struct{
 ST_CARDDATA cardHolderData;
 ST_TERMINALDATA terminalData;
 EN_TRANS_STATE transState;
 uint32 TransactionSequenceNumber;
 }ST_TRANSACTION;

 typedef enum
 {
 SERVER_OK, SAVING_FAILED, TRANSACTION_NOT_FOUND, ACCOUNT_NOT_FOUND,
 LOW_BALANCE, BLOCKED_ACCOUNT
 }EN_SERVER_ERROR ;

 typedef enum
 {
 RUNNING,
 BLOCKED
 }EN_ACCOUNT_STATE;

 typedef struct
 {
 float balance;
EN_ACCOUNT_STATE state;
 uint8 primaryAccountNumber[20];
 }ST_ACCOUNTS_DB;


EN_TRANS_STATE SERVER_RecieveTransactionData(ST_TRANSACTION * Copy_pstruyctTransData);
EN_SERVER_ERROR SERVER_IsValidAccount(ST_CARDDATA * Copy_pstructCardData);
EN_SERVER_ERROR SERVER_IsBlockedAccount(ST_ACCOUNTS_DB * Copy_pstructAccountReference);
EN_SERVER_ERROR SERVER_IsAmountAvailable(ST_TERMINALDATA * Copy_pstructTermData);
EN_SERVER_ERROR SERVER_SaveTransaction(ST_TRANSACTION * Copy_pstructTransData);
void SERVER_ListSavedTransactions(void);

#endif /* SERVER_H_ */
