/**
**************************************************
* @file main.c
* @author Mahmoud Mahmoud / Judy Abou Rmeh
* @version v1.0
* @date 30.11.25
* @brief Stopwatch demo: shows current time and lap times on the LCD.
@verbatim
==================================================
				### Resources used ###
	TIMER:   TIM1 (used inside stopwatch module)
	GPIO:    PA0 as EXTI0 (start/lap button, inside stopwatch module)
	LCD:     TFT display (via lcd library)
==================================================
					### Usage ###
	(#) Call 'main()' on startup. It will:
    	- Initialize HAL
    	- Initialize LCD
    	- Initialize stopwatch timer, GPIO and interrupts
    	- In the main loop:
        	* Continuously display the current stopwatch time
        	* Whenever a new lap is added (button press),
          	  display the lap time on the LCD including lap index.
==================================================
@endverbatim
**************************************************
*/

/* Includes */
#include <lcd/lcd.h>
#include "stm32f4xx.h"
#include <stopwatch/stopwatch.h>
#include <stdio.h>

/**
 * @brief  Main application entry point.
 * @return int Program does not return in normal operation.
 */
int main(void)
{
    /* Initialize HAL library */
    HAL_Init();

    /* Initialization of the LCD */
    lcd_init();

    /* Initialize stopwatch (timer, GPIO, interrupts) */
    stopwatch_init_timer();
    stopwatch_init_gpio();
    stopwatch_init_interrupt();

    char ch_buffer[64];

    while (1) {
        /* Show current stopwatch time (mm:ss.cs) */
        sprintf(ch_buffer,
                "time: %02u:%02u.%02u",
                (unsigned)stopwatch_get_current_minutes(),
                (unsigned)stopwatch_get_current_seconds(),
                (unsigned)(stopwatch_get_current_milliseconds() % 100U));
        lcd_draw_text_at_line(ch_buffer, 1, BLACK, 2, WHITE);

        /* Check if a new lap has been added (flag set in ISR) */
        if (bool_stopwatch_lap_added_flag) {
            bool_stopwatch_lap_added_flag = false;

            /* Get pointers to lap time buffers */
            uint16_t* u16_laps_in_milliseconds = (uint16_t*)stopwatch_get_lap_milliseconds();
            uint16_t* u16_laps_in_seconds      = (uint16_t*)stopwatch_get_lap_seconds();
            uint16_t* u16_laps_in_minutes      = (uint16_t*)stopwatch_get_lap_minutes();

            /* Index of the last added lap (set in ISR) */
            uint8_t i = u8_stopwatch_lap_added_index;

            /* Display lap N: mm:ss.cs on line (i+3) */
            sprintf(ch_buffer,
                    "lap %u: %02u:%02u.%02u",
                    (unsigned)stopwatch_get_lap_counts(),
                    (unsigned)u16_laps_in_minutes[i],
                    (unsigned)u16_laps_in_seconds[i],
                    (unsigned)(u16_laps_in_milliseconds[i] % 100U));

            lcd_draw_text_at_line(ch_buffer, (uint8_t)(i + 3U), BLACK, 2, WHITE);
        }
    }
}
