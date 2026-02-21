/**
**************************************************
* @file main.c
* @author Mahmoud Mahmoud / Judy Abou Rmeh
* @version v1.0
* @date 30.11.25
* @brief Main application: adjusts dot-LED brightness using TIM1 PWM based on
*        potentiometer input and displays debug information on the LCD.
@verbatim
==================================================
				### Resources used ###
	GPIO:    as configured by DOT and POTIS_DMA modules
	TIMER:   TIM1 (PWM mode, used for brightness control)
	ADC:     ADC1 with DMA (via potis_dma)
	LCD:     TFT display (via lcd library)
==================================================
					### Usage ###
	(#) Call 'main()' on startup. It will:
    	- Initialize HAL
    	- Initialize GPIO + PWM output for the dot-LED
    	- Initialize ADC1 + DMA sampling of two potentiometers
    	- Initialize the LCD
    	- In the infinite loop:
        	* Read potentiometer 1 value via DMA
        	* Convert it into a PWM duty-cycle value
        	* Update LED brightness via 'dot_change_brightness()'
        	* Display timer counter value and ADC reading on LCD
==================================================
@endverbatim
**************************************************
*/

/* Includes */
#include <lcd/lcd.h>
#include "stm32f4xx.h"
#include <stdio.h>
#include <dot/dot.h>
#include <potis_dma/potis_dma.h>

/* Preprocessor defines */
/**
 * @brief Scales an ADC value to a range suitable for timer brightness control.
 *
 * Example:
 *   brightness = (adc_value * DOT_MAX_BRIGHTNESS) / 4095;
 *
 * @param adc_value                 Raw ADC reading (0..4095).
 * @param timer_min_max             Maximum timer value (e.g. DOT_MAX_BRIGHTNESS).
 * @param adc_resolution_in_decimal Full-scale ADC resolution (e.g. 4095).
 */
#define CONVERT_VALUE_TO_TIMER_VALUE(adc_value, timer_min_max, adc_resolution_in_decimal) \
    ((adc_value * (timer_min_max)) / (adc_resolution_in_decimal))

/**
 * @brief Main application entry point.
 * @return int Never returns in normal operation.
 */
int main(void)
{
    /* Initialize HAL library */
    HAL_Init();

    /* Initialize dot-LED hardware */
    dot_esd_init();
    dot_timer_init(DOT_DIMMING_MODE);

    /* Initialize ADC+DMA potentiometer module */
    potis_dma_init();
    potis_dma_start();

    /* Initialization of the LCD */
    lcd_init();

    char ch_buffer[64];

    while (1)
    {
        /* Show timer counter value on LCD */
        uint16_t u16_time = __HAL_TIM_GET_COUNTER(&tim_handle_struct);
        sprintf(ch_buffer, "CNT: %5d", u16_time);
        lcd_draw_text_at_line(ch_buffer, 4, BLACK, 3, WHITE);

        /* Show potentiometer 1 value on LCD */
        uint16_t u16_pot = potis_dma_get_val(POTI_1);
        sprintf(ch_buffer, "POT: %5d", u16_pot);
        lcd_draw_text_at_line(ch_buffer, 6, BLACK, 3, WHITE);

        /* Convert potentiometer value to PWM brightness */
        uint32_t brightness =
                CONVERT_VALUE_TO_TIMER_VALUE(potis_dma_get_val(POTI_1),
                                             DOT_MAX_BRIGHTNESS,
                                             ADC_12_BIT_RESOLUTION);

        /* Apply brightness to dot-LED */
        dot_change_brightness(brightness);
    }
}
