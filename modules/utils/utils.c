/**
**************************************************
* @file utils.c
* @author Mahmoud Mahmoud / Judy Abou Rmeh
* @version v1.0
* @date 15.11.25
* @brief Implementation of utility functions such as delay and GPIO port write.
**************************************************
*/

/* Includes */
#include "utils.h"

/* Public functions */

/**
 * @brief  Delays code execution for the specified number of milliseconds.
 *         This function simply wraps the HAL_Delay() function.
 * @param  t Duration in milliseconds.
 * @return None
 */
void utils_delay_ms(uint32_t t)
{
    HAL_Delay(t);
}

/**
 * @brief  Writes an entire 16-bit pattern to a GPIO port's ODR register.
 *         This overrides the complete output register of the port.
 * @param  GPIOx Pointer to GPIO port (e.g. GPIOA)
 * @param  bitmask 16-bit value to write to ODR.
 * @return None
 */
void utils_gpio_port_write(GPIO_TypeDef *GPIOx, uint16_t bitmask)
{
    GPIOx->ODR = bitmask;
}
