/*
 * RFID.h
 *
 *  Created on: May 14, 2025
 *      Author: arthurloeul
 */

#ifndef BSP_RFID_RFID_H_
#define BSP_RFID_RFID_H_

#define MAX_RFID_CARDS 4
#define UID_MAX_LENGTH 10

extern uint8_t rfid_cards[MAX_RFID_CARDS][UID_MAX_LENGTH];
extern uint8_t rfid_card_sizes[MAX_RFID_CARDS];
extern int rfid_card_count;

bool BSP_NFC03A1_VERIF(void);

bool BSP_NFC03A1_DETECTE(void);

bool BSP_NFC03A1_DELETE(void);

bool BSP_NFC03A1_ADD(void);



#endif /* BSP_RFID_RFID_H_ */
