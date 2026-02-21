/**
  ******************************************************************************
  * @file    	joystick.c
  * @author		Mahmoud Mahmoud / Judy Abou Rmeh
  * @version 	V1.0
  * @date		02.11.2025
  * @brief  	Implementierung der Joystick-Steuerung.
  ******************************************************************************
*/


#include "joystick.h"

/**
 * @brief Initialisiert alle GPIO-Pins für den Joystick als Eingänge.
 *
 * Diese Funktion aktiviert die Taktung für Port G und konfiguriert
 * die zugehörigen Pins (UP, DOWN, LEFT, RIGHT, PRESS) als Eingänge
 * mit Pull-up-Widerständen.
 *
 * @note Die Pull-ups sorgen dafür, dass der Eingang im Ruhezustand HIGH ist.
 *       Wird der Joystick gedrückt, zieht er den Eingang auf LOW.
 */
void initJoyStick(void){
	__HAL_RCC_GPIOG_CLK_ENABLE();

	GPIO_InitTypeDef GPIO_InitStructPG;

		GPIO_InitStructPG.Pin = JS_ALL;
		GPIO_InitStructPG.Mode = GPIO_MODE_INPUT;
		GPIO_InitStructPG.Pull = GPIO_PULLUP;
		GPIO_InitStructPG.Speed = GPIO_SPEED_MEDIUM;

		HAL_GPIO_Init(GPIOG, &GPIO_InitStructPG);
}

/**
 * @brief Liest den Zustand der UP-Richtung des Joysticks aus.
 *
 * @retval GPIO_PIN_SET   Joystick ist NICHT gedrückt
 * @retval GPIO_PIN_RESET Joystick wird nach oben gedrückt
 */
GPIO_PinState readUP(void){
	return HAL_GPIO_ReadPin(GPIOG, JS_UP);
}

/**
 * @brief Liest den Zustand der DOWN-Richtung des Joysticks aus.
 *
 * @retval GPIO_PIN_SET   Joystick ist NICHT gedrückt
 * @retval GPIO_PIN_RESET Joystick wird nach unten gedrückt
 */
GPIO_PinState readDOWN(void){
	return HAL_GPIO_ReadPin(GPIOG, JS_DOWN);
}
/**
 * @brief Liest den Zustand der RIGHT-Richtung des Joysticks aus.
 *
 * @retval GPIO_PIN_SET   Joystick ist NICHT gedrückt
 * @retval GPIO_PIN_RESET Joystick wird nach rechts gedrückt
 */
GPIO_PinState readRIGHT(void){
	return HAL_GPIO_ReadPin(GPIOG, JS_RIGHT);
}

/**
 * @brief Liest den Zustand der LEFT-Richtung des Joysticks aus.
 *
 * @retval GPIO_PIN_SET   Joystick ist NICHT gedrückt
 * @retval GPIO_PIN_RESET Joystick wird nach links gedrückt
 */

GPIO_PinState readLEFT(void){
	return HAL_GPIO_ReadPin(GPIOG, JS_LEFT);
}

/**
 * @brief Liest den Zustand des Joystick-Druckknopfs (CENTER / PRESS).
 *
 * @retval GPIO_PIN_SET   Joystick ist NICHT gedrückt
 * @retval GPIO_PIN_RESET Joystick wird gedrückt
 */
GPIO_PinState readPRESS(void){
	return HAL_GPIO_ReadPin(GPIOG, JS_PRESS);
}

