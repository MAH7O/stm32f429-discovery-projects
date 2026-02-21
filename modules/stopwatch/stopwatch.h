/**
**************************************************
* @file stopwatch.h
* @author Mahmoud Mahmoud / Judy Abou Rmeh
* @version v1.0
* @date 30.11.25
* @brief Simple stopwatch module with lap functionality using TIM1 and EXTI0.
**************************************************
*/
#ifndef STOPWATCH_STOPWATCH_H_
#define STOPWATCH_STOPWATCH_H_

/* Includes */
#include "stm32f4xx.h"
#include <stdbool.h>

/* Public preprocessor defines */

/**
 * @brief Maximum number of laps that can be stored.
 */
#define STOPWATCH_LAPS        10

/**
 * @brief Debounce time in milliseconds for the start/lap button.
 */
#define STOPWATCH_DEBOUNCE_MS 50

/* Public global flags and indices */

/**
 * @brief Flag that is set to true when a new lap has been added.
 *
 *        This flag is set inside the EXTI interrupt and can be
 *        polled in the main loop to update the UI or LCD.
 */
extern volatile bool    bool_stopwatch_lap_added_flag;

/**
 * @brief Index of the last added lap.
 *
 *        Valid only when 'stopwatch_lap_added_flag' is true.
 */
extern volatile uint8_t u8_stopwatch_lap_added_index;

/* Public function prototypes */

/**
 * @brief  Initializes TIM1 as the stopwatch timer.
 *         - Configures TIM1 with 1s period
 *         - Does not start the timer yet.
 * @param  None
 * @return None
 */
void stopwatch_init_timer(void);

/**
 * @brief  Initializes GPIO used for stopwatch button input.
 *         - Configures PA0 as external interrupt source on rising edge.
 * @param  None
 * @return None
 */
void stopwatch_init_gpio(void);

/**
 * @brief  Initializes NVIC priorities and enables timer and EXTI interrupts.
 * @param  None
 * @return None
 */
void stopwatch_init_interrupt(void);

/**
 * @brief  Returns the current stopwatch time in milliseconds (0..999).
 *
 *         The value is derived from the TIM1 counter value.
 *
 * @param  None
 * @return Current milliseconds part of the stopwatch time.
 */
uint16_t stopwatch_get_current_milliseconds(void);

/**
 * @brief  Returns the current stopwatch time in seconds.
 * @param  None
 * @return Current seconds part of the stopwatch time.
 */
uint16_t stopwatch_get_current_seconds(void);

/**
 * @brief  Returns the current stopwatch time in minutes.
 * @param  None
 * @return Current minutes part of the stopwatch time.
 */
uint16_t stopwatch_get_current_minutes(void);

/**
 * @brief  Returns a pointer to the lap times (milliseconds).
 * @param  None
 * @return Pointer to array of lap milliseconds.
 */
volatile uint16_t* stopwatch_get_lap_milliseconds(void);

/**
 * @brief  Returns a pointer to the lap times (seconds).
 * @param  None
 * @return Pointer to array of lap seconds.
 */
volatile uint16_t* stopwatch_get_lap_seconds(void);

/**
 * @brief  Returns a pointer to the lap times (minutes).
 * @param  None
 * @return Pointer to array of lap minutes.
 */
volatile uint16_t* stopwatch_get_lap_minutes(void);

/**
 * @brief  Returns the total number of laps that have been recorded
 *         since the stopwatch started.
 *
 *         This value is a simple up-counter and is independent of
 *         the number of laps stored in the internal arrays
 *         (which are limited to STOPWATCH_LAPS).
 *
 * @return Total lap count (0..65535).
 */
uint16_t stopwatch_get_lap_counts(void);

/**
 * @brief  Returns the index where the next lap will be stored.
 *
 *         This index is incremented modulo STOPWATCH_LAPS.
 *
 * @param  None
 * @return Current lap index (0..STOPWATCH_LAPS-1).
 */
uint8_t stopwatch_get_current_lap_index(void);

#endif /* STOPWATCH_STOPWATCH_H_ */
