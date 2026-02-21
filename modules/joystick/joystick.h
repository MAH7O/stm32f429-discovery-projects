/**
  ******************************************************************************
  * @file    	joystick.h
  * @author		Mahmoud Mahmoud / Judy Abou Rmeh
  * @version 	V1.0
  * @date		02.11.2025
  * @brief  	Headerdatei zur Implementierung der Joystick-Steuerung.
  ******************************************************************************
*/

#ifndef JOYSTICK_JOYSTICK_H_
#define JOYSTICK_JOYSTICK_H_

#include "stm32f4xx.h"

/**
 * @file joystick.h
 * @brief Headerdatei zur Ansteuerung eines 5-Wege-Joysticks am STM32F4.
 *
 * Diese Datei enthält alle Definitionen und Funktionsprototypen
 * zur Initialisierung und Abfrage eines digitalen Joysticks,
 * der über GPIO-Port G mit dem Mikrocontroller verbunden ist.
 *
 * Der Joystick verfügt über fünf Richtungen:
 * - UP (oben)
 * - DOWN (unten)
 * - LEFT (links)
 * - RIGHT (rechts)
 * - PRESS (zentraler Druckknopf)
 *
 * @date 27.10.2025
 * @version 1.0
 */

/** @brief Joystick-Nach-unten-Taste – Port G, Pin 6 */

#define JS_DOWN GPIO_PIN_6

/** @brief Joystick-Nach-links-Taste – Port G, Pin 9 */

#define JS_LEFT GPIO_PIN_9

/** @brief Joystick-Nach-rechts-Taste – Port G, Pin 10 */

#define JS_RIGHT GPIO_PIN_10

/** @brief Joystick-Nach-oben-Taste – Port G, Pin 11 */

#define JS_UP GPIO_PIN_11

/** @brief Joystick-Zentraler Druckknopf – Port G, Pin 12 */

#define JS_PRESS GPIO_PIN_12

/** @brief Kombination aller Joystick-Pins */

#define JS_ALL (JS_UP | JS_DOWN | JS_LEFT | JS_RIGHT | JS_PRESS)


/**
 * @brief Initialisiert alle GPIO-Pins für den Joystick.
 *
 * Aktiviert die Taktung für Port G und konfiguriert die Pins JS_UP, JS_DOWN,
 * JS_LEFT, JS_RIGHT und JS_PRESS als Eingänge mit Pull-up-Widerständen.
 */
void initJoyStick(void);

/**
 * @brief Liest den Zustand der UP-Richtung des Joysticks.
 *
 * @retval GPIO_PIN_SET   Joystick ist nicht gedrückt
 * @retval GPIO_PIN_RESET Joystick wird nach oben gedrückt
 */
GPIO_PinState readUP(void);

/**
 * @brief Liest den Zustand der DOWN-Richtung des Joysticks.
 *
 * @retval GPIO_PIN_SET   Joystick ist nicht gedrückt
 * @retval GPIO_PIN_RESET Joystick wird nach unten gedrückt
 */
GPIO_PinState readDOWN(void);

/**
 * @brief Liest den Zustand der RIGHT-Richtung des Joysticks.
 *
 * @retval GPIO_PIN_SET   Joystick ist nicht gedrückt
 * @retval GPIO_PIN_RESET Joystick wird nach rechts gedrückt
 */
GPIO_PinState readRIGHT(void);

/**
 * @brief Liest den Zustand der LEFT-Richtung des Joysticks.
 *
 * @retval GPIO_PIN_SET   Joystick ist nicht gedrückt
 * @retval GPIO_PIN_RESET Joystick wird nach links gedrückt
 */
GPIO_PinState readLEFT(void);

/**
 * @brief Liest den Zustand des zentralen Druckknopfs (PRESS).
 *
 * @retval GPIO_PIN_SET   Joystick ist nicht gedrückt
 * @retval GPIO_PIN_RESET Joystick wird gedrückt
 */
GPIO_PinState readPRESS(void);

#endif /* JOYSTICK_JOYSTICK_H_ */
