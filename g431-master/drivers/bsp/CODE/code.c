/*
 * code.c
 *
 *  Created on: May 14, 2025
 *      Author: arthurloeul
 */
#include "MatrixKeyboard/stm32g4_matrix_keyboard.h"
#include "tft_ili9341/stm32g4_ili9341.h"
#include "CODE/code.h"
#include "stm32g4_systick.h"
#include <stdbool.h>
#include <stdio.h>

extern char admin_codes[MAX_ADMIN_CODES][CODE_LENGTH] = {
    {'2','0','0','3'},
    {0}
};
extern int admin_count = 1;

extern char livreur_codes[MAX_LIVREUR_CODES][CODE_LENGTH] = {
		{'2','4','1','0'},
		{0}
};
extern int livreur_count = 1;

static bool local_initialized = false;

char BSP_MATRIX_KEYBOARD_DETECT(void){
	static char last_key = 0;
	char press_key_event;
	uint32_t all_touch_pressed;

	if (!local_initialized) {
		BSP_systick_add_callback_function(BSP_MATRIX_KEYBOARD_demo_process_1ms);
		BSP_MATRIX_KEYBOARD_init(default_keyboard_keys);
		local_initialized = true;
	}

	if (!t) {
			t = 10;
			BSP_MATRIX_KEYBOARD_press_and_release_events(&press_key_event, NULL, &all_touch_pressed);

			if (press_key_event != NO_KEY && press_key_event != MANY_KEYS && press_key_event != last_key) {
				last_key = press_key_event;
				return press_key_event;
			}

			if (press_key_event == NO_KEY) {
				last_key = 0;
			}
		}

		return 0;
}

CodeStatus BSP_MATRIX_KEYBOARD_VERIF(char code_list[][CODE_LENGTH], int count, bool reset)
{
	static char raw_input[5];  // pour affichage
	static char input_code[CODE_LENGTH];  // pour vérif
	static uint8_t index = 0;
	static bool code_started = false;
	static bool screen_initialized = false;

	if (reset) {
		index = 0;
		code_started = false;
		screen_initialized = false;
		for (int i = 0; i < CODE_LENGTH + 1; i++) {
			input_code[i] = 0;
		}
		return CODE_NOT_READY;  // valeur spéciale pour indiquer un reset simple
	}

	if (!local_initialized) {
		BSP_systick_add_callback_function(BSP_MATRIX_KEYBOARD_demo_process_1ms);
		BSP_MATRIX_KEYBOARD_init(default_keyboard_keys);
		local_initialized = true;
	}

	if (!t) {
		t = 10;

		char press_key_event;
		uint32_t all_touch_pressed;
		BSP_MATRIX_KEYBOARD_press_and_release_events(&press_key_event, NULL, &all_touch_pressed);

		if (press_key_event != NO_KEY && press_key_event != MANY_KEYS) {
			printf("Key: %c\n", press_key_event);

			if (!code_started && press_key_event == '*') {
			    code_started = true;
			    index = 0;
			    raw_input[0] = '*';
			    ILI9341_Fill(ILI9341_COLOR_WHITE);
			    ILI9341_Puts(50, 40, "ENTRER LE CODE", &Font_16x26, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
			    screen_initialized = true;
			}
			else if (code_started && index < CODE_LENGTH) {
			    input_code[index] = press_key_event;
			    raw_input[index + 1] = press_key_event;
			    index++;
			}

			if (code_started) {
			    char x_display[6];
			    for (int i = 0; i < 5; i++) {
			        x_display[i] = (i <= index) ? 'X' : '_';
			    }
			    x_display[5] = '\0';
			    ILI9341_Puts(80, 120, x_display, &Font_16x26, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
			}


			if (index == CODE_LENGTH) {
			    bool match = false;
			    for (int i = 0; i < count; i++) {
			        bool same = true;
			        for (int j = 0; j < CODE_LENGTH; j++) {
			            if (code_list[i][j] != input_code[j]) {
			                same = false;
			                break;
			            }
			        }
			        if (same) {
			            match = true;
			            break;
			        }
			    }

			    index = 0;
			    code_started = false;
			    return match ? CODE_CORRECT : CODE_INCORRECT;
			}
		}
	}

	return CODE_NOT_READY;
}


AddStatus BSP_MATRIX_KEYBOARD_add_code(char code_list[][CODE_LENGTH], int *count, int max, const char *type_label)
{
	static char new_code[CODE_LENGTH];
	static uint8_t index = 0;
	static bool started = false;

	if (*count >= max) {
		ILI9341_Fill(ILI9341_COLOR_WHITE);
		ILI9341_Puts(20, 160, "Limite atteinte", &Font_11x18, ILI9341_COLOR_RED, ILI9341_COLOR_WHITE);
        index = 0;
        started = false;
		return ADD_LIMIT;
	}

	if (!started) {
		ILI9341_Fill(ILI9341_COLOR_WHITE);
		char msg[32];
		sprintf(msg, "Ajout %s (commencer par *)", type_label);
		ILI9341_Puts(20, 40, msg, &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
		ILI9341_Puts(80, 100, "_____", &Font_16x26, ILI9341_COLOR_GRAY, ILI9341_COLOR_WHITE);
		index = 0;
		started = true;
		return ADD_NOT_READY;
	}

	char press_key_event;
	BSP_MATRIX_KEYBOARD_press_and_release_events(&press_key_event, NULL, NULL);

	if (press_key_event != NO_KEY && press_key_event != MANY_KEYS) {
		if (index == 0) {
			if (press_key_event != '*') return ADD_NOT_READY;
			index = 1;
		} else if (index <= CODE_LENGTH) {
			new_code[index - 1] = press_key_event;
			index++;
		}

		// Affichage état code
		char buf[6];
		for (int i = 0; i < 5; i++) buf[i] = (i < index) ? 'X' : '_';
		buf[5] = '\0';
		ILI9341_Puts(80, 100, buf, &Font_16x26, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);

		if (index == CODE_LENGTH + 1) {
			// Stockage
			for (int i = 0; i < CODE_LENGTH; i++) {
				code_list[*count][i] = new_code[i];
			}
			(*count)++;

			// Affichage visuel
			char code_affiche[CODE_LENGTH + 1];
			for (int i = 0; i < CODE_LENGTH; i++) {
				code_affiche[i] = new_code[i];
			}
			code_affiche[CODE_LENGTH] = '\0';

			ILI9341_Fill(ILI9341_COLOR_WHITE);
			ILI9341_Puts(20, 80, "Code ajoute :", &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
			ILI9341_Puts(80, 110, code_affiche, &Font_16x26, ILI9341_COLOR_BLUE, ILI9341_COLOR_WHITE);
			ILI9341_Puts(20, 160, "Ajout reussi", &Font_11x18, ILI9341_COLOR_GREEN, ILI9341_COLOR_WHITE);
			HAL_Delay(1000);

			index = 0;
			started = false;
			return ADD_CODE;
		}
	}

	return ADD_NOT_READY;
}

bool BSP_MATRIX_KEYBOARD_delete_code(char code_list[][CODE_LENGTH], int *count, const char *type_label) {
    if (*count == 0) {
        ILI9341_Fill(ILI9341_COLOR_WHITE);
        ILI9341_Puts(20, 80, "Aucun code a supprimer", &Font_11x18, ILI9341_COLOR_RED, ILI9341_COLOR_WHITE);
        HAL_Delay(1000);


        return false;
    }

    static bool started = false;
    static int selected = -1;

    if (!started) {
        ILI9341_Fill(ILI9341_COLOR_WHITE);
        ILI9341_Puts(20, 20, "Codes disponibles:", &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);

        for (int i = 0; i < *count; i++) {
            char line[32];
            snprintf(line, sizeof(line), "%d: %c%c%c%c", i+1, code_list[i][0], code_list[i][1], code_list[i][2], code_list[i][3]);
            ILI9341_Puts(20, 50 + i * 20, line, &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
        }

        ILI9341_Puts(20, 160, "Choisir: 1-2-3-4", &Font_11x18, ILI9341_COLOR_BLUE, ILI9341_COLOR_WHITE);
        started = true;
        return false;
    }

    char key = BSP_MATRIX_KEYBOARD_DETECT();
    if (key >= '1' && key <= '0' + *count) {
        selected = key - '1';

        // Suppression logique
        for (int i = selected; i < *count - 1; i++) {
            for (int j = 0; j < CODE_LENGTH; j++) {
                code_list[i][j] = code_list[i + 1][j];
            }
        }

        (*count)--;

        ILI9341_Fill(ILI9341_COLOR_WHITE);
        ILI9341_Puts(20, 80, "Code supprime", &Font_11x18, ILI9341_COLOR_GREEN, ILI9341_COLOR_WHITE);
        HAL_Delay(1000);

        started = false;
        return true;
    } else if (key == 'A') {
        started = false;
        return true; // Retourner au menu
    }

    return false;
}

