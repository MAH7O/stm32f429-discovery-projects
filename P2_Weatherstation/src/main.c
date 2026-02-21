/**
 ******************************************************************************
 * @file        main.c
 * @author      Mahmoud Mahmoud, Judy Abou Rmeh
 * @version     V1.0
 * @date        14.03.2019
 * @brief       Hauptprogramm zur Anzeige von Umweltdaten auf dem LCD
 *
 * @details
 * Dieses Programm initialisiert das LCD sowie den Umweltsensor und liest
 * zyklisch Temperatur, Luftdruck und Luftfeuchtigkeit aus.
 * Die Messwerte werden formatiert und auf dem LCD dargestellt.
 *
 * Verwendete Module:
 *  - lcd
 *  - env_sensor
 *
 * Verwendete Peripherie:
 *  - LCD
 *  - Umweltsensor (z. B. Temperatur-, Druck- und Feuchtigkeitssensor)
 *
 ******************************************************************************
 */

#include <lcd/lcd.h>
#include "stm32f4xx.h"
#include <env_sensor/env_sensor.h>

/**
 * @brief   Einstiegspunkt des Programms
 *
 * @details
 * Initialisiert das HAL-System, das LCD und den Umweltsensor.
 * In der Endlosschleife werden Temperatur, Luftdruck und Luftfeuchtigkeit
 * ausgelesen, formatiert und auf dem LCD ausgegeben.
 *
 * @param   None
 * @return  None
 */
int main(void)
{
    HAL_Init();
    lcd_init();
    env_sensor_init();

    float temp, press, hum;
    char buffer[32];

    while (1)
    {
        env_sensor_read_data(&temp, &press, &hum);

        sprintf(buffer, "Temp: %.2f C", temp);
        lcd_draw_text_at_line(buffer, 2, BLACK, 2, WHITE);

        sprintf(buffer, "Pres: %.2f hPa", press);
        lcd_draw_text_at_line(buffer, 3, BLACK, 2, WHITE);

        sprintf(buffer, "Hum: %.2f %%", hum);
        lcd_draw_text_at_line(buffer, 4, BLACK, 2, WHITE);
    }
}
