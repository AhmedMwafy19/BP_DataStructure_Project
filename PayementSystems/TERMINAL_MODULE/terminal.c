#include <stdio.h>
#include "STD_TYPES.h"
#include "card.h"
#include "terminal.h"

static uint8 ReadString(unsigned char *str,uint8 Length){ /*Function To read String*/
	uint8 Local_u8LoopCounter = 0 , character;
	while((character = getchar()) != '\n'){
		if(Local_u8LoopCounter < Length - 1){
			str[Local_u8LoopCounter++]=character;
		}
		else {
			return 1;/* in case string is too long for the size */
		}
	}
   str[Local_u8LoopCounter] = '\0';
   return 0;
}


static uint8 StringLength(unsigned char Copy_u8String[]){
	uint8 Local_u8Counter = 0;
	while(Copy_u8String[Local_u8Counter] != '\0'){
		Local_u8Counter++;
	} /*To find the length of the string*/
	return Local_u8Counter;
}



EN_TERMINALDATA TERMINAL_GetTransactionDate(ST_TERMINALDATA * Copy_pstructTerminal){
	uint8 Local_u8StrLen,Local_u8LoopCounter,Max_Days;
	if(Copy_pstructTerminal != NULL){//checks if the pointer is valid
		printf("Enter the transaction date (DD/MM/YYYY): \n");
		if(ReadString((Copy_pstructTerminal->Transaction_Date) , 11) == 1){
					return WRONG_DATE;
				}

		Local_u8StrLen = StringLength(Copy_pstructTerminal->Transaction_Date);

		if(Local_u8StrLen != 10){
			return WRONG_DATE;
		}/*Checks if the date of the correct length*/


		if(Copy_pstructTerminal->Transaction_Date[2] != '/' ||Copy_pstructTerminal-> Transaction_Date[5] != '/'){
			return WRONG_DATE;
		}/*Checks if the date of the right format*/

		for(Local_u8LoopCounter=0;Local_u8LoopCounter<Local_u8StrLen;Local_u8LoopCounter++){
			if(Local_u8LoopCounter == 2 || Local_u8LoopCounter ==5){
				continue;
			}
			if(Copy_pstructTerminal->Transaction_Date[Local_u8LoopCounter]<'0'||Copy_pstructTerminal->Transaction_Date[Local_u8LoopCounter]>'9'){
				return WRONG_DATE;
			}
		}/*Checks if the date consists of numbers*/

		uint8 Local_u8Days = (Copy_pstructTerminal ->Transaction_Date [0]-'0')*10+(Copy_pstructTerminal->Transaction_Date[1]-'0');
		uint8 Local_u8Months = (Copy_pstructTerminal->Transaction_Date[3]-'0')*10+ (Copy_pstructTerminal->Transaction_Date[4]-'0');
		uint8 Local_u8Years = (Copy_pstructTerminal->Transaction_Date[6]-'0')*1000+ (Copy_pstructTerminal->Transaction_Date[7]-'0')*100+(Copy_pstructTerminal->Transaction_Date[8]-'0')*10+ (Copy_pstructTerminal->Transaction_Date[9]-'0');

		if (Local_u8Months > 12 || Local_u8Months < 1){
			return WRONG_DATE;
		}

		switch(Local_u8Months){
		case 4 : case 6: case 9: case 11:
			Max_Days=30;
			break;
		case 2 :
			 if ((Local_u8Years % 4 == 0 && Local_u8Years % 100 != 0) || (Local_u8Years % 400 == 0)) {
			                Max_Days = 29; // Leap Years
			            } else {
			                Max_Days = 28; // Non-leap Years
			            } /*This information is from the internet tbf i don't know if it,s correct*/
		break;
		default :
			Max_Days =31;
			break;
		}
		if(Local_u8Days >Max_Days || Local_u8Days <1){
			return WRONG_DATE;
		}

	}
	else {
		return WRONG_DATE;
	}
	return TERMINAL_OK;
}


EN_TERMINALDATA TERMINAL_IsCardExpired(ST_CARDDATA * Copy_pstructdata,ST_TERMINALDATA * Copy_pstructTerminal){
	uint8 Local_u8EXPYears,Local_u8EXPMonths,Local_u8Years,Local_u8Months,Local_u8Days;
	if(Copy_pstructTerminal != NULL){//checks if the poiter is valid
		Local_u8Years = (Copy_pstructTerminal->Transaction_Date[6]-'0')*1000+ (Copy_pstructTerminal->Transaction_Date[7]-'0')*100+(Copy_pstructTerminal->Transaction_Date[8]-'0')*10+ (Copy_pstructTerminal->Transaction_Date[9]-'0');
		Local_u8Months =(Copy_pstructTerminal->Transaction_Date[3]-'0')*10+ (Copy_pstructTerminal->Transaction_Date[4]-'0');
		Local_u8Days = (Copy_pstructTerminal ->Transaction_Date [0]-'0')*10+(Copy_pstructTerminal->Transaction_Date[1]-'0');
		Local_u8EXPMonths = (Copy_pstructdata->Account_ExpirationDate[0] - '0') * 10 + (Copy_pstructdata->Account_ExpirationDate[1] - '0');
		Local_u8EXPYears = (Copy_pstructdata->Account_ExpirationDate[3] - '0') * 10 + (Copy_pstructdata->Account_ExpirationDate[4] - '0') + 2000;

		if(Local_u8EXPYears > Local_u8Years){
			return TERMINAL_OK;
		}
		else if(Local_u8EXPYears < Local_u8Years){
			return EXPIRED_CARD;
		}/*Check the years first */

		if(Local_u8EXPMonths > Local_u8Months){
			return TERMINAL_OK;
		}
		else if(Local_u8EXPMonths < Local_u8Months){
					return EXPIRED_CARD;
				}/*if the same year check month*/

		if(Local_u8Days>1){
			return EXPIRED_CARD;
		}/*if same month check days*/
	}
	else {
		return EXPIRED_CARD;
	}
	return TERMINAL_OK;
}



EN_TERMINALDATA TERMINAL_GetTransactionAmount(ST_TERMINALDATA * Copy_pstructTerminal){
	f32 Local_f32TransAmount = 0 ;
	if(Copy_pstructTerminal != NULL){//checks if the pointer is valid
		 printf("Enter the transaction amount: \n");
		 scanf("%f",&Local_f32TransAmount);
		 while (getchar() != '\n'); // to fix the buffer issue (found out about it online)
		if(Local_f32TransAmount <= (float)0){
			return INVALID_AMOUNT;
		}/*checks if transfer amount is valid > 0*/
	}
	else {
		return INVALID_AMOUNT;
	}
	Copy_pstructTerminal->Trans_Amount = Local_f32TransAmount;
	return TERMINAL_OK;
}

EN_TERMINALDATA TERMINAL_IsBelowMaxAmount(ST_TERMINALDATA * Copy_pstructTerminal){
	if(Copy_pstructTerminal != NULL){//checks if the pointer is valid

		if(Copy_pstructTerminal->Trans_Amount > Copy_pstructTerminal-> Max_Trans_Amount){
			return EXCEED_MAX_AMOUNT;
		}
		 /*checks if the amount of money transferred is valid in relation to max amount */
	}
	else {
		return EXCEED_MAX_AMOUNT;
	}
	return TERMINAL_OK;
}

EN_TERMINALDATA TERMINAL_SetMaxAmount(ST_TERMINALDATA * Copy_pstructTerminal, f32 Copy_f32MaxAmount){
	if(Copy_pstructTerminal != NULL){ // checks if the pointer is a valid pointer
		if(Copy_f32MaxAmount <= 0){
			return INVALID_MAX_AMOUNT;
		}/*checks if the maximum amount is valid*/

	}
	else {
		return INVALID_MAX_AMOUNT;
	}
	Copy_pstructTerminal->Max_Trans_Amount = Copy_f32MaxAmount;
	return TERMINAL_OK;
}

EN_TERMINALDATA TERMINAL_IsValidCardPAN(ST_CARDDATA * Copy_pstructCardData){/*checks if PAN is a Luhn Number*/
	uint8 Local_u8StrLen,Local_u8Num ,Local_u8flag=0,Local_u8Sum=0;
	sint8 Local_s8LoopCounter;
	if(Copy_pstructCardData != NULL){// checks if the pointer is a valid pointer
		Local_u8StrLen = StringLength(Copy_pstructCardData->AccountNumber_Prim);
/*What is a luhn number from Internet ?
Starting from the rightmost digit (the check digit), double every second digit.
If doubling a number results in a number greater than 9, subtract 9 from the result.
Add all the digits together, including the digits that were not doubled.
If the total is divisible by 10,
then the number is valid according to the Luhn formula. */
		for(Local_s8LoopCounter = Local_u8StrLen -1;Local_s8LoopCounter>=0;Local_s8LoopCounter--){
			Local_u8Num =( Copy_pstructCardData -> AccountNumber_Prim[Local_s8LoopCounter])-'0';
			if(Local_u8flag == 1){
				Local_u8Num *= 2;
				if(Local_u8Num > 9){
					Local_u8Num-=9;
				}
			}
			Local_u8flag = !Local_u8flag;
			Local_u8Sum += Local_u8Num;
		}
		if(Local_u8Sum % 10 != 0){
			return INVALID_CARD;
		}
	}
	else {
		return INVALID_CARD;
	}
	return TERMINAL_OK;
}
