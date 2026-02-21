/**
  ******************************************************************************
  * @file    	my_lcd.c
  * @author		Mahmoud Mahmoud / Judy Abou Rmeh
  * @version 	V1.0
  * @date		02.11.2025
  * @brief  	Benutzerdefinierte LCD-Funktionen zur Anzeige von Text, Balkendiagrammen und grafischen Elementen.
  ******************************************************************************
*/

#include "my_lcd.h"

/**
 * @brief Startet einen Countdown auf dem LCD.
 *
 * Diese Funktion zeigt eine einfache Countdown-Sequenz von 10 bis 1
 * auf dem LCD an. Zu Beginn wird der Bildschirm gelöscht, anschließend
 * wird pro Sekunde die aktuelle Zahl angezeigt.
 *
 * @note Die Funktion nutzt `lcd_fill_screen()`, `lcd_draw_text_at_line()` und `utils_delay_ms()`.
 */
void startLCDTimer(){
	char buff[64];
	for(int i=10; i>=1; i--){
		if(i == 9)
		lcd_fill_screen(WHITE);

		sprintf(buff, "    Zahl = %d",i);
        lcd_draw_text_at_line(buff,4,BLACK,2, WHITE);
        utils_delay_ms(1000);
	}
}

/**
 * @brief Zeichnet einen horizontalen Balken (Bargraph) auf dem LCD.
 *
 * @param x         X-Startkoordinate des Balkens
 * @param y         Y-Startkoordinate des Balkens
 * @param width     Gesamtlänge des Balkens in Pixel
 * @param height    Höhe des Balkens in Pixel
 * @param value     Aktueller Füllstandswert (0–1000)
 * @param color     Farbe des gefüllten Bereichs
 * @param bgcolor   Hintergrundfarbe (leerer Bereich)
 *
 * Diese Funktion berechnet den gefüllten Anteil proportional zu `value`
 * und zeichnet zwei Rechtecke: einen gefüllten Bereich und den Rest als Hintergrund.
 *
 * Beispiel:
 * `my_lcd_draw_baargraph(10, 20, 200, 20, 500, GREEN, WHITE);`
 * → Zeichnet einen halb gefüllten grünen Balken mit weißem Hintergrund.
 */
void my_lcd_draw_baargraph(int x, int y, int width, int height, int value, uint16_t color, uint16_t bgcolor){
    if (value < 0) value = 0;
    if (value > 1000) value = 1000;

    int filled = (width * value) / 1000;

	// Gefüllter Teil
    ILI9341_Draw_Filled_Rectangle_Coord(x, y, x + filled, y + height, color);
	// Hintergrund (nicht gefüllter Teil)

    ILI9341_Draw_Filled_Rectangle_Coord(x + filled, y, x + width, y + height, bgcolor);
}

/**
 * @brief Zeichnet ein diagonales Kreuz aus zwei Linien.
 *
 * @param x  X-Koordinate des Startpunkts
 * @param y  Y-Koordinate des Startpunkts
 *
 * Zeichnet ein 100×100 Pixel großes Kreuz bestehend aus zwei farbigen Diagonalen:
 * - Eine rote Linie von oben links nach unten rechts
 * - Eine blaue Linie von oben rechts nach unten links
 *
 * Beispiel:
 * `draw_cross(50, 50);` zeichnet ein Kreuz mit Startpunkt (50,50).
 */
void draw_cross(int x, int y) {
    for (int i = 0; i < 100; i++) {
        lcd_draw_pixel(x + i, y + i, RED);// Diagonale von oben links nach unten rechts
        lcd_draw_pixel(x + i, y + 99 - i, BLUE); // Diagonale von oben rechts nach unten links
	}
}
