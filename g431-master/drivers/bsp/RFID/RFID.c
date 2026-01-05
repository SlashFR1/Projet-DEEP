#include "NFC03A1/stm32g4_nfc03a1.h"
#include "config.h"
#include "RFID/RFID.h"

#include <stdio.h>
#include <string.h>

uint8_t rfid_cards[MAX_RFID_CARDS][UID_MAX_LENGTH];
uint8_t rfid_card_sizes[MAX_RFID_CARDS];
int rfid_card_count = 0;

static bool local_initialized = false;

bool BSP_NFC03A1_DETECTE(void) {

	if(!local_initialized){
		BSP_NFC03A1_Init(PCD);
		local_initialized = true;
	}

	uint8_t tag;
	tag = ConfigManager_TagHunting(TRACK_ALL);
	switch(tag){
		case TRACK_NFCTYPE1 :
			return true;
			break;
		case TRACK_NFCTYPE2 :
			return true;
			break;
		case TRACK_NFCTYPE3 :
			return true;
			break;
		case TRACK_NFCTYPE4A:
			return true;
			break;
		case TRACK_NFCTYPE4B:
			return true;
			break;
		case TRACK_NFCTYPE5 :
			return true;
		default :
			return false;
			break;
	}
}

bool BSP_NFC03A1_VERIF(void){
	uint8_t tag;
	//uint8_t uid_reference[] = {0x04, 0x67, 0x45, 0x92, 0x83, 0x19, 0x90}; // Carte ESEO Arthur Loeul
	//uint8_t uid_ref_size = sizeof(uid_reference) / sizeof(uid_reference[0]);

	tag = ConfigManager_TagHunting(TRACK_ALL);
	switch(tag)
			{
				case TRACK_NFCTYPE4A:{

					ISO14443A_CARD infos;
					BSP_NFC03A1_get_ISO14443A_infos(&infos);

					printf("uid detecté = ");

					for(uint8_t i=0; i<infos.UIDsize;i++){
						printf("%02x ",infos.UID[i]);
					}
					printf("\n");

					for (int i = 0; i < rfid_card_count; i++) {
						if (infos.UIDsize == rfid_card_sizes[i] &&
						    memcmp(infos.UID, rfid_cards[i], infos.UIDsize) == 0) {
							return true;
						}
					}
					break;}

				default:
					break;
			}
	return false;
}

bool BSP_NFC03A1_ADD(void) {

	if(!local_initialized){
		BSP_NFC03A1_Init(PCD);
		local_initialized = true;
	}

	if (rfid_card_count >= MAX_RFID_CARDS) return false;

	uint8_t tag = ConfigManager_TagHunting(TRACK_ALL);
	if (tag == TRACK_NFCTYPE4A) {
		ISO14443A_CARD infos;
		BSP_NFC03A1_get_ISO14443A_infos(&infos);

		// Éviter les doublons
		for (int i = 0; i < rfid_card_count; i++) {
			if (infos.UIDsize == rfid_card_sizes[i] &&
			    memcmp(infos.UID, rfid_cards[i], infos.UIDsize) == 0) {
				return false; // déjà enregistrée
			}
		}

		memcpy(rfid_cards[rfid_card_count], infos.UID, infos.UIDsize);
		rfid_card_sizes[rfid_card_count] = infos.UIDsize;
		rfid_card_count++;
		return true;
	}
	return false;
}

bool BSP_NFC03A1_DELETE(void) {

	if(!local_initialized){
		BSP_NFC03A1_Init(PCD);
		local_initialized = true;
	}

	uint8_t tag = ConfigManager_TagHunting(TRACK_ALL);
	if (tag == TRACK_NFCTYPE4A) {
		ISO14443A_CARD infos;
		BSP_NFC03A1_get_ISO14443A_infos(&infos);

		for (int i = 0; i < rfid_card_count; i++) {
			if (infos.UIDsize == rfid_card_sizes[i] &&
			    memcmp(infos.UID, rfid_cards[i], infos.UIDsize) == 0) {
				// Décaler les suivantes
				for (int j = i; j < rfid_card_count - 1; j++) {
					memcpy(rfid_cards[j], rfid_cards[j + 1], UID_MAX_LENGTH);
					rfid_card_sizes[j] = rfid_card_sizes[j + 1];
				}
				rfid_card_count--;
				return true;
			}
		}
	}
	return false;
}
