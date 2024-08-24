#ifndef FILE_HANDLING_H
#define FILE_HANDLING_H

typedef enum{
	FILEHANDLING_SUCCESS,FILEHANDLING_FAILED
}EN_FH;

EN_FH FH_LoadAccounts(void); /*function to load data from data base into program*/


#endif
