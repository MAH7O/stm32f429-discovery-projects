/**
  ******************************************************************************
  * @file    	esd.c
  * @author		Mahmoud Mahmoud / Judy Abou Rmeh
  * @version 	V1.0
  * @date		02.11.2025
  * @brief  	ESD Controller
  ******************************************************************************
*/

#include "stm32f4xx.h"
#include "esd.h"

/**
 * @brief Initialisiert alle GPIO-Pins für das 7-Segment-Display.
 *
 * Diese Funktion aktiviert die Clock für Port D und Port E und konfiguriert
 * die entsprechenden Pins als digitale Push-Pull-Ausgänge.
 *
 * Port D enthält:
 * - Segmente A–F
 * - Steuerleitungen CNTL1–CNTL4
 *
 * Port E enthält:
 * - Segment G
 * - Punkt (POINT)
 * - Komma (DOT)
 *
 * Die Pins werden ohne Pull-Widerstände und mit mittlerer Geschwindigkeit betrieben.
 */
void esd_init(void){

	__HAL_RCC_GPIOD_CLK_ENABLE();
	__HAL_RCC_GPIOE_CLK_ENABLE();

	GPIO_InitTypeDef GPIO_InitStruct; // Port D & E aktivieren
 //Alle Segmente und Steuerleitungen sind als digitale Ausgänge bereit, um LEDs des Displays direkt anzusteuern.
	GPIO_InitStruct.Pin = CNTL1_PD | CNTL2_PD | CNTL3_PD | CNTL4_PD | A_PD | B_PD | C_PD | D_PD | E_PD | F_PD;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_MEDIUM;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = POINT_PE | G_PE | DOT_PE;

	HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
}

/**
 * @brief Zeigt eine einzelne Ziffer auf einer bestimmten Displayposition an.
 *
 * @param digit  Die darzustellende Ziffer (0–9)
 * @param pos    Die Position des Displays, an der die Ziffer erscheinen soll
 *                (ESD_POSITION_1 ... ESD_POSITION_4 oder ESD_POSITION_ALL)
 *
 * Diese Funktion steuert die Segmente A–G entsprechend einem Bitmuster an.
 * Je nach Position wird die entsprechende Steuerleitung aktiviert.
 * Punkt (POINT) und Komma (DOT) werden standardmäßig deaktiviert.
 */
void esd_show_digit(esd_digit_t digit, esd_position_t pos){

	uint8_t numIndex[10] = {
			0b1111110, 		// 0
			0b0110000, 		// 1
			0b1101101, 		// 2
			0b1111001, 		// 3
			0b0110011,		// 4
			0b1011011, 		// 5
			0b1011111, 		// 6
			0b1110000, 		// 7
			0b1111111, 		// 8
			0b1111011		// 9
		};

	uint8_t nI = numIndex[digit];//Bitmuster für geewählte Ziffer

	// Segmente A–G ansteuern (LOW aktiviert Segment)

	HAL_GPIO_WritePin(GPIOD, A_PD, (nI & (1<<6)) ? GPIO_PIN_RESET : GPIO_PIN_SET); // A
	HAL_GPIO_WritePin(GPIOD, B_PD, (nI & (1<<5)) ? GPIO_PIN_RESET : GPIO_PIN_SET); // B
	HAL_GPIO_WritePin(GPIOD, C_PD, (nI & (1<<4)) ? GPIO_PIN_RESET : GPIO_PIN_SET); // C
	HAL_GPIO_WritePin(GPIOD, D_PD, (nI & (1<<3)) ? GPIO_PIN_RESET : GPIO_PIN_SET); // D
	HAL_GPIO_WritePin(GPIOD, E_PD, (nI & (1<<2)) ? GPIO_PIN_RESET : GPIO_PIN_SET); // E
	HAL_GPIO_WritePin(GPIOD, F_PD, (nI & (1<<1)) ? GPIO_PIN_RESET : GPIO_PIN_SET); // F
	HAL_GPIO_WritePin(GPIOE, G_PE, (nI & (1<<0)) ? GPIO_PIN_RESET : GPIO_PIN_SET); // G

	// Punkt und Komma standardmäßig deaktivieren

	HAL_GPIO_WritePin(GPIOE, POINT_PE, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOE, DOT_PE, GPIO_PIN_SET);

	// Gewählte Displayposition aktivieren

	switch(pos){
		case ESD_POSITION_1:
			HAL_GPIO_WritePin(GPIOD, CNTL1_PD, GPIO_PIN_SET);
			break;
		case ESD_POSITION_2:
			HAL_GPIO_WritePin(GPIOD, CNTL2_PD, GPIO_PIN_SET);
			break;
		case ESD_POSITION_3:
			HAL_GPIO_WritePin(GPIOD, CNTL3_PD, GPIO_PIN_SET);
			break;
		case ESD_POSITION_4:
			HAL_GPIO_WritePin(GPIOD, CNTL4_PD, GPIO_PIN_SET);
			break;
		case ESD_POSITION_ALL: //Alle Positionen gleichzeitig leuchten
			HAL_GPIO_WritePin(GPIOD, CNTL1_PD | CNTL2_PD | CNTL3_PD | CNTL4_PD, GPIO_PIN_SET);
			break;
	}


}

/**
 * @brief Schaltet alle Positionen (CNTL1–CNTL4) gleichzeitig aus.
 *
 * Wird im Multiplexbetrieb verwendet, um Flackern beim Wechsel der Positionen zu vermeiden.
 */
void turn_all_positions_off(void){
		HAL_GPIO_WritePin(GPIOD, CNTL1_PD | CNTL2_PD | CNTL3_PD | CNTL4_PD, GPIO_PIN_RESET);
	}

