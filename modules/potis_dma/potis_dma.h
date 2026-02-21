/**
**************************************************
* @file potis_dma.h
* @author Mahmoud Mahmoud & Judy Abou Rmeh
* @version v1.0
* @date 15.11.25
* @brief Potentiometer module using ADC1 with DMA for continuous sampling.
*
*        This module provides two ways to access the potentiometer data:
*        1) By calling 'potis_dma_get_val(POTI_x)' which internally
*           filters the DMA buffer before returning the value.
*        2) By calling 'potis_dma_filter_data()' in the application and
*           then reading the filtered values from the global array
*           'g_u32_potis_filtered_data[POTI_x]'.
**************************************************
*/
#ifndef POTIS_DMA_POTIS_DMA_H_
#define POTIS_DMA_POTIS_DMA_H_

/* Includes */
#include "stm32f4xx.h"

/* Public Preprocessor defines */
/**
 * @brief GPIO pin for potentiometer 1 (ADC channel 6) on test board.
 */
#define POTENTIOMETER1_GPIO_PIN GPIO_PIN_6

/**
 * @brief GPIO pin for potentiometer 2 (ADC channel 7) on test board.
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
 * @brief Length of the filtered data array (one entry per potentiometer).
 */
#define FILTERED_DATA_ARRAY_LENGTH 2

/**
 * @brief Length of the raw DMA data buffer (interleaved samples).
 */
#define NON_FILTERED_DATA_ARRAY_LENGTH 200

/**
 * @brief Timeout in milliseconds for ADC conversion polling (if used).
 */
#define ADC_POLL_TIMEOUT_MS 1000

/**
 * @brief Index for potentiometer 1 in the filtered data array.
 */
#define POTI_1 0

/**
 * @brief Index for potentiometer 2 in the filtered data array.
 */
#define POTI_2 1

/* Public Preprocessor macros */
/* Public type definitions */

/* Public variables */
/**
 * @brief Global array holding the filtered potentiometer values.
 *        Index with POTI_1 or POTI_2.
 */
extern uint32_t g_u32_potis_filtered_data[FILTERED_DATA_ARRAY_LENGTH];

/* Public functions (prototypes) */
/**
 * @brief  Initializes hardware resources for ADC+DMA potentiometer sampling.
 *         - Configures GPIO pins (PA6, PA7) as analog inputs
 *         - Configures DMA2 Stream0 Channel 0
 *         - Configures ADC1 with two channels (6 and 7)
 * @param  None
 * @return None
 */
void potis_dma_init(void);

/**
 * @brief  Starts ADC1 DMA transfers into the raw data buffer.
 * @param  None
 * @return None
 */
void potis_dma_start(void);

/**
 * @brief  Filters the raw DMA data and calculates averaged values
 *         for each potentiometer.
 *
 *         The DMA buffer contains interleaved samples:
 *         index 0,2,4,... -> POTI_1
 *         index 1,3,5,... -> POTI_2
 *
 *         This function:
 *         - Sums up all samples per potentiometer
 *         - Divides by (NON_FILTERED_DATA_ARRAY_LENGTH / 2)
 *         - Stores the results into 'g_u32_potis_filtered_data'
 *
 * @param  None
 * @return None
 */
void potis_dma_filter_data(void);

/**
 * @brief  Returns the filtered ADC value of the selected potentiometer.
 *
 *         Internally calls 'potis_dma_filter_data()' before returning
 *         the filtered value.
 *
 * @param  poti_num  Potentiometer index:
 *                   - POTI_1 : potentiometer 1
 *                   - POTI_2 : potentiometer 2
 * @return uint32_t  Filtered ADC value of the requested potentiometer.
 *         Returns 0 if poti_num is invalid (>=2).
 */
uint32_t potis_dma_get_val(uint8_t poti_num);

#endif /* POTIS_DMA_POTIS_DMA_H_ */
