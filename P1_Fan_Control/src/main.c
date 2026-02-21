/**
 ******************************************************************************
 * @file        main.c
 * @author      Mahmoud Mahmoud / Judy Abou Rmeh
 * @date        21.12.2025
 * @version     V1.0
 * @brief       Main application controlling fan speed via potentiometer
 *              and LCD output.
 *
 * @details
 * This application initializes the HAL, LCD, fan control and
 * potentiometer DMA module. The potentiometer value is read
 * continuously and mapped to a target fan RPM. The current
 * and target RPM are displayed on the LCD.
 *
 * @resources
 *  - ADC (DMA based potentiometer input)
 *  - Timer (fan RPM measurement)
 *  - GPIO (LCD, fan)
 ******************************************************************************
 */

/* Includes ---------------------------------------------------------------- */
#include "stm32f4xx.h"
#include <stdio.h>

#include "lcd/lcd.h"
#include "fan/fan.h"
#include "potis_dma/potis_dma.h"

/* Preprocessor Defines ---------------------------------------------------- */
/**
 * @brief Converts an ADC value to a fan RPM value.
 *
 * @param adc_value Raw ADC value
 * @param fan_max   Maximum fan RPM
 * @param adc_max   ADC resolution (e.g. 4095 for 12 bit)
 */
#define MAIN_CONVERT_ADC_TO_RPM(adc_value, fan_max, adc_max) \
    (((adc_value) * (fan_max)) / (adc_max))

/* Static Module Variables ------------------------------------------------- */
/**
 * @brief Character buffer for LCD output.
 */
static char g_ch_lcd_buffer[64];

/* Public Functions -------------------------------------------------------- */
/**
 * @brief Main program entry point.
 *
 * @details
 * Initializes all peripherals and runs the main control loop.
 * The potentiometer value is read via DMA, converted to a
 * target RPM and passed to the fan PI controller. Target and
 * current RPM values are displayed on the LCD.
 *
 * @return int Program should never return.
 */
int main(void)
{
    uint32_t u32_poti_value;

    /* Initialize HAL */
    HAL_Init();

    /* Initialize modules */
    lcd_init();
    fan_control_init();
    potis_dma_init();
    potis_dma_start();

    /* Main application loop */
    while (1)
    {
        /* Read potentiometer value */
        u32_poti_value = potis_dma_get_val(POTI_1);

        /* Set fan target RPM */
        fan_change_target_rpm(
            MAIN_CONVERT_ADC_TO_RPM(
                u32_poti_value,
                FAN_MAX_RPM,
                ADC_12_BIT_RESOLUTION
            )
        );

        /* Update PI controller */
        fan_update_pi_controller();

        /* Display target RPM */
        sprintf(g_ch_lcd_buffer,
                "TAR: %-4lu",
                fan_get_target_rpm());
        lcd_draw_text_at_line(g_ch_lcd_buffer, 4, BLACK, 3, WHITE);

        /* Display current RPM */
        sprintf(g_ch_lcd_buffer,
                "CUR: %-4lu",
                fan_get_filtered_rpm());
        lcd_draw_text_at_line(g_ch_lcd_buffer, 6, BLACK, 3, WHITE);
    }
}
