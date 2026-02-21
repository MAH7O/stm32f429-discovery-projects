/**
  ******************************************************************************
  * @file    	main.c
  * @author		Florian von Zabiensky
  * @version 	V1.0
  * @date		14.03.2019
  * @brief  	Template using the LCD
  ******************************************************************************
*/


#include <lcd/lcd.h>
#include "stm32f4xx.h"
#include <stdio.h>
#include <utils/utils.h>
#include <my_lcd/my_lcd.h>


int main(void)
{
    HAL_Init();
    lcd_init();

    /* LCD-Demo-Funktionen */
//    startLCDTimer();                                   // Countdown 10–1
//    my_lcd_draw_baargraph(100, 100, 100, 100, 500, RED, BLUE); // Balken 50 % gefüllt
    //    draw_cross(100, 100);                              // Kreuz bei (100,100)

    while (1) {
    	char buff[64];
    	for(int i=10; i>=1; i--){
    		if(i == 9)
    		lcd_fill_screen(WHITE);

    		sprintf(buff, "    Zahl = %d",i);
            lcd_draw_text_at_line(buff,4,BLACK,2, WHITE);
            my_lcd_draw_baargraph(65, 100, 100, 100, (i * 100), RED, BLUE);
            utils_delay_ms(1000);
    	}
    }
}
