/*
 * code.h
 *
 *  Created on: May 14, 2025
 *      Author: arthurloeul
 */

#ifndef BSP_CODE_CODE_H_
#define BSP_CODE_CODE_H_

#define CODE_LENGTH 4
#define MAX_ADMIN_CODES 2
#define MAX_LIVREUR_CODES 4

typedef enum {
    CODE_NOT_READY = 0,
    CODE_INCORRECT,
    CODE_CORRECT
} CodeStatus;

typedef enum {
    ADD_NOT_READY = 0,
    ADD_CODE,
    ADD_LIMIT
} AddStatus;

CodeStatus BSP_MATRIX_KEYBOARD_VERIF(char code_list[][CODE_LENGTH], int count, bool reset);
char BSP_MATRIX_KEYBOARD_DETECT (void);
AddStatus BSP_MATRIX_KEYBOARD_add_code(char code_list[][CODE_LENGTH], int *count, int max, const char *type_label);
bool BSP_MATRIX_KEYBOARD_delete_code(char code_list[][CODE_LENGTH], int *count, const char *type_label);

#endif /* BSP_CODE_CODE_H_ */
