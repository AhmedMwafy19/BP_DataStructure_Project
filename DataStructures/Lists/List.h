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

EN_DB LINKEDLIST_CreateIt(ST_DBNode * * Copy_ppstructHead, uint8 * Copy_pu8AccountNumber, f32 Copy_f32Balance,uint8 Copy_u8State);/*To create a linked list from data gained by file handling*/
void printAllNodes(const ST_DBNode *head);/*Test function To see if it's working*/
void Stringcpy(uint8 *str1, const uint8 *str2);/*used instead of library to have better control*/
#endif
