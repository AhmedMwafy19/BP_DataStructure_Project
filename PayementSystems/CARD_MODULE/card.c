#include "STD_TYPES.h"
#include "ErrorState.h"
#include "card.h"


EN_CARDDATA CARD_GetCardHolderName(ST_CARDDATA * Copy_pstructCardData){
	uint8 Local_u8ErrorState = OK ;  /*Returns the error if existed*/
	if(Copy_pstructCardData != NULL){/*Checks the validity of the pointer*/
		
	}
	else {
	  Local_u8ErrorState = NULL_PTR_ERR;
	  return Local_u8ErrorState ;
	}
	
}

EN_CARDDATA CARD_GetCardExpiryDate(ST_CARDDATA * Copy_pstructCardData){
	uint8 Local_u8ErrorState = OK ;  /*Returns the error if existed*/
	if(Copy_pstructCardData != NULL){/*Checks the validity of the pointer*/
		
	}
	else {
	  Local_u8ErrorState = NULL_PTR_ERR;
	  return Local_u8ErrorState ;
	}
	
	
}

EN_CARDDATA CARD_GetCardPAN(ST_CARDDATA * Copy_pstructCardData){
	uint8 Local_u8ErrorState = OK ;  /*Returns the error if existed*/
	if(Copy_pstructCardData != NULL){/*Checks the validity of the pointer*/
		
	}
	else {
	  Local_u8ErrorState = NULL_PTR_ERR;
	  return Local_u8ErrorState ;
	}
	
	
	
}