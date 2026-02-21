/**
  ******************************************************************************
  * @file    	main.c
  * @author		Mahmoud Mahmoud / Judy Abou Rmeh
  * @version 	V1.0
  * @date		02.11.2025
  * @brief  	Introduction
  ******************************************************************************
*/

#include <lcd/lcd.h>
#include "stm32f4xx.h"

#include <stdio.h>

#include <utils/utils.h>
/**
 * @brief Initialisiert die GPIO-Pins für die LED-Ausgänge.
 *
 * Diese Funktion aktiviert die Taktung für Port G und konfiguriert
 * die Pins PG13 und PG14 als Push-Pull-Ausgänge ohne Pull-Widerstände.
 *
 * @note PG13 entspricht in der Regel der grünen LED und PG14 der roten LED
 *       auf dem STM32F4-Discovery-Board.
 */
void initGPIOS(){
	/* Aktivierung der Clock für Port G */

		__HAL_RCC_GPIOG_CLK_ENABLE();

	/* GPIO-Konfiguration */

	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Pin = GPIO_PIN_13 | GPIO_PIN_14; // beide LED-Pins
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP; // Push-Pull-Ausgang
	GPIO_InitStruct.Pull = GPIO_NOPULL; //keine Pull-widerstäde
	GPIO_InitStruct.Speed = GPIO_SPEED_MEDIUM; // mittlere Geschwindigkeit

	HAL_GPIO_Init(GPIOG, &GPIO_InitStruct); //G Init

}

int main(void)
{
	/* Initialisierung der Hardware Abstraction Layer */

	HAL_Init();

	/* GPIO-Initialisierung für LEDs */

	initGPIOS();

	/* Grüne LED aktivieren */

	HAL_GPIO_WritePin(GPIOG, GPIO_PIN_13, GPIO_PIN_SET); // grüne LED aktivieren

//	HAL_GPIO_WritePin(GPIOG, GPIO_PIN_13 | GPIO_PIN_14, GPIO_PIN_SET); // beide LEDs aktivieren

//	HAL_GPIO_WritePin(GPIOG, GPIO_PIN_14, GPIO_PIN_RESET); // Rote LED deaktivieren

	while(1) {
		HAL_GPIO_TogglePin(GPIOG, GPIO_PIN_13 | GPIO_PIN_14); // LEDs umschalten
		utils_delay_ms(500); // 500 ms warten
	}
}
