/**
 ******************************************************************************
 * @file        fan.h
 * @author      Mahmoud Mahmoud / Judy Abou Rmeh
 * @date        21.12.2025
 * @version     V1.0
 * @brief       Public interface of the fan control module.
 *
 * @details
 * This module provides functions to control a fan using PWM and to
 * measure its rotational speed via a tachometer signal.
 * A PI controller is used to adjust the PWM duty cycle in order
 * to reach a defined target RPM.
 *
 * ----------------------------------------------------------------------------
 * 							### Functionality ###
 *  - PWM-based fan speed control
 *  - RPM measurement via tachometer (EXTI + timer)
 *  - Filtered RPM output
 *  - PI controller for closed-loop speed control
 *
 ******************************************************************************
 */

#ifndef FAN_FAN_H_
#define FAN_FAN_H_

/* Includes ---------------------------------------------------------------- */
#include "stm32f4xx.h"

/* Public Preprocessor Defines --------------------------------------------- */
/**
 * @brief GPIO port used for fan control signals.
 */
#define FAN_GPIO_PORT        GPIOE

/**
 * @brief GPIO pin used for PWM output to the fan.
 */
#define FAN_PWM_INPUT        GPIO_PIN_5

/**
 * @brief GPIO pin used for the fan tachometer signal.
 */
#define FAN_TACHO_OUTPUT     GPIO_PIN_6

/**
 * @brief Maximum fan speed in revolutions per minute.
 */
#define FAN_MAX_RPM          5000U

/* Public Function Prototypes ---------------------------------------------- */
/**
 * @brief Initializes the fan control module.
 *
 * Initializes GPIOs, timers and interrupts required for
 * fan speed control and RPM measurement.
 *
 * @return None
 */
void fan_control_init(void);

/**
 * @brief Returns the filtered fan speed in RPM.
 *
 * If no valid tachometer signal is detected within a timeout
 * period, the function returns 0.
 *
 * @return Current filtered fan speed in RPM.
 */
uint32_t fan_get_filtered_rpm(void);

/**
 * @brief Sets the target speed for the fan controller.
 *
 * @param target_rpm Target fan speed in RPM.
 * @return None
 */
void fan_change_target_rpm(uint32_t target_rpm);

/**
 * @brief Returns the currently set target speed.
 *
 * @return Target fan speed in RPM.
 */
uint32_t fan_get_target_rpm(void);

/**
 * @brief Updates the PI controller and PWM duty cycle.
 *
 * This function must be called periodically with the
 * controller sampling time.
 *
 * @return None
 */
void fan_update_pi_controller(void);

#endif /* FAN_FAN_H_ */
