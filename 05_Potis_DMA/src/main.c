/**
**************************************************
* @file main.c
* @author Mahmoud Mahmoud & Judy Abou Rmeh
* @version v1.0
* @date 15.11.25
* @brief Main application: reads potentiometer values via ADC+DMA
*        and displays them on LCD as text and bargraphs.
@verbatim
==================================================
				### Resources used ###
	GPIO:    PA6, PA7 (analog inputs for potentiometers)
	ADC:     ADC1 (2 channels: CH6, CH7, DMA2 Stream0 Channel 0)
	DMA:     DMA2 Stream0 Channel 0
	LCD:     TFT LCD via lcd / my_lcd libraries
==================================================
					### Usage ###
	(#) Call 'main()' after startup. It will:
    	- Initialize the HAL
    	- Initialize the LCD
    	- Initialize the potentiometer DMA module (potis_dma_init)
    	- Start ADC+DMA data acquisition (potis_dma_start)
    	- Continuously read two potentiometers (via potis_dma_get_val)
      	  and display their values in mV and as bargraphs.
==================================================
@endverbatim
**************************************************
*/

/* Includes */
#include <lcd/lcd.h>
#include <my_lcd/my_lcd.h>
#include <potis_dma/potis_dma.h>
#include "stm32f4xx.h"
#include <stdio.h>   /* for sprintf */

/* Preprocessor defines */
/**
 * @brief Converts an ADC raw value to millivolts.
 * @param adc_value                  Measured ADC raw value.
 * @param board_millievoltage        Supply/reference voltage in millivolts.
 * @param adc_resolution_in_decimal  Maximum ADC raw value (e.g. 4095 for 12-bit).
 */
#define CONVERT_VALUE_TO_MILLIEVOLT(adc_value, board_millievoltage, adc_resolution_in_decimal) ((adc_value * board_millievoltage) / adc_resolution_in_decimal)

/**
 * @brief Converts an ADC raw value to a bargraph value.
 * @param adc_value                  Measured ADC raw value.
 * @param baargraph_max              Maximum value of the bargraph scale.
 * @param adc_resolution_in_decimal  Maximum ADC raw value (e.g. 4095 for 12-bit).
 */
#define CONVERT_VALUE_TO_BARAGRAPH_VALUE(adc_value, baargraph_max, adc_resolution_in_decimal) ((adc_value * baargraph_max) / adc_resolution_in_decimal)

/* Preprocessor macros */
/* Module intern type definitions */
/* Static module variables */
/* Static module functions (prototypes) */
/* Public functions */

/**
 * @brief  Main program entry point.
 *         Initializes HAL, LCD and ADC+DMA potentiometer module, then
 *         continuously reads and displays two potentiometers.
 * @param  None
 * @return int Program does not return in normal operation.
 */
int main(void)
{
    /* Initialize the HAL library (configures system clock, SysTick, etc.) */
    HAL_Init();

    /* Initialization of the LCD */
    lcd_init();

    /* Initialize and start ADC+DMA-based potentiometer acquisition */
    potis_dma_init();
    potis_dma_start();

    char buffer[64];

    while(1) {

        /* Display Poti1 value in millivolts */
        sprintf(buffer, "     Poti1: %-4lu",
                CONVERT_VALUE_TO_MILLIEVOLT(potis_dma_get_val(POTI_1),
                                            TEST_BOARD_MILLIEVOLTAGE,
                                            ADC_12_BIT_RESOLUTION));
        lcd_draw_text_at_line(buffer, 6, BLACK, 2, WHITE);

        /* Draw bargraph for Poti1 */
        my_lcd_draw_baargraph(50, 50, 150, 50,
                              CONVERT_VALUE_TO_BARAGRAPH_VALUE(
                                  potis_dma_get_val(POTI_1),
                                  MAX_BAARGRAPH_VALUE,
                                  ADC_12_BIT_RESOLUTION),
                              GREEN, DARKGREY);

        /* Display Poti2 value in millivolts */
        sprintf(buffer, "     Poti2: %-4lu",
                CONVERT_VALUE_TO_MILLIEVOLT(potis_dma_get_val(POTI_2),
                                            TEST_BOARD_MILLIEVOLTAGE,
                                            ADC_12_BIT_RESOLUTION));
        lcd_draw_text_at_line(buffer, 12, BLACK, 2, WHITE);

        /* Draw bargraph for Poti2 */
        my_lcd_draw_baargraph(50, 150, 150, 50,
                              CONVERT_VALUE_TO_BARAGRAPH_VALUE(
                                  potis_dma_get_val(POTI_2),
                                  MAX_BAARGRAPH_VALUE,
                                  ADC_12_BIT_RESOLUTION),
                              RED, DARKGREY);
    }
}
