#ifndef TERMINAL_H
#define TERMINAL_H
 typedef struct
 {
 f32 Trans_Amount; /*used to store the amount of money
  the user would like to transfer*/
 f32 Max_Trans_Amount; /*used to store the maximum amount of money
  that can be transferred*/
 uint8 Transaction_Date[11]; // to store transaction date
 }ST_TERMINALDATA;

 typedef enum
 {
 TERMINAL_OK, WRONG_DATE, EXPIRED_CARD, INVALID_CARD, INVALID_AMOUNT,
 EXCEED_MAX_AMOUNT, INVALID_MAX_AMOUNT
 }EN_TERMINALDATA;/*Hold Symbols that will be used a lot in the project*/


EN_TERMINALDATA TERMINAL_GetTransactionDate(ST_TERMINALDATA * Copy_pstructTerminal);

EN_TERMINALDATA TERMINAL_IsCardExpired(ST_CARDDATA * Copy_pstructdata,ST_TERMINALDATA * Copy_pstructTerminal);

EN_TERMINALDATA TERMINAL_GetTransactionAmount(ST_TERMINALDATA * Copy_pstructTerminal);

EN_TERMINALDATA TERMINAL_IsBelowMaxAmount(ST_TERMINALDATA * Copy_pstructTerminal);

EN_TERMINALDATA TERMINAL_SetMaxAmount(ST_TERMINALDATA * Copy_pstructTerminal, f32 Copy_f32MaxAmount);

EN_TERMINALDATA TERMINAL_IsValidCardPAN(ST_CARDDATA * Copy_pstructCardData);
#endif
