/**
**************************************************
* @file potis.h
* @author Mahmoud Mahmoud & Judy Abou Rmeh
* @version v1.0
* @date 15.11.25
* @brief Module for reading potentiometer values via ADC1.
**************************************************
*/

#ifndef POTIS_POTIS_H_
#define POTIS_POTIS_H_

/* Includes */
#include "stm32f4xx.h"

/* Public Preprocessor defines */
/**
 * @brief GPIO pin for potentiometer 1 on test board.
 */
#define POTENTIOMETER1_GPIO_PIN GPIO_PIN_6

/**
 * @brief GPIO pin for potentiometer 2 on test board.
 */
#define POTENTIOMETER2_GPIO_PIN GPIO_PIN_7

/**
 * @brief Board supply/reference voltage in millivolts.
 */
#define TEST_BOARD_MILLIEVOLTAGE 3300

/**
 * @brief Maximum ADC value for 12-bit resolution (0..4095).
 */
#define ADC_12_BIT_RESOLUTION 4095

/**
 * @brief Identifier for potentiometer 1.
 */
#define POTI_1 1

/**
 * @brief Identifier for potentiometer 2.
 */
#define POTI_2 2

/**
 * @brief Timeout in milliseconds for ADC conversion polling.
 */
#define ADC_POLL_TIMEOUT_MS 1000

/* Public Preprocessor macros */

/* Public type definitions */

/* Public functions (prototypes) */

/**
 * @brief  Initializes hardware resources needed for the potentiometers.
 *         Configures GPIO pins and ADC1 (2 channels).
 * @param  None
 * @return None
 */
void potis_init(void);

/**
 * @brief  Reads the current raw ADC value of the selected potentiometer.
 * @param  poti_num  Identifier of the potentiometer:
 *                   - POTI_1 for potentiometer 1
 *                   - POTI_2 for potentiometer 2
 * @return uint32_t  Raw ADC conversion value (0..ADC_12_BIT_RESOLUTION).
 *         Returns 0 if an invalid potentiometer number is passed.
 */
uint32_t potis_get_val(uint8_t poti_num);

#endif /* POTIS_POTIS_H_ */
