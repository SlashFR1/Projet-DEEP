/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/
#include "stm32g4xx_hal.h"
#include "tft_ili9341/stm32g4_ili9341.h"


#include "config.h"
#if USE_FINGERPRINT
#include "stm32g4_utils.h"
#include "fingerprint.h"
#include "stm32g4_uart.h"
#include "stm32g4_gpio.h"
#include "stdio.h"
#include "stm32g4_systick.h"
#include <unistd.h>

#ifndef FINGERPRINT_UART_ID
	#define FINGERPRINT_UART_ID UART1_ID
#endif

void fingerprint_state_machine(void);

static volatile bool flag_answer_received = false;
static volatile bool flag_ack = false;
static volatile bool flag_ack_fail = false;
static volatile bool flag_cannot_detect_finger = false;
static volatile bool flag_fail_to_collect_finger = false;
static volatile confirmation_code_e confirmation_code;
static volatile uint32_t t = 0;
static volatile uint16_t page_id = 0;

void fingerprint_process_ms(void)
{
	if(t)
		t--;
}


void fingerprint_process_main(void)
{

	static frame_t frame;


	uint8_t c;
	add_fingerprint_state_machine();
	search_fingerprint_state_machine();

	if(BSP_UART_data_ready(FINGERPRINT_UART_ID))
	{
		c = BSP_UART_get_next_byte(FINGERPRINT_UART_ID);
		if(parse_frames(c, &frame))
		{
			if(frame.pid == PID_ACK)
			{
				flag_answer_received = true;
				confirmation_code = frame.datas[0];
				switch(frame.datas[0])
				{
					case 0x00:
							flag_ack = true;
							break;
					case 0x01:
							flag_ack_fail = true;
							break;
					case 0x02:
							flag_cannot_detect_finger = true;
							break;
					case 0x03:
							flag_fail_to_collect_finger = true;
							break;
					case 0x0a:
							flag_ack_fail = true;
							break;
					case 0x09:
							flag_ack_fail = true;
							break;
				}
			}
			for(int i=0; i<frame.length && i<256; i++)
				printf("%08x ", frame.datas[i]);

		}
	}
}
void fingerprint_init(void) {
	BSP_UART_init(FINGERPRINT_UART_ID, 57600);
	HAL_Delay(300);
	BSP_systick_add_callback_function(&fingerprint_process_ms);
/*	pageid = 0;
	bool mode;
	mode = HAL_GPIO_ReadPin(FINGERPRINT_MODE_SELECTOR_GPIO,
			FINGERPRINT_MODE_SELECTOR_PIN);
	fingerprint_set_mode_save_new_finger(false);

	for (int i = 0; i < MAX_PAGES; i++) {
		fingerprint_pages[i].pageid = i;
		fingerprint_pages[i].is_occupied = false;
		fingerprint_pages[i].nom_utilisateur[0] = '\0';*/
	}


void search_fingerprint_state_machine(void){
	typedef enum {
		INIT = 0,
		LED_ON,
		GET_IMAGE,
		IMAGE_TZ,
		SEARCH,
		FOUND,
		NOT_FOUND,
		FAIL
	} search_state_e;

	static search_state_e state = INIT;
	static search_state_e previous_state = INIT;
	bool entrance = (state != previous_state);
	previous_state = state;

	static bool img_index = 0;
	static uint8_t pageid;

	switch(state)
	{
		case INIT:
			ILI9341_Fill(ILI9341_COLOR_WHITE);
			ILI9341_Puts(60, 60, "Initialisation", &Font_16x26, ILI9341_COLOR_BLUE, ILI9341_COLOR_WHITE);

			BSP_UART_init(FINGERPRINT_UART_ID, 57600);
			HAL_Delay(300);
			BSP_systick_add_callback_function(&fingerprint_process_ms);
			pageid = 0;
			state = LED_ON;
			break;

		case LED_ON:
			HAL_Delay(2000);

			if (entrance) {
				send_command_withoutparameters(MODULE_DEFAULT_ADDRESS, OPEN_LED);
				t = 4000;
				printf("envoi commande : allume led - \n");
			}
			if (flag_answer_received) {
				flag_answer_received = false;
				state = (confirmation_code == CONFIRMATION_CODE_OK) ? GET_IMAGE : FAIL;
			}
			if (!t) state = FAIL;
			break;

		case GET_IMAGE:
			HAL_Delay(2000);
			ILI9341_Fill(ILI9341_COLOR_WHITE);
							ILI9341_Puts(60, 60, "recupere l'image", &Font_16x26, ILI9341_COLOR_BLUE, ILI9341_COLOR_WHITE);

			if (entrance) {
				send_command_withoutparameters(MODULE_DEFAULT_ADDRESS, GET_IMG);
				t = 4000;
			}
			if (flag_ack) {
				flag_ack = false;
				state = IMAGE_TZ;
			}
			if (flag_ack_fail || !t) {
				flag_ack_fail = false;
				state = FAIL;
			}
			break;

		case IMAGE_TZ:
			if (entrance) {
				send_Img2Tz(MODULE_DEFAULT_ADDRESS, 0x01 + img_index);
				t = 4000;
			}
			if (flag_ack) {
				flag_ack = false;
				state = SEARCH;
			}
			if (flag_ack_fail || !t) {
				flag_ack_fail = false;
				state = FAIL;
			}
			break;

		case SEARCH:
			HAL_Delay(2000);
			ILI9341_Fill(ILI9341_COLOR_WHITE);
							ILI9341_Puts(60, 60, "recherche", &Font_16x26, ILI9341_COLOR_BLUE, ILI9341_COLOR_WHITE);

			if (entrance) {
				send_search(MODULE_DEFAULT_ADDRESS, 0x01, 0x0001, pageid);
				t = 5000;
			}
			if (flag_ack) {
				flag_ack = false;
				state = FOUND;
			}
			if (flag_ack_fail) {
				flag_ack_fail = false;
				state = NOT_FOUND;
			}
			if (!t) state = FAIL;
			break;

		case FOUND:
			HAL_Delay(2000);
			ILI9341_Fill(ILI9341_COLOR_WHITE);
			ILI9341_Puts(60, 60, "image trouve", &Font_16x26, ILI9341_COLOR_BLUE, ILI9341_COLOR_WHITE);
			ILI9341_Puts(60, 90, "Porte ouverte", &Font_16x26, ILI9341_COLOR_BLUE, ILI9341_COLOR_WHITE);

			state = SEARCH;
			break;

		case NOT_FOUND:
			HAL_Delay(2000);
			ILI9341_Fill(ILI9341_COLOR_WHITE);
			ILI9341_Puts(60, 60, "image inconnu", &Font_16x26, ILI9341_COLOR_BLUE, ILI9341_COLOR_WHITE);
			ILI9341_Puts(60, 90, "Porte ferme", &Font_16x26, ILI9341_COLOR_BLUE, ILI9341_COLOR_WHITE);

			state = SEARCH;
			break;

		case FAIL:
			HAL_Delay(2000);
			ILI9341_Fill(ILI9341_COLOR_WHITE);
			ILI9341_Puts(60, 60, "echec", &Font_16x26, ILI9341_COLOR_BLUE, ILI9341_COLOR_WHITE);

			//state = GET_IMAGE;
			break;

		default:
			ILI9341_Puts(60, 60, "echec", &Font_16x26, ILI9341_COLOR_BLUE, ILI9341_COLOR_WHITE);
			break;
	}
}

void add_fingerprint_state_machine(void){
	typedef enum {
		INIT = 0,
		LED_ALLUME,
		GET_IMAGE,
		IMG_TZ,
		SEARCH,
		REG_MOD,
		STORE,
		FAIL
	} add_state_e;

	static add_state_e state = INIT;
	static add_state_e previous_state = INIT;
	bool entrance = (state != previous_state);
	previous_state = state;

	static bool img_index = 0;
	static uint8_t pageid;

	switch(state)
	{
		case INIT:
			BSP_UART_init(FINGERPRINT_UART_ID, 57600);
			HAL_Delay(300);
			BSP_systick_add_callback_function(&fingerprint_process_ms);
			pageid = 0;
			state = LED_ALLUME;
			break;

		case LED_ALLUME:
			HAL_Delay(2000);

			if (entrance) {
				send_command_withoutparameters(MODULE_DEFAULT_ADDRESS, OPEN_LED);
				t = 4000;
				printf("envoi commande : allume led - \n");
			}
			if (flag_answer_received) {
				flag_answer_received = false;
				state = (confirmation_code == CONFIRMATION_CODE_OK) ? GET_IMAGE : FAIL;
			}
			if (!t) {
				printf("TIME OUT Led-allume \n");
				state = FAIL;
			}
			break;

		case GET_IMAGE:
			HAL_Delay(2000);
			ILI9341_Fill(ILI9341_COLOR_WHITE);
							ILI9341_Puts(60, 60, "recupere l'image", &Font_16x26, ILI9341_COLOR_BLUE, ILI9341_COLOR_WHITE);

			if (entrance) {
				send_command_withoutparameters(MODULE_DEFAULT_ADDRESS, GET_IMG);
				t = 4000;
			}
			if (flag_ack) {
				flag_ack = false;
				state = IMG_TZ;
			}
			if (flag_ack_fail || !t) {
				flag_ack_fail = false;
				state = FAIL;
			}
			break;

		case IMG_TZ:
			if (entrance) {
				send_Img2Tz(MODULE_DEFAULT_ADDRESS, 0x01 + img_index);
				t = 4000;
			}
			if (flag_ack) {
				flag_ack = false;
				img_index = !img_index;
				state = SEARCH;
			}
			if (flag_ack_fail || !t) {
				flag_ack_fail = false;
				state = FAIL;
			}
			break;

		case SEARCH:
			HAL_Delay(2000);
			ILI9341_Fill(ILI9341_COLOR_WHITE);
							ILI9341_Puts(60, 60, "recherche l'image", &Font_16x26, ILI9341_COLOR_BLUE, ILI9341_COLOR_WHITE);

			if (entrance) {
				send_search(MODULE_DEFAULT_ADDRESS, 0x01, 0x0001, pageid);
				t = 5000;
			}
			if (flag_ack || flag_ack_fail) {
				flag_ack = flag_ack_fail = false;
				if (!HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13)) {
					state = REG_MOD;
				}
			}
			if (!t) state = FAIL;
			break;

		case REG_MOD:
			if (entrance) {
				send_command_withoutparameters(MODULE_DEFAULT_ADDRESS, REG_MODEL);
				t = 3000;
			}
			if (flag_ack) {
				flag_ack = false;
				state = STORE;
			}
			if (flag_ack_fail || !t) {
				flag_ack_fail = false;
				state = FAIL;
			}
			break;

		case STORE:
			HAL_Delay(2000);
			ILI9341_Fill(ILI9341_COLOR_WHITE);
							ILI9341_Puts(60, 60, "enregistrement du doigt", &Font_16x26, ILI9341_COLOR_BLUE, ILI9341_COLOR_WHITE);

			if (entrance) {
				send_store(MODULE_DEFAULT_ADDRESS, 0x01, pageid);
				printf("Demande d'enregistrement à la pageid = %d\n", pageid);
				pageid++;
				t = 8000;

			}
			if (flag_ack) {
				flag_ack = false;
				ILI9341_Puts(60, 60, "enregistrement reussi", &Font_16x26, ILI9341_COLOR_BLUE, ILI9341_COLOR_WHITE);
				state = GET_IMAGE;
			}
			if (flag_ack_fail || !t) {
				flag_ack_fail = false;
				ILI9341_Puts(60, 60, "enregistrement fail", &Font_16x26, ILI9341_COLOR_BLUE, ILI9341_COLOR_WHITE);
				state = FAIL;
			}
			break;

		case FAIL:
			HAL_Delay(2000);
			ILI9341_Fill(ILI9341_COLOR_WHITE);
							ILI9341_Puts(60, 60, "fail ", &Font_16x26, ILI9341_COLOR_BLUE, ILI9341_COLOR_WHITE);
							HAL_Delay(3000);
							ILI9341_Puts(60, 110, "Recommencer ", &Font_16x26, ILI9341_COLOR_BLUE, ILI9341_COLOR_WHITE);
			if (entrance) t = 500;
			//if (!t) state = GET_IMAGE;
			break;

		default:
			break;
	}
}









void send_setPwd(uint32_t address, uint32_t password)
{
	frame_t frame;
	frame.addr = address;
	frame.pid = PID_COMMAND;
	frame.length = 2+5;
	frame.datas[0] = 0x12;
	frame.datas[1] = (uint8_t)((password>>24)&0xFF);
	frame.datas[2] = (uint8_t)((password>>16)&0xFF);
	frame.datas[3] = (uint8_t)((password>>8)&0xFF);
	frame.datas[4] = (uint8_t)((password)&0xFF);
	send_command(&frame);
}


void send_genImg(uint32_t address, uint32_t password)
{
	frame_t frame;
	frame.addr = address;
	frame.pid = PID_COMMAND;
	frame.length = 2+1;
	frame.datas[0] = 0x01;
	send_command(&frame);
}


void send_Img2Tz(uint32_t address, uint8_t buffer_id){
	frame_t frame;
	frame.addr = address;
	frame.pid = PID_COMMAND;
	frame.length = 2+2;
	frame.datas[0] = 0x02;
	frame.datas[1] = buffer_id;
	send_command(&frame);
}

void send_store(uint32_t address, uint8_t buffer_id, uint16_t page_id){
	frame_t frame;
	frame.addr = address;
	frame.pid = PID_COMMAND;
	frame.length = 2+4;
	frame.datas[0] = 0x06;
	frame.datas[1] = buffer_id;
	frame.datas[2] = (uint8_t)((page_id>>8)&0xFF);
	frame.datas[3] = (uint8_t)((page_id)&0xFF);
	send_command(&frame);
}

void send_command_withoutparameters(uint32_t address, uint8_t command_id)
{
	frame_t frame;
	frame.addr = address;
	frame.pid = PID_COMMAND;
	frame.length = 2+1;
	frame.datas[0] = command_id;
	send_command(&frame);
}

void send_search(uint32_t address, uint8_t buffer_id, uint16_t start_page, uint16_t page_num)
{
	frame_t frame;
	frame.addr = address;
	frame.pid = PID_COMMAND;
	frame.length = 2+6;
	frame.datas[0] = 0x04;
	frame.datas[1] = buffer_id;
	frame.datas[2] = (uint8_t)((start_page>>8)&0xFF);
	frame.datas[3] = (uint8_t)((start_page)&0xFF);
	frame.datas[4] = (uint8_t)((page_num>>8)&0xFF);
	frame.datas[5] = (uint8_t)((page_num)&0xFF);
	send_command(&frame);
}




bool parse_frames(uint8_t byte, frame_t * pframe)
{
	typedef enum
	{
		STEP_HEADER_WAIT_EF,
		STEP_HEADER_WAIT_01,
		STEP_ADDR0,
		STEP_ADDR1,
		STEP_ADDR2,
		STEP_ADDR3,
		STEP_PID,
		STEP_LENGTH0,
		STEP_LENGTH1,
		STEP_DATAS,
		STEP_CS0,
		STEP_CS1
	}step_e;

	static step_e step = STEP_HEADER_WAIT_EF;
	static uint16_t size_datas = 0;
	static uint16_t checksum;
	static uint16_t sum;

	bool ret;
	ret = false;
	if(step==STEP_PID)
		sum = byte;
	else if(step == STEP_LENGTH0 || step == STEP_LENGTH1 || step == STEP_DATAS)
		sum = sum + ((uint16_t)(byte));

	switch(step)
	{
		case STEP_HEADER_WAIT_EF:
			if(byte == 0xEF)
				step = STEP_HEADER_WAIT_01;
			break;
		case STEP_HEADER_WAIT_01:
			if(byte == 0x01)
				step = STEP_ADDR0;
			else
				step = STEP_HEADER_WAIT_EF;
			break;
		case STEP_ADDR0:
			pframe->addr = (uint32_t)byte<<24;
			step++;
			break;
		case STEP_ADDR1:
			pframe->addr |= (uint32_t)byte<<16;
			step++;
			break;
		case STEP_ADDR2:
			pframe->addr |= (uint32_t)byte<<8;
			step++;
			break;
		case STEP_ADDR3:
			pframe->addr |= (uint32_t)byte;
			step++;
			break;
		case STEP_PID:
			if(byte == PID_COMMAND || byte == PID_DATA || byte == PID_ACK || byte ==PID_END)
			{
				pframe->pid = byte;
				printf("pid OK=%d\n", pframe->pid);
			}
			else
			{
				printf("pid KO !!!=%d\n", pframe->pid);
				pframe->pid = PID_UNKNOW;
			}
			step++;
			break;
		case STEP_LENGTH0:
			pframe->length = U16FROMU8(byte, 0);
			step++;
			break;
		case STEP_LENGTH1:
			pframe->length |= ((uint16_t)(byte));

			if(pframe->length <= 256)
			{
				size_datas = 0;
				step = STEP_DATAS;
				printf("length=%d\n", pframe->length);
			}
			else
			{
				printf("mauvais length=%d\n", pframe->length);
				step = STEP_HEADER_WAIT_EF;
			}
			break;
		case STEP_DATAS:
			pframe->datas[size_datas] = byte;
			size_datas++;
			if(size_datas >= pframe->length - 2)
				step = STEP_CS0;
			break;
		case STEP_CS0:
			checksum = ((uint16_t)(byte))<<8;
			step++;
			break;
		case STEP_CS1:
			checksum |= ((uint16_t)(byte));
			if(checksum == sum)
			{
				ret = true;
			}
			else
			{
				printf("mauvais checksum sum=%d, checksum=%d!\n", sum, checksum);
			}
			step = STEP_HEADER_WAIT_EF;
			break;
		default:
			break;
	}
	return ret;
}

void send_command(frame_t * pframe)
{
	uint8_t buf[300];
	uint16_t size = 0;
	buf[size++] = 0xEF;
	buf[size++] = 0x01;
	buf[size++] = (char)(pframe->addr>>24)&0xFF;
	buf[size++] = (char)(pframe->addr>>16)&0xFF;
	buf[size++] = (char)(pframe->addr>>8)&0xFF;
	buf[size++] = (char)(pframe->addr)&0xFF;
	buf[size++] = (pframe->pid);
	buf[size++] = (char)(pframe->length>>8)&0xFF;
	buf[size++] = (char)(pframe->length)&0xFF;
	for(int i = 0; i<pframe->length-2; i++)
		buf[size++] = pframe->datas[i];
	uint16_t sum = 0;
	for(int i = 6; i<6+1+pframe->length; i++)
	{
		sum+= (uint16_t)buf[i];
	}
	buf[size++] = (char)(sum>>8)&0xFF;
	buf[size++] = (char)(sum)&0xFF;
	BSP_UART_puts(FINGERPRINT_UART_ID, buf, size);
}

#endif



