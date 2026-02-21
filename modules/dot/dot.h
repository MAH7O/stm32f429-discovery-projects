/**
**************************************************
* @file dot.h
* @author Mahmoud Mahmoud / Judy Abou Rmeh
* @version v1.0
* @date 30.11.25
* @brief Module for controlling a dot-LED (or single segment) using TIM1 PWM.
**************************************************
*/
#ifndef DOT_DOT_H_
#define DOT_DOT_H_

/* Includes */
#include "stm32f4xx.h"

/* Public Preprocessor defines */

/**
 * @brief GPIO pin of the dot-LED output (TIM1 channel).
 */
#define DOT_GPIO_PIN        GPIO_PIN_11

/**
 * @brief Digit control pins for multiplexing (on GPIOD).
 */
#define DOT_CNTL1_GPIO_PIN  GPIO_PIN_14
#define DOT_CNTL2_GPIO_PIN  GPIO_PIN_15
#define DOT_CNTL3_GPIO_PIN  GPIO_PIN_0
#define DOT_CNTL4_GPIO_PIN  GPIO_PIN_1

/**
 * @brief Combined mask of all digit control pins.
 */
#define DOT_ALL_CNTL_GPIO_PINS \
    (DOT_CNTL1_GPIO_PIN | DOT_CNTL2_GPIO_PIN | DOT_CNTL3_GPIO_PIN | DOT_CNTL4_GPIO_PIN)

/**
 * @brief Operating mode: blinking (frequency controlled via prescaler).
 */
#define DOT_BLINKING_MODE   1

/**
 * @brief Operating mode: dimming (brightness controlled via duty cycle).
 */
#define DOT_DIMMING_MODE    2

/**
 * @brief Maximum allowed mode value.
 */
#define DOT_MODES_MAX       DOT_DIMMING_MODE

/**
 * @brief Maximum blink speed (upper bound for frequency input).
 */
#define DOT_MAX_BLINKSPEED  100000

/**
 * @brief Minimum blink speed (lower bound for frequency input).
 */
#define DOT_MIN_BLINKSPEED  10000

/**
 * @brief Maximum brightness value (TIM compare value for PWM duty).
 */
#define DOT_MAX_BRIGHTNESS  255

/* Public variables */
/**
 * @brief Global timer handle used by the dot module (TIM1).
 */
extern TIM_HandleTypeDef tim_handle_struct;

/* Public functions (prototypes) */

/**
 * @brief  Initializes the dot-LED GPIOs.
 *         - Enables GPIOD and GPIOE clocks
 *         - Configures digit control pins as push-pull outputs
 *         - Configures the dot GPIO as alternate function for TIM1
 *         - Sets all control pins to off-state.
 * @param  None
 * @return None
 */
void dot_esd_init(void);

/**
 * @brief  Initializes TIM1 for either blinking or dimming mode.
 *         - Configures TIM1 base settings (prescaler, period)
 *         - Configures TIM1 channel 2 in PWM mode
 *         - Starts TIM1 output.
 * @param  mode  DOT_BLINKING_MODE or DOT_DIMMING_MODE.
 * @return None
 */
void dot_timer_init(uint8_t mode);

/**
 * @brief  Changes the blink speed of the dot-LED by adjusting the TIM1 prescaler.
 *         The given frequency value is clamped between DOT_MIN_BLINKSPEED
 *         and DOT_MAX_BLINKSPEED.
 * @param  frequency Desired effective frequency value (used for prescaler calculation).
 * @return None
 */
void dot_change_blink_speed(uint32_t frequency);

/**
 * @brief  Changes the brightness of the dot-LED by adjusting the PWM duty cycle.
 *         The brightness value is clamped to DOT_MAX_BRIGHTNESS.
 * @param  brightness PWM compare value (0..DOT_MAX_BRIGHTNESS).
 * @return None
 */
void dot_change_brightness(uint32_t brightness);

#endif /* DOT_DOT_H_ */
