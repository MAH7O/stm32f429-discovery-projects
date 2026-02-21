/**
  ******************************************************************************
  * @file    	main.c
  * @author		Mahmoud Mahmoud / Judy Abou Rmeh
  * @version 	V1.0
  * @date		02.11.2025
  * @brief  	Hauptprogramm zur Ansteuerung der ESD
  ******************************************************************************
*/


#include <lcd/lcd.h>
#include <esd/esd.h>
#include <utils/utils.h>
#include "stm32f4xx.h"


	/**
	 * @brief Startet einen einfachen Countdown auf jeder Position einzeln.
	 *
	 * Zeigt nacheinander auf jeder der vier Positionen die Zahlen 9 bis 0.
	 * Zwischen jeder Anzeige wird eine Sekunde gewartet.
	 *
	 * @note Diese Funktion läuft sequenziell über die vier Positionen.
	 */
	void startNormalCountdown(void){ //9 To 0 in je POs
		for(int i = ESD_POSITION_1; i < ESD_POSITION_ALL; i++){
			for(int j = ESD_DIGIT_9; j >= ESD_DIGIT_0; j--){
				esd_show_digit(j, i);
				utils_delay_ms(1000); //1 sec Delay
			}
			turnAllPositionsOff();
		}
	}

	/**
	 * @brief Teilt eine 4-stellige Zahl in ihre Einzelziffern auf.
	 *
	 * @param value  Zahl zwischen 0 und 9999
	 * @param out    Array der Länge 4, in das die Ziffern geschrieben werden
	 *
	 * Beispiel:
	 * `split_number_4(1234, digits)` ergibt `digits = {1, 2, 3, 4}`
	 */
	void split_number_4(int value, esd_digit_t out[4]) { //ZB 1234
	    if (value < 0) value = 0;
	    if (value > 9999) value = 9999;
	    out[0] = (esd_digit_t)((value / 1000) % 10);
	    out[1] = (esd_digit_t)((value / 100)  % 10);
	    out[2] = (esd_digit_t)((value / 10)   % 10);
	    out[3] = (esd_digit_t)( value         % 10);
	}

	/**
	 * @brief Zeigt eine vierstellige Zahl für eine bestimmte Dauer auf dem Display an.
	 *
	 * @param value     Zahl (0–9999), die angezeigt werden soll
	 * @param duration  Gesamtdauer in Millisekunden, wie lange die Zahl sichtbar sein soll
	 *
	 * Diese Funktion nutzt Multiplexing, um alle vier Ziffern scheinbar gleichzeitig
	 * anzuzeigen. Jede Position wird kurz nacheinander aktiviert und wieder ausgeschaltet.
	 */

	void showNumbersInESD(int value, uint32_t duration) {
	    esd_digit_t esdDigits[4];
	    split_number_4(value, esdDigits);

	    uint32_t durationPerPos = 2;

	    uint32_t elapsed = 0;
	    while (elapsed < duration) {
	        esd_show_digit(esdDigits[0], ESD_POSITION_1);
	        utils_delay_ms(durationPerPos);
	        turnAllPositionsOff();

	        esd_show_digit(esdDigits[1], ESD_POSITION_2);
	        utils_delay_ms(durationPerPos);
	        turnAllPositionsOff();

	        esd_show_digit(esdDigits[2], ESD_POSITION_3);
	        utils_delay_ms(durationPerPos);
	        turnAllPositionsOff();

	        esd_show_digit(esdDigits[3], ESD_POSITION_4);
	        utils_delay_ms(durationPerPos);
	        turnAllPositionsOff();

	        elapsed += (durationPerPos * 4);
	    }
	}

	/**
	 * @brief Startet einen Countdown von 9999 bis 0000.
	 *
	 * Jeder Wert wird für 1 Sekunde angezeigt, wobei das Display über Multiplexing
	 * betrieben wird. Diese Funktion demonstriert die gleichzeitige Anzeige
	 * mehrerer Ziffern im schnellen Wechsel.
	 */

	void startBigNumberCountdown(void){
		for (int v = 9999; v >= 0; --v) {
			showNumbersInESD(v, 1000);  // 1 Sekunde
		}

	}


int main(void)
{
	HAL_Init(); // HAL-System initialisieren

	esd_init(); // 7-Segment-Display vorbereiten

	/* Initialization of the LCD */
//	lcd_init();

//	HAL_GPIO_WritePin(GPIOD, CNTL1_PD, GPIO_PIN_SET);
//	HAL_GPIO_WritePin(GPIOD, A_PD, GPIO_PIN_SET);

//	esd_show_digit(ESD_DIGIT_0, ESD_POSITION_1);

//	HAL_GPIO_WritePin(GPIOD, CNTL4_PD, GPIO_PIN_SET);
//	HAL_GPIO_WritePin(GPIOE, B_PD, GPIO_PIN_SET);


//	esd_show_digit(ESD_DIGIT_2, ESD_POSITION_1);

	while(1) {
		startNormalCountdown(); // Countdown

//		startBigNumberCountdown(); // 9999 → 0000 Countdown

//		esd_show_digit(ESD_DIGIT_0, ESD_POSITION_1);
//		HAL_Delay(5);
//		turnAllPositionsOff();
//		esd_show_digit(ESD_DIGIT_1, ESD_POSITION_2);
//		HAL_Delay(5);
//		turnAllPositionsOff();
	}
}
