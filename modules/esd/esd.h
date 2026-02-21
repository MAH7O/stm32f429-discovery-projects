/**
  ******************************************************************************
  * @file    	esd.h
  * @author		Mahmoud Mahmoud / Judy Abou Rmeh
  * @version 	V1.0
  * @date		02.11.2025
  * @brief  	Headerdatei für ESD Controller
  ******************************************************************************
*/

#ifndef ESD_ESD_H_
#define ESD_ESD_H_

#include "stm32f4xx.h"
/**
 * @file esd.h
 * @brief Headerdatei zur Ansteuerung eines 4-stelligen 7-Segment-Displays.
 *
 * Diese Datei enthält alle Definitionen, Datentypen und Funktionsprototypen,
 * die für die Steuerung des Electronic Segment Display (ESD) mit einem STM32F4
 * Mikrocontroller benötigt werden.
 *
 * Das Display wird über die Ports D und E betrieben und unterstützt
 * die Anzeige einzelner Ziffern (0–9) sowie das Aktivieren einzelner
 * oder aller Positionen (1–4).
 */

/** @brief Steuerleitungen (Positionsauswahl) – Port D */
#define CNTL1_PD GPIO_PIN_14 	// PD14
#define CNTL2_PD GPIO_PIN_15 	// PD15
#define CNTL3_PD GPIO_PIN_0 	// PD0
#define CNTL4_PD GPIO_PIN_1 	// PD1
#define POINT_PE GPIO_PIN_7 	// PE7

/** @brief Punktanzeige – Port E */

#define A_PD GPIO_PIN_7 		// PD7
#define B_PD GPIO_PIN_4			// PD8
#define C_PD GPIO_PIN_5 		// PD5
#define D_PD GPIO_PIN_6 		// PD6
#define E_PD GPIO_PIN_12 		// PD12
#define F_PD GPIO_PIN_11 		// PD11
#define G_PE GPIO_PIN_12 		// PE12
#define DOT_PE GPIO_PIN_11 		// PE11


/**
 * @enum esd_digit_t
 * @brief Repräsentiert die anzuzeigenden Ziffern (0–9).
 */
typedef enum {
	ESD_DIGIT_0, //Ziffer 0
	ESD_DIGIT_1, //Ziffer 1
	ESD_DIGIT_2, //Ziffer 2
	ESD_DIGIT_3, //Ziffer 3
	ESD_DIGIT_4, //Ziffer 4
	ESD_DIGIT_5, //Ziffer 5
	ESD_DIGIT_6, //Ziffer 6
	ESD_DIGIT_7, //Ziffer 7
	ESD_DIGIT_8, //Ziffer 8
	ESD_DIGIT_9 //Ziffer 9
} esd_digit_t;

/**
 * @enum esd_position_t
 * @brief Gibt an, welche Stelle des Displays aktiviert werden soll.
 */

typedef enum {
	ESD_POSITION_1, //position 1 links
	ESD_POSITION_2, //position 2
	ESD_POSITION_3, //position 3
	ESD_POSITION_4, //position 4 rechts
	ESD_POSITION_ALL //alle positionen

} esd_position_t;

/**
 * @brief Initialisiert alle benötigten GPIO-Pins für das Display.
 *
 * Aktiviert die Taktung für Port D und Port E und konfiguriert
 * die zugehörigen Pins als digitale Ausgänge.
 */
void esd_init(void);

/**
 * @brief Zeigt eine Ziffer an einer bestimmten Displayposition an.
 *
 * @param digit  Die darzustellende Ziffer (0–9)
 * @param pos    Die gewünschte Position (1–4 oder alle)
 *
 * Diese Funktion steuert die Segmente entsprechend dem
 * Bitmuster der gewählten Ziffer an und aktiviert die
 * zugehörige Position (CNTL1–CNTL4).
 */
void esd_show_digit(esd_digit_t digit, esd_position_t pos);

/**
 * @brief Schaltet alle Positionen (CNTL1–CNTL4) aus.
 *
 * Wird insbesondere im Multiplexbetrieb verwendet, um
 * Flackern während des Umschaltens zwischen den Positionen zu vermeiden.
 */
void turnAllPositionsOff(void);

/**
 * @brief Schreibt ein 16-Bit-Bitmuster auf alle Display-GPIOs.
 *
 * Steuert Segmente (A–G, Punkt/DOT) und Steuerleitungen (CNTL1–CNTL4)
 * entsprechend dem übergebenen Bitmuster.
 *
 * @param bincode  16-Bit-Wert für das Anzeigemuster.
 */
void utils_gpio_port_write(uint16_t bincode);


#endif /* ESD_ESD_H_ */
