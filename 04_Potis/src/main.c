/**
**************************************************
* @file main.c
* @author Mahmoud Mahmoud & Judy Abou Rmeh
* @version v1.0
* @date 15.11.25
* @brief Main application: reads potentiometer values and displays them on LCD
*        as text and bargraphs.
@verbatim
==================================================
				### Resources used ###
	GPIO: PA6, PA7 (analog inputs for potentiometers)
	ADC: ADC1 (2 channels: CH6, CH7)
	LCD: TFT LCD via lcd library
==================================================
					### Usage ###
	(#) Call 'main()' after startup. It will:
    	- Initialize the HAL
    	- Initialize the potentiometer module (ADC & GPIO)
    	- Initialize the LCD
    	- Continuously read two potentiometers and
      	  display their values in mV and as bargraphs.
==================================================
@endverbatim
**************************************************
*/

/* Includes */

#include <lcd/lcd.h>
#include "stm32f4xx.h"
#include <stdio.h>
#include <my_lcd/my_lcd.h>
#include <potis/potis.h>

/* Preprocessor defines */
/**
 * @brief Converts an ADC raw value to millivolts.
 * @param adc_value               Measured ADC raw value.
 * @param board_millievoltage     Supply/reference voltage in millivolts.
 * @param adc_resolution_in_decimal Maximum ADC raw value (e.g. 4095 for 12-bit).
 */
#define CONVERT_VALUE_TO_MILLIEVOLT(adc_value, board_millievoltage, adc_resolution_in_decimal) ((adc_value * board_millievoltage) / adc_resolution_in_decimal)

/**
 * @brief Converts an ADC raw value to a bargraph value.
 * @param adc_value               Measured ADC raw value.
 * @param baargraph_max           Maximum value of the bargraph scale.
 * @param adc_resolution_in_decimal Maximum ADC raw value (e.g. 4095 for 12-bit).
 */
#define CONVERT_VALUE_TO_BARAGRAPH_VALUE(adc_value, baargraph_max, adc_resolution_in_decimal) ((adc_value * baargraph_max) / adc_resolution_in_decimal)

/* Preprocessor macros */

/* Module intern type definitions */

/* Static module variables */

/* Static module functions (prototypes) */

/* Public functions */

/**
 * @brief  Main program entry point.
 *         Initializes HAL, potentiometers and LCD, then continuously
 *         reads two potentiometers and displays their values.
 * @param  None
 * @return int Program never returns in normal operation.
 */
int main(void)
{
    /* Initialize the HAL library (configures the system clock, SysTick, etc.) */
    HAL_Init();

    /* Initialize potentiometers (GPIO + ADC configuration) */
    potis_init();

    /* Initialization of the LCD */
    lcd_init();

    char buffer[64];

    while(1) {

        /* Read ADC values from the two potentiometers */
        uint32_t u32_poti_1_value = potis_get_val(POTI_1);
        uint32_t u32_poti_2_value = potis_get_val(POTI_2);

        /* Display Poti1 value in millivolts */
        sprintf(buffer,
                "     Poti1: %-4lu",
                CONVERT_VALUE_TO_MILLIEVOLT(u32_poti_1_value,
                                            TEST_BOARD_MILLIEVOLTAGE,
                                            ADC_12_BIT_RESOLUTION));
        lcd_draw_text_at_line(buffer, 6, BLACK, 2, WHITE);

        /* Draw bargraph for Poti1 */
        my_lcd_draw_baargraph(50, 50, 150, 50,
                              CONVERT_VALUE_TO_BARAGRAPH_VALUE(u32_poti_1_value,
                                                               MAX_BAARGRAPH_VALUE,
                                                               ADC_12_BIT_RESOLUTION),
                              GREEN, DARKGREY);

        /* Display Poti2 value in millivolts */
        sprintf(buffer,
                "     Poti2: %-4lu",
                CONVERT_VALUE_TO_MILLIEVOLT(u32_poti_2_value,
                                            TEST_BOARD_MILLIEVOLTAGE,
                                            ADC_12_BIT_RESOLUTION));
        lcd_draw_text_at_line(buffer, 12, BLACK, 2, WHITE);

        /* Draw bargraph for Poti2 */
        my_lcd_draw_baargraph(50, 150, 150, 50,
                              CONVERT_VALUE_TO_BARAGRAPH_VALUE(u32_poti_2_value,
                                                               MAX_BAARGRAPH_VALUE,
                                                               ADC_12_BIT_RESOLUTION),
                              RED, DARKGREY);
    }
}
