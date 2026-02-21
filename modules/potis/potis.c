/**
**************************************************
* @file potis.c
* @author Mahmoud Mahmoud & Judy Abou Rmeh
* @version v1.0
* @date 15.11.25
* @brief ADC-based potentiometer reading module for STM32F4.
@verbatim
==================================================
				### Resources used ###
	GPIO:  PA6 (ADC1_), PA7 (TB_ADC2)
	ADC:   ADC1 (2 channels: CH6, CH7)
==================================================
					### Usage ###
	(#) Call 'potis_init()' once during system initialization to:
    	- Enable GPIOA and ADC1 clocks
    	- Configure PA6 and PA7 as analog inputs
    	- Set up ADC1 with two channels (6 and 7)

	(#) Use 'potis_get_val(POTI_1)' or 'potis_get_val(POTI_2)'
    	to read the raw ADC value of the respective potentiometer.
==================================================
@endverbatim
**************************************************
*/

/* Includes */

#include "potis.h"

/* Preprocessor defines */

/* Preprocessor macros */

/* Module intern type definitions */

/* Static module variables */

/**
 * @brief Global ADC handle structure used for ADC1 configuration and operation.
 */
ADC_HandleTypeDef ADC_handle_structure;

/* Static module functions (prototypes) */

/**
 * @brief  Initializes GPIO pins for the ADC channels used by the potentiometers.
 *         Configures PA6 and PA7 as analog inputs.
 * @param  None
 * @return None
 */
void potis_init_gpio(void);

/* Public functions */

/**
 * @brief  Initializes potentiometer module hardware.
 *         - Enables GPIOA and ADC1 clocks
 *         - Configures PA6/PA7 as analog inputs
 *         - Configures ADC1 for 2-channel conversion (CH6 and CH7)
 * @param  None
 * @return None
 */
void potis_init(void)
{
    /* Initialize GPIOs for ADC channels */
	potis_init_gpio();

    /* Enable ADC1 clock */
    __HAL_RCC_ADC1_CLK_ENABLE();

    /* Configure ADC instance */
    ADC_handle_structure.Instance = ADC1;
    ADC_handle_structure.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
    ADC_handle_structure.Init.Resolution = ADC_RESOLUTION_12B;
    ADC_handle_structure.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    ADC_handle_structure.Init.ScanConvMode = ENABLE;
    ADC_handle_structure.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
    ADC_handle_structure.Init.ContinuousConvMode = DISABLE;
    ADC_handle_structure.Init.NbrOfConversion = 2;
    ADC_handle_structure.Init.ExternalTrigConv = ADC_SOFTWARE_START;

    HAL_ADC_Init(&ADC_handle_structure);

    /* Channel configuration */
    ADC_ChannelConfTypeDef ADC_channel_structure;

    /* Configure channel 6 (POTI_1) */
    ADC_channel_structure.Channel = ADC_CHANNEL_6;
    ADC_channel_structure.Rank = 1;
    ADC_channel_structure.SamplingTime = ADC_SAMPLETIME_84CYCLES;
    HAL_ADC_ConfigChannel(&ADC_handle_structure, &ADC_channel_structure);

    /* Configure channel 7 (POTI_2) */
    ADC_channel_structure.Channel = ADC_CHANNEL_7;
    ADC_channel_structure.Rank = 2;
    ADC_channel_structure.SamplingTime = ADC_SAMPLETIME_84CYCLES;
    HAL_ADC_ConfigChannel(&ADC_handle_structure, &ADC_channel_structure);
}

/**
 * @brief  Reads the raw ADC value of the selected potentiometer.
 *         Starts a conversion sequence for both channels and returns
 *         either the first or second conversion result.
 * @param  poti_num  Potentiometer identifier:
 *                   - POTI_1 : read value from channel 6
 *                   - POTI_2 : read value from channel 7
 * @return uint32_t  Raw ADC value of the requested potentiometer.
 *         Returns 0 if poti_num is invalid (>2) or on timeout.
 */
uint32_t potis_get_val(uint8_t poti_num)
{
    if (poti_num > 2) {
        return 0;
    }

    HAL_StatusTypeDef adc_conv_status;
    uint32_t u32_poti1_value = 0;
    uint32_t u32_poti2_value = 0;

    /* Start ADC conversion */
    HAL_ADC_Start(&ADC_handle_structure);

    /* Read first conversion (POTI_1) */
    adc_conv_status = HAL_ADC_PollForConversion(&ADC_handle_structure, ADC_POLL_TIMEOUT_MS);
    if (adc_conv_status != HAL_TIMEOUT) {
        u32_poti1_value = HAL_ADC_GetValue(&ADC_handle_structure);
    }

    /* Read second conversion (POTI_2) */
    adc_conv_status = HAL_ADC_PollForConversion(&ADC_handle_structure, ADC_POLL_TIMEOUT_MS);
    if (adc_conv_status != HAL_TIMEOUT) {
        u32_poti2_value = HAL_ADC_GetValue(&ADC_handle_structure);
    }

    /* Stop ADC conversion */
    HAL_ADC_Stop(&ADC_handle_structure);

    /* Return value depending on selected potentiometer */
    return (poti_num == POTI_1) ? u32_poti1_value : u32_poti2_value;
}

/* Static module functions (implementation) */

/**
 * @brief  Initializes the necessary GPIOs for the ADC channels.
 *         Configures PA6 and PA7 as analog inputs without pull resistors.
 * @param  None
 * @return None
 */
void potis_init_gpio(void)
{
    __HAL_RCC_GPIOA_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_InitStruct;

    GPIO_InitStruct.Pin = POTENTIOMETER1_GPIO_PIN | POTENTIOMETER2_GPIO_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_MEDIUM;

    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}
