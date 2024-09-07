#ifndef FILE_HANDLING_H
#define FILE_HANDLING_H
#include "server.h"
typedef enum{
	FILEHANDLING_SUCCESS,FILEHANDLING_FAILED
}EN_FH;

EN_FH FH_LoadAccounts(void); /*function to load data from data base into program*/

EN_FH FH_LoadTransactions(void);
EN_FH FH_WriteTransactionData(ST_TRANSACTION * Copy_pstructTransactionData);
EN_FH FH_LoadTransactions(void);
EN_FH FH_WriteAccountsData(void);
#endif
