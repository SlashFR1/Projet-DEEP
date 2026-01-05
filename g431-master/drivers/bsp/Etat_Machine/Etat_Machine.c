/*
 * Etat_Machine.c
 *
 *  Created on: May 14, 2025
 *      Author: arthurloeul
 */
#include "tft_ili9341/stm32g4_ili9341.h"
#include "MatrixKeyboard/stm32g4_matrix_keyboard.h"
#include "GACHE/gache.h"
#include "RFID/RFID.h"
#include "Etat_Machine/Etat_Machine.h"
#include "CODE/code.h"


#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define BLINK_DELAY		100	//ms



extern char admin_codes[MAX_ADMIN_CODES][CODE_LENGTH];
extern int admin_count;

extern char livreur_codes[MAX_LIVREUR_CODES][CODE_LENGTH];
extern int livreur_count;

extern uint8_t rfid_cards[MAX_RFID_CARDS][UID_MAX_LENGTH];
extern uint8_t rfid_card_sizes[MAX_RFID_CARDS];
extern int rfid_card_count;



void stateMachine(void){

	static State currentState = IDLE;

	switch (currentState){
		case IDLE :

			static bool messagePrinted = false;
			if (!messagePrinted) {
				ILI9341_Fill(ILI9341_COLOR_WHITE);
						ILI9341_Puts(60, 60, "Bonjour", &Font_16x26, ILI9341_COLOR_BLUE, ILI9341_COLOR_WHITE);
				        ILI9341_Puts(30, 100, "Boite aux lettres", &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
				        ILI9341_Puts(30, 120, "Arthur & Louis", &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
			        messagePrinted = true;
			}

			char key = BSP_MATRIX_KEYBOARD_DETECT();

			if (BSP_NFC03A1_DETECTE()) {
			    currentState = RFID;
			    messagePrinted = false;
			}

			if (key == 'A') {
			    currentState = ADMIN_ENTRY;
			    messagePrinted = false;
			}

			if (key == '*') {
			    currentState = CODE_ENTRY;
			    messagePrinted = false;
			}
			if (key == '#') {
						    currentState = ADMIN_FP;
						    messagePrinted = false;
						}

//				else if (BSP_MATRIX_KEYBOARD_DETECT() == true){
//					currentState = EMPREINTE;
//				}
			break;

		case RFID :

			ECRAN_AfficherMessage("Carte RFID d�tect�");

			if (BSP_NFC03A1_VERIF() == true){
				ECRAN_AfficherMessage("Carte RFID BONNE");

				currentState = GATE_OPEN;
			}
			else{
				ECRAN_AfficherMessage("Carte RFID non reconnue");
				currentState = IDLE;
			}
			break;

		case CODE_ENTRY :

			CodeStatus status = BSP_MATRIX_KEYBOARD_VERIF(livreur_codes, livreur_count, false);

			if (status == CODE_CORRECT) {
				HAL_Delay(900);
				ILI9341_Fill(ILI9341_COLOR_WHITE);
				ILI9341_Puts(40, 100, "Code correct", &Font_16x26, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
				currentState = GATE_OPEN;
			}
			else if (status == CODE_INCORRECT) {
				HAL_Delay(900);
				ILI9341_Fill(ILI9341_COLOR_WHITE);
				ILI9341_Puts(40, 100, "Code incorrect", &Font_16x26, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
				currentState = IDLE;
			}
			break;

		case GATE_OPEN :

			ILI9341_Fill(ILI9341_COLOR_WHITE);
			ILI9341_Puts(40, 100, "Ouverture porte", &Font_16x26,ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);

			uint32_t startTime = HAL_GetTick();

			GACHE_Activer();

			while ((HAL_GetTick() - startTime) < 30000) {
				char key2 = BSP_MATRIX_KEYBOARD_DETECT();
				if (key2 == '#') {
					break;  // Sortir d�s que '#' est press�
				}
				HAL_Delay(10);  // Petite pause pour ne pas surcharger
			}

			currentState = GATE_CLOSE;
			break;

		case GATE_CLOSE :

			ILI9341_Fill(ILI9341_COLOR_WHITE);
			ILI9341_Puts(40, 100, "FERMETURE PORTE", &Font_16x26,ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
			HAL_Delay(500);

			GACHE_Desactiver();

			currentState = IDLE;
			break;

		case ADMIN_ENTRY :

			status = BSP_MATRIX_KEYBOARD_VERIF(admin_codes, admin_count, false);

			if (status == CODE_CORRECT) {
				HAL_Delay(900);
				ILI9341_Fill(ILI9341_COLOR_WHITE);
				ILI9341_Puts(40, 100, "Code correct", &Font_16x26, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
				currentState = ADMIN_MODE;
			}
			else if (status == CODE_INCORRECT) {
				HAL_Delay(900);
				ILI9341_Fill(ILI9341_COLOR_WHITE);
				ILI9341_Puts(40, 100, "Code incorrect", &Font_16x26, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
				currentState = IDLE;
			}
			break;

		case ADMIN_MODE :

			static bool messagePrinted3 = false;
			if (!messagePrinted3) {
					ILI9341_Fill(ILI9341_COLOR_WHITE);
					ILI9341_Puts(20, 70, "1 - Livreur", &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
					ILI9341_Puts(20, 100, "2 - ADMIN", &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
					ILI9341_Puts(20, 130, "3 - RFID", &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
					ILI9341_Puts(20, 160, "A - Retourner au menu IDLE", &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
					messagePrinted3 = true;
				}

			char adminKey = BSP_MATRIX_KEYBOARD_DETECT();

			if (adminKey == '1') {
				currentState = LIVREUR_CODE;
				messagePrinted3 = false;
			} else if (adminKey == '2') {
				currentState = ADMIN_CODE;
				messagePrinted3 = false;
			} else if (adminKey == '3'){
				currentState = ADMIN_RFID;
				messagePrinted3 = false;
			} else if (adminKey == 'A') {
				currentState = IDLE;
				messagePrinted3 = false;
			}
			break;


		case LIVREUR_CODE :
			static bool messagePrinted2 = false;
			if (!messagePrinted2) {
					ILI9341_Fill(ILI9341_COLOR_WHITE);
					ILI9341_Puts(20, 70, "LIVREUR", &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
					ILI9341_Puts(10, 100, "1 - Ajouter un code Livreur", &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
					ILI9341_Puts(10, 130, "2 - Supprimer un code Livreur", &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
					ILI9341_Puts(10, 160, "A - Retourner au menu IDLE Livreur", &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
					messagePrinted2 = true;
				}

			char clavierKey = BSP_MATRIX_KEYBOARD_DETECT();

			if (clavierKey == '1') {
				currentState = LIVREUR_ADD;
				messagePrinted2 = false;
			} else if (clavierKey == '2') {
				currentState = LIVREUR_DELETE;
				messagePrinted2 = false;
			} else if (clavierKey == 'A') {
				currentState = IDLE;
				messagePrinted2 = false;
			}
			break;

		case ADMIN_CODE:
			static bool messagePrinted4 = false;
			if (!messagePrinted4) {
					ILI9341_Fill(ILI9341_COLOR_WHITE);
					ILI9341_Puts(20, 70, "ADMIN", &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
					ILI9341_Puts(20, 100, "1 - Ajouter un code", &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
					ILI9341_Puts(20, 130, "2 - Supprimer un code", &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
					ILI9341_Puts(20, 160, "A - Retourner au menu IDLE", &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
					messagePrinted4 = true;
				}
			char adminCodeKey = BSP_MATRIX_KEYBOARD_DETECT();

			if (adminCodeKey == '1') {
				currentState = ADMIN_ADD;
				messagePrinted4 = false;
			} else if (adminCodeKey == '2') {
				currentState = ADMIN_DELETE;
				messagePrinted4 = false;
			} else if (adminCodeKey == 'A') {
				currentState = IDLE;
				messagePrinted4 = false;
			}
			break;

		case ADMIN_ADD :

			AddStatus status_add_admin = BSP_MATRIX_KEYBOARD_add_code(admin_codes, &admin_count, MAX_ADMIN_CODES, "Code Admin");

			if (status_add_admin == ADD_CODE || status_add_admin == ADD_LIMIT) {
				currentState = ADMIN_CODE;
			}

			char admin_add_key = BSP_MATRIX_KEYBOARD_DETECT();

			if (admin_add_key == 'A') {
				currentState = IDLE;
			}
			break;

		case ADMIN_DELETE :
			if (BSP_MATRIX_KEYBOARD_delete_code(admin_codes, &admin_count, "Code ADMIN")) {
				currentState = ADMIN_CODE;
			}

			char admin_delete_key = BSP_MATRIX_KEYBOARD_DETECT();

			if (admin_delete_key == 'A') {
				currentState = IDLE;
			}
			break;

		case LIVREUR_ADD :

			AddStatus status_add_livreur = BSP_MATRIX_KEYBOARD_add_code(livreur_codes, &livreur_count, MAX_LIVREUR_CODES, "Code Livreur"); //faire uyn switch avec une variable pour enlever les variable global en haut du main
																																			// dans la fonction
			if (status_add_livreur == ADD_CODE || status_add_livreur == ADD_LIMIT) {
				currentState = LIVREUR_CODE;
			}

			char clavier_add_key = BSP_MATRIX_KEYBOARD_DETECT();

			if (clavier_add_key == 'A') {
				currentState = IDLE;
			}
			break;


		case LIVREUR_DELETE :
			if (BSP_MATRIX_KEYBOARD_delete_code(livreur_codes, &livreur_count, "Code Livreur")){
				currentState = LIVREUR_CODE;
			}

			char clavier_delete_key = BSP_MATRIX_KEYBOARD_DETECT();

			if (clavier_delete_key == 'A') {
				currentState = IDLE;
			}
			break;

		case ADMIN_RFID :
			static _Bool messagePrinted5 = false;
			if (!messagePrinted5) {
				ILI9341_Fill(ILI9341_COLOR_WHITE);
				ILI9341_Puts(20, 70, "RFID", &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
				ILI9341_Puts(20, 100, "1 - Ajouter une carte", &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
				ILI9341_Puts(20, 130, "2 - Supprimer une carte", &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
				ILI9341_Puts(20, 160, "A - Retourner au menu IDLE", &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
				messagePrinted5 = true;
			}

			char RFID_CodeKey = BSP_MATRIX_KEYBOARD_DETECT();

			if (RFID_CodeKey == '1') {
				currentState = RFID_ADD;
				messagePrinted5 = false;
			} else if (RFID_CodeKey == '2') {
				currentState = RFID_DELETE;
				messagePrinted5 = false;
			} else if (RFID_CodeKey == 'A') {
				currentState = IDLE;
				messagePrinted5 = false;
			}
			break;

		case RFID_ADD :

			ILI9341_Fill(ILI9341_COLOR_WHITE);
			ILI9341_Puts(20, 70, "Presentez la carte a ajouter", &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);

			HAL_Delay(1000);

			if (BSP_NFC03A1_ADD()) {
				ILI9341_Fill(ILI9341_COLOR_WHITE);
				ILI9341_Puts(20, 70, "Carte ajoutee", &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
			} else {
				ILI9341_Fill(ILI9341_COLOR_WHITE);
				ILI9341_Puts(20, 70, "Erreur ou doublon", &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
			}

			HAL_Delay(1000);
			currentState = ADMIN_RFID;

			break;

		case RFID_DELETE :

			ILI9341_Fill(ILI9341_COLOR_WHITE);
			ILI9341_Puts(20, 70, "Presentez la carte � supprimer", &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);

			HAL_Delay(1000);

			if (BSP_NFC03A1_DELETE()) {
				ILI9341_Fill(ILI9341_COLOR_WHITE);
				ILI9341_Puts(20, 70, "Carte supprimee", &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
			} else {
				ILI9341_Fill(ILI9341_COLOR_WHITE);
				ILI9341_Puts(20, 70, "Carte non trouvee", &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
			}

			HAL_Delay(1000);
			currentState = ADMIN_RFID;

			break;





		case ADMIN_FP :
			static _Bool messagePrinted8 = false;
				if (!messagePrinted8) {
					ILI9341_Fill(ILI9341_COLOR_WHITE);
					ILI9341_Puts(20, 70, "Empreinte", &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
					ILI9341_Puts(20, 100, "1 - Ajouter une empreinte", &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
					ILI9341_Puts(20, 130, "2 - Ouvrir la porte", &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
					ILI9341_Puts(20, 160, "3 - Supprimer une empreinte", &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
					ILI9341_Puts(20, 190, "A - Retourner au menu IDLE", &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
						messagePrinted8 = true;
					}

					char FP_CodeKey = BSP_MATRIX_KEYBOARD_DETECT();

					if (FP_CodeKey == '1') {
						currentState = FP_ADD;
						messagePrinted8 = false;
					} else if (FP_CodeKey == '2') {
						currentState = FP_OPEN;
						messagePrinted8 = false;
					} else if (FP_CodeKey == '3') {
						currentState = FP_DELETE;
						messagePrinted8 = false;
					} else if (FP_CodeKey == 'A') {
						currentState = IDLE;
						messagePrinted8 = false;
					}

			break;

		case FP_ADD :

            ILI9341_Fill(ILI9341_COLOR_WHITE);
            ILI9341_Puts(20, 70, "Presentez le doigt a ajouter", &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);

            add_fingerprint_state_machine();
            currentState = ADMIN_FP;
            fingerprint_process_main();

            break;
		case FP_OPEN :
            ILI9341_Fill(ILI9341_COLOR_WHITE);
            ILI9341_Puts(20, 70, "Presentez le doigt", &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);

            search_fingerprint_state_machine();
            currentState = ADMIN_FP;
            fingerprint_process_main();

            break;

	/*	case FP_DELETEHAL_Delay(1000);                                                                                                :

			fingerprint_process_main();

			ILI9341_Fill(ILI9341_COLOR_WHITE);
			ILI9341_Puts(20, 70, "Presentez le doigt a supprimer", &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);

			HAL_Delay(1000);

			if (fingerprint_delete()) {
				ILI9341_Fill(ILI9341_COLOR_WHITE);
				ILI9341_Puts(20, 70, "Empreinte supprimee", &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
			} else {
				ILI9341_Fill(ILI9341_COLOR_WHITE);
				ILI9341_Puts(20, 70, "Erreur, empreinte introuvable", &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
			}

			currentState = ADMIN_FP;

			break;*/

		default :
			break;


	}
}


