/**
  ******************************************************************************
  * @file    	my_lcd.h
  * @author		Mahmoud Mahmoud / Judy Abou Rmeh
  * @version 	V1.0
  * @date		02.11.2025
  * @brief  	Headerdatei für benutzerdefinierte LCD-Funktionen.
  ******************************************************************************
*/

#ifndef MY_LCD_MY_LCD_H_
#define MY_LCD_MY_LCD_H_

#include "stm32f4xx.h"
#include <lcd/lcd.h>

#define MAX_BAARGRAPH_VALUE 1000

/**
 * @brief Startet einen Countdown (10 → 1) auf dem LCD.
 *
 * Zeigt nacheinander die Zahlen von 10 bis 1 auf dem Bildschirm an.
 * Der Bildschirm wird beim Start gelöscht und nach jeder Sekunde aktualisiert.
 *
 * @note Verwendet `lcd_fill_screen()`, `lcd_draw_text_at_line()` und `utils_delay_ms()`.
 */
void startLCDTimer();

/**
 * @brief Zeichnet einen horizontalen Balken (Bargraph) auf dem LCD.
 *
 * @param x         X-Startkoordinate des Balkens (linke obere Ecke)
 * @param y         Y-Startkoordinate des Balkens
 * @param width     Breite des Balkens in Pixel
 * @param height    Höhe des Balkens in Pixel
 * @param value     Füllstandswert (0–1000 entspricht 0–100 %)
 * @param color     Farbe des gefüllten Bereichs
 * @param bgcolor   Hintergrundfarbe (nicht gefüllter Bereich)
 *
 * Beispiel:
 * `my_lcd_draw_baargraph(10, 20, 200, 20, 750, GREEN, WHITE);`
 * → Zeichnet einen Balken, der zu 75 % gefüllt ist.
 */
void my_lcd_draw_baargraph(int x, int y, int width, int height, int value, uint16_t color, uint16_t bgcolor);

/**
 * @brief Zeichnet ein diagonales Kreuz auf dem LCD.
 *
 * @param x  X-Startkoordinate des Kreuzes
 * @param y  Y-Startkoordinate des Kreuzes
 *
 * Zeichnet ein 100 × 100 Pixel großes Kreuz bestehend aus:
 * - einer roten Diagonale (oben links → unten rechts)
 * - einer blauen Diagonale (oben rechts → unten links)
 */
void draw_cross(int x, int y);

#endif /* MY_LCD_MY_LCD_H_ */
