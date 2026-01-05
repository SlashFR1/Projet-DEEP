/**
 *******************************************************************************
 * @file	gache.c
 * @author	vchav
 * @date	Jun 10, 2024
 * @brief	Module pour utiliser une gache
 *******************************************************************************
 */

 #include "config.h"
 #if USE_GACHE
 #include "gache.h"
#include "MCP23S17/stm32g4_mcp23s17.h"
 //#include "stm32g4_systick.h"
 //#include <stdbool.h>
 //#include <stdio.h>
 

 void GACHE_Init(void){

	 BSP_MCP23S17_init();
	 BSP_MCP23S17_setGPIODirection(GACHE_GPIO_PORT, GACHE_GPIO_PIN, MCP23S17_DIRECTION_OUTPUT);
	 	 //MATRIX_GACHE_HAL_CLOCK_ENABLE();  // Active l'horloge du port GPIOB

       //GPIO_InitTypeDef GPIO_InitStruct = {0};
       //GPIO_InitStruct.Pin = GACHE_GPIO_PIN;
       //GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;  // Sortie Push-Pull
       //GPIO_InitStruct.Pull = GPIO_NOPULL;
       //GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
       //HAL_GPIO_Init(GACHE_GPIO_PORT, &GPIO_InitStruct);
}

void GACHE_Activer(void) {
   //HAL_GPIO_WritePin(GACHE_GPIO_PORT, GACHE_GPIO_PIN, GPIO_PIN_SET);
	BSP_MCP23S17_writeGPIO(GACHE_GPIO_PORT, GACHE_GPIO_PIN, MCP23S17_PIN_STATE_HIGH);
}

void GACHE_Desactiver(void) {
	BSP_MCP23S17_writeGPIO(GACHE_GPIO_PORT, GACHE_GPIO_PIN, MCP23S17_PIN_STATE_LOW);
}

void GACHE_DEMO (void)
{
	//printf("Test Gache");

	GACHE_Activer();
	HAL_Delay(10000);
	GACHE_Desactiver();
	HAL_Delay(10000);

}

 
 
 #endif
 
