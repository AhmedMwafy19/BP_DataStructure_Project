#include <stdio.h>
#include "STD_TYPES.h"
#include "card.h"

static uint8 StringLength(unsigned char Copy_u8String[]){
	uint8 Local_u8Counter = 0;
	while(Copy_u8String[Local_u8Counter] != '\0'){
		Local_u8Counter++;
	} /*To find the length of the string*/
	return Local_u8Counter;
}


static uint8 ReadString(char *str,uint8 Length){ /*Function To read String*/
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
EN_CARDDATA CARD_GetCardHolderName(ST_CARDDATA * Copy_pstructCardData){
	uint8 Local_u8StringLength = 0 , Local_u8LoopCounter =0;

	/*Don't Forget To Add it to the Data base*/
	if(Copy_pstructCardData != NULL){/*Checks the validity of the pointer*/
	/*To get the length of the name string */
		printf("Enter card holder name (20-24 characters): \n");
		if(ReadString((Copy_pstructCardData -> CardHolderName) , 25) == 1){
			return WRONG_NAME;
		}


	Local_u8StringLength = StringLength((char *)Copy_pstructCardData -> CardHolderName);

	if(Local_u8StringLength < 20 || Local_u8StringLength > 24){
		return WRONG_NAME;
	} /*Checks if the name fits The naming length rule*/



	for(Local_u8LoopCounter = 0; Local_u8LoopCounter < Local_u8StringLength ;Local_u8LoopCounter++){
		if(!((Copy_pstructCardData -> CardHolderName [Local_u8LoopCounter] >= 'A' && Copy_pstructCardData -> CardHolderName [Local_u8LoopCounter]<= 'Z')
	||(Copy_pstructCardData -> CardHolderName [Local_u8LoopCounter] >= 'a' && Copy_pstructCardData -> CardHolderName [Local_u8LoopCounter]<= 'z'))){
		
			return WRONG_NAME; /*Checks if the name sent consists of only Alphabetical Characters */
		}
	}
/*Don't Forget To Add it to the Data base*/


	}
	else {
	  return NULL_PTR_ERR;
	}
	return CARD_OK;
}


EN_CARDDATA CARD_GetCardExpiryDate(ST_CARDDATA * Copy_pstructCardData){
	uint8 Local_u8StrLen,Local_u8LoopCounter;
	if(Copy_pstructCardData != NULL){/*Checks the validity of the pointer*/
		printf("Enter card holder Expiration Date (MM/YY): \n");
			if(ReadString(Copy_pstructCardData -> Account_ExpirationDate , 6) == 1){
					return WRONG_EXPIRATION_DATE;
					}

		if(Copy_pstructCardData -> Account_ExpirationDate[2] != '/'){
					return WRONG_EXPIRATION_DATE;  /*checks whether the date is in correct form*/
				}


		Local_u8StrLen = StringLength(Copy_pstructCardData ->Account_ExpirationDate);

		if(Local_u8StrLen > 5){
			return WRONG_EXPIRATION_DATE;
		} /*Checks whether the date length is correct*/

		for(Local_u8LoopCounter =0 ;Local_u8LoopCounter<5;Local_u8LoopCounter++){
			if(Local_u8LoopCounter==2){
				continue ;
			}
			if(Copy_pstructCardData->Account_ExpirationDate[Local_u8LoopCounter] < '0' || Copy_pstructCardData->Account_ExpirationDate[Local_u8LoopCounter] > '9'){
				return WRONG_EXPIRATION_DATE;
			}
		}
		                                                  /*This 48 to convert from asci to value*/
		if(((Copy_pstructCardData->Account_ExpirationDate[0]-48)*10 + (Copy_pstructCardData -> Account_ExpirationDate[1]-48))>12){
			return WRONG_EXPIRATION_DATE;
		}/*To make sure The input month is not larger than 12*/

	}
	else {
	  return NULL_PTR_ERR;
	}
	
	return CARD_OK;
}



EN_CARDDATA CARD_GetCardPAN(ST_CARDDATA * Copy_pstructCardData){
	uint8 Local_u8LoopCounter=0,Local_u8StrLen;
	if(Copy_pstructCardData != NULL){/*Checks the validity of the pointer*/
			printf("Enter card holder PAN (16-19NUM): \n");
			if(ReadString(Copy_pstructCardData -> AccountNumber_Prim , 20) == 1){
						return WRONG_PAN;
						}

			Local_u8StrLen=StringLength(Copy_pstructCardData -> AccountNumber_Prim);
			if(Local_u8StrLen <16 || Local_u8StrLen >20){
				return WRONG_PAN;
			}/*Checks whether the PAN length is correct*/

		
		for(Local_u8LoopCounter = 0 ;Local_u8LoopCounter<Local_u8StrLen;Local_u8LoopCounter++){
			if(Copy_pstructCardData->AccountNumber_Prim[Local_u8LoopCounter] < '0' || Copy_pstructCardData->AccountNumber_Prim[Local_u8LoopCounter] > '9'){
				return WRONG_PAN;
			}
		}/*checks whether The PAN consists of numbers or not*/

	}
	else {
	 return NULL_PTR_ERR;

	}
	
	return CARD_OK;
	
}
