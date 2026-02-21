/**
**************************************************
* @file main.c
* @author Mahmoud Mahmoud / Judy Abou Rmeh
* @version v1.0
* @date 30.11.25
* @brief Main application: controls dot-LED blink speed via timer
*        based on potentiometer input and displays debug values on LCD.
@verbatim
==================================================
				### Resources used ###
	GPIO:    as configured in DOT and POTIS_DMA modules
	TIMER:   TIM1 (used for blinking / PWM)
	ADC:     ADC1 with DMA (via potis_dma module)
	LCD:     TFT LCD via lcd library
==================================================
					### Usage ###
	(#) Call 'main()' after system startup. It will:
    	- Initialize the HAL
    	- Initialize the dot-LED hardware (GPIO + Timer)
    	- Initialize ADC1 + DMA for potentiometer sampling
    	- Initialize the LCD
    	- In an infinite loop:
        	* Read potentiometer 1 via 'potis_dma_get_val(POTI_1)'
        	* Convert the ADC value to a timer prescaler value
        	* Update the timer blink speed with 'dot_change_blink_speed()'
        	* Display current timer counter, ADC value and prescaler on the LCD
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
 * @brief Converts an ADC value into a timer frequency/prescaler-related value.
 *
 * The resulting value is scaled between DOT_MIN_BLINKSPEED and DOT_MAX_BLINKSPEED,
 * depending on the ADC input value and resolution.
 *
 * @param adc_value                 Measured ADC value.
 * @param timer_min_max_value       Difference between max and min blink speed.
 * @param adc_resolution_in_decimal Maximum ADC value (e.g. 4095 for 12-bit).
 */
#define CONVERT_VALUE_TO_TIMER_VALUE(adc_value, timer_min_max_value, adc_resolution_in_decimal) \
    (((adc_value * (timer_min_max_value)) / (adc_resolution_in_decimal)) + DOT_MIN_BLINKSPEED)

/* Public functions */

/**
 * @brief  Main program entry point.
 *         Links potentiometer 1 to the blink speed of the dot-LED via TIM1.
 * @param  None
 * @return int Program does not return in normal operation.
 */
int main(void)
{
    /* Initialize the HAL library */
    HAL_Init();

    /* Initialize dot-LED hardware (GPIO + PWM timer) */
    dot_esd_init();
    dot_timer_init(DOT_BLINKING_MODE);

    /* Initialize ADC+DMA potentiometer module */
    potis_dma_init();
    potis_dma_start();

    /* Initialization of the LCD */
    lcd_init();

    char ch_buffer[64];

    while (1) {

        /* Calculate new timer value (frequency / prescaler-related) from potentiometer 1 */
        uint32_t u32_new_prescaler =
                CONVERT_VALUE_TO_TIMER_VALUE(potis_dma_get_val(POTI_1),
                                             (DOT_MAX_BLINKSPEED - DOT_MIN_BLINKSPEED),
                                             ADC_12_BIT_RESOLUTION);

        /* Apply new blink speed */
        dot_change_blink_speed(u32_new_prescaler);

        /* Read current timer counter value */
        uint16_t u16_time = __HAL_TIM_GET_COUNTER(&tim_handle_struct);
        sprintf(ch_buffer, "CNT: %5d", u16_time);
        lcd_draw_text_at_line(ch_buffer, 4, BLACK, 3, WHITE);

        /* Read current potentiometer raw value */
        uint16_t u16_pot = (uint16_t)potis_dma_get_val(POTI_1);
        sprintf(ch_buffer, "POT: %5d", u16_pot);
        lcd_draw_text_at_line(ch_buffer, 6, BLACK, 3, WHITE);

        /* Display calculated prescaler / frequency value */
        sprintf(ch_buffer, "PRES: %-6lu", (unsigned long)u32_new_prescaler);
        lcd_draw_text_at_line(ch_buffer, 8, BLACK, 3, WHITE);
    }
}
