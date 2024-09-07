#ifndef LIST_H
#define LIST_H
#include "server.h"
typedef enum{
	DATA_OK , DATA_ERROR
}EN_DB;
typedef struct AccountNode{
	ST_ACCOUNTS_DB Account_database;
	struct AccountNode * nextptr ;
}ST_DBNode; /*Creating A Linked list Node*/


typedef struct Transaction{
	ST_TRANSACTION Transaction_Data ;
	struct Transaction * nextptr ;
}ST_TDNode;


EN_DB LINKEDLIST_CreateIt(ST_DBNode * * Copy_ppstructHead, uint8 * Copy_pu8AccountNumber, f32 Copy_f32Balance,uint8 Copy_u8State);/*To create a linked list from data gained by file handling*/
void printAllNodes(const ST_TDNode *head);/*Test function To see if it's working*/
EN_DB LINKEDLIST_TDCreateIt(ST_TDNode * * Copy_ppstructHead,uint32 Copy_u32TransactionSeqNum,uint8 * Copy_Transaction_Date,f32 Copy_f32TranAmount,uint32 Copy_u8state,f32 Copy_f32MaxAmount,char * Copy_CardName,uint8 * Copy_au8PAN,uint8 * Copy_ExpirationDate);
ST_TDNode * LinkedLists_u8LastNode(ST_TDNode **head);
void freeList_TD(ST_TDNode **head);
void freeList_DB(ST_DBNode **head);
#endif
