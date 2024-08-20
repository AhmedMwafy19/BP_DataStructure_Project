#ifndef CARD_H //File gaurd to protect the project from multiple inclusions
#define CARD_H
typedef struct {
	uint8 CardHolderName[25]; // Meant to store the name of the card holder
	uint8 AccountNumber_Prim[20]; // Meant to Store The account number of the holder 
	uint8 Account_ExpirationDate[6]; // meant to the expiration date 
}ST_CARDDATA;
typedef enum {
	CARD_OK , WRONG_NAME , WRONG_EXPIRATION_DATE , WRONG_PAN
}EN_CARDDATA;/*Hold Symbols that will be used a lot in the project*/

EN_CARDDATA CARD_GetCardHolderName(ST_CARDDATA * Copy_pstructCardData); // using pointers to save stack memory

EN_CARDDATA CARD_GetCardExpiryDate(ST_CARDDATA * Copy_pstructCardData);

EN_CARDDATA CARD_GetCardPAN(ST_CARDDATA * Copy_pstructCardData);

#endif