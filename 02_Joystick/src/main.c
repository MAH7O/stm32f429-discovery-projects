/**
  ******************************************************************************
  * @file    	main.c
  * @author		Mahmoud Mahmoud / Judy Abou Rmeh
  * @version 	V1.0
  * @date		02.11.2025
  * @brief  	Joystick Function Examples
  ******************************************************************************
*/

#include <joystick/joystick.h>
#include <esd/esd.h>
#include <utils/utils.h>
#include "stm32f4xx.h"


/**
 * @brief Hauptprogramm: kombiniert Joystick- und 7-Segment-Steuerung.
 *
 * Initialisiert das HAL-System, den Joystick und das ESD-Modul.
 * In der Endlosschleife werden die Joystickeingaben ausgewertet und
 * die entsprechende Zahl bzw. Position auf dem 7-Segment-Display angezeigt.
 *
 * Steuerung:
 * - **UP:**    Ziffer um +1 erhöhen (9 → 0)
 * - **DOWN:**  Ziffer um -1 verringern (0 → 9)
 * - **RIGHT:** Position nach rechts wechseln (4 → 1)
 * - **LEFT:**  Position nach links wechseln (1 → 4)
 * - **PRESS:** Countdown von aktueller Zahl bis 0 an aktueller Position
 *
 * @return int Gibt keinen Wert zurück (Endlosschleife)
 */
int main(void)
{
	HAL_Init();

	initJoyStick();

	esd_init();

	esd_digit_t currentNumber = ESD_DIGIT_0; //startwert 0
	esd_position_t currentPosition = ESD_POSITION_1; // Startposition: 1

	while(1) {
		utils_delay_ms(150); // Kurze Entprell- und Reaktionsverzögerung
		// Nach oben drücken → Zahl erhöhen
		if(readUP() == GPIO_PIN_RESET){ //Taste gedrückt
			if(currentNumber == ESD_DIGIT_9){
				currentNumber = ESD_DIGIT_0;// Von 9 auf 0 springen
			}else{
				currentNumber = currentNumber + 1; //Zahl inkrementieren
			}
		}
		// Nach unten drücken → Zahl verringern
		if(readDOWN() == GPIO_PIN_RESET){
			if(currentNumber == ESD_DIGIT_0){
				currentNumber = ESD_DIGIT_9; // Von 0 auf 9 springen

			}else{
				currentNumber = currentNumber - 1;//Zahl verringern
			}
		}
		// Nach rechts drücken → Position nach rechts wechseln
		if(readRIGHT() == GPIO_PIN_RESET){
			if(currentPosition == ESD_POSITION_4){
				currentPosition = ESD_POSITION_1;
			}else{
				currentPosition = currentPosition + 1;//Pos rechts schieben
			}
		}
		// Nach links drücken → Position nach links wechseln

		if(readLEFT() == GPIO_PIN_RESET){
			if(currentPosition == ESD_POSITION_1){
				currentPosition = ESD_POSITION_4; // Von links wieder ganz rechts
			}else{
				currentPosition = currentPosition - 1;
			}
		}
		// Drücken (PRESS) → Countdown von aktueller Zahl
		if(readPRESS() == GPIO_PIN_RESET){
			for(int i = currentNumber; i >= 0; i--){
				esd_show_digit(i, currentPosition);// Countdown anzeigen
				utils_delay_ms(1000); // 1 Sekunde pro Zahl
			}
		}
		// Display aktualisieren
		turnAllPositionsOff();  // Vorher alle Positionen löschen
		esd_show_digit(currentNumber, currentPosition); // Neue Zahl anzeigen
	}
}
