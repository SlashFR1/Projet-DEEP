/**
 *******************************************************************************
 * @file	gache.h
 * @author	vchav
 * @date	Jun 10, 2024
 * @brief	Module pour utiliser une gache
 *******************************************************************************
 */

 //#ifndef BSP_MATRIXKEYBOARD_STM32G4_MATRIX_KEYBOARD_H_
 //#define BSP_MATRIXKEYBOARD_STM32G4_MATRIX_KEYBOARD_H_
 
 #include "config.h"
 #if USE_GACHE
 

 //#include <stdbool.h>
 
 // valeurs possibles :
     //			GPIO du STM32G4					|	GPIOs de l'expander MCP23S17
     // ports : 	GPIOA, GPIOB, 					|	MCP23S17_PORT_A, MCP23S17_PORT_B
     // pins : 	GPIO_PIN_x (x from 0 to 15) 	|	MCP23S17_PIN_x (x from 0 to 7)

 #define GACHE_GPIO_PORT 	MCP23S17_PORT_A
 #define GACHE_GPIO_PIN	MCP23S17_PIN_6

 #define MATRIX_GACHE_HAL_CLOCK_ENABLE()	__HAL_RCC_GPIOA_CLK_ENABLE()
 
void GACHE_Init(void);
void GACHE_Activer(void);
void GACHE_Desactiver(void);
void GACHE_DEMO(void);

 //void BSP_MATRIX_KEYBOARD_init(const char * new_keyboard_keys);
 //void BSP_MATRIX_KEYBOARD_demo_process_main(void);
 //void BSP_MATRIX_KEYBOARD_demo_process_1ms(void);
 //bool BSP_MATRIX_KEYBOARD_is_pressed(void);
 //char BSP_MATRIX_KEYBOARD_get_key(void);
 //void BSP_MATRIX_KEYBOARD_press_and_release_events(char * press_event, char * release_event, uint32_t * all_touchs_pressed);
 
 //#endif
 #endif /* BSP_MATRIXKEYBOARD_STM32G4_MATRIX_KEYBOARD_H_ */
 
