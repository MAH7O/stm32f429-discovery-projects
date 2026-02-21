/**
**************************************************
* @file utils.h
* @author Mahmoud Mahmoud / Judy Abou Rmeh
* @version v1.0
* @date 15.11.25
* @brief Utility module providing helper functions such as delay and GPIO port write.
**************************************************
*/
#ifndef UTILS_UTILS_H_
#define UTILS_UTILS_H_

/* Includes */
#include "stm32f4xx.h"

/* Public functions (prototypes) */

/**
 * @brief  Delays execution for a specified number of milliseconds.
 *         Wrapper for HAL_Delay().
 * @param  t Duration in milliseconds.
 * @return None
 */
void utils_delay_ms(uint32_t t);

/**
 * @brief  Writes a complete 16-bit output pattern to a GPIO port.
 *         The value in 'bitmask' is written directly into ODR (Output Data Register).
 * @param  GPIOx Pointer to the GPIO port (e.g. GPIOA, GPIOB, ...)
 * @param  bitmask 16-bit value to write into ODR.
 * @return None
 */
void utils_gpio_port_write(GPIO_TypeDef *GPIOx, uint16_t bitmask);

#endif /* UTILS_UTILS_H_ */
