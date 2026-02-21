/**
**************************************************
* @file dot.c
* @author Mahmoud Mahmoud / Judy Abou Rmeh
* @version v1.0
* @date 30.11.25
* @brief Implementation of dot-LED control using TIM1 PWM.
@verbatim
==================================================
				### Resources used ###
	GPIO:
  	  - GPIOD: DOT_CNTL1_GPIO_PIN, DOT_CNTL2_GPIO_PIN,
           	   DOT_CNTL3_GPIO_PIN, DOT_CNTL4_GPIO_PIN
  	  - GPIOE: DOT_GPIO_PIN (AF1 -> TIM1 CH2)

	TIMER:
  	  - TIM1 CH2 for PWM-based blinking/dimming
==================================================
					### Usage ###
	(#) Call 'dot_esd_init()' once to configure GPIOs for the dot-LED.

	(#) Call 'dot_timer_init(mode)' with one of:
    	- DOT_BLINKING_MODE: timer configured for visible blinking speed
    	- DOT_DIMMING_MODE : timer configured for brightness control

	(#) To change behavior at runtime:
    	- Use 'dot_change_blink_speed(freq)' to adjust blink speed
      	  (valid only in blinking mode, but prescaler still changes).
    	- Use 'dot_change_brightness(brightness)' to control LED intensity
      	  via PWM duty cycle.
==================================================
@endverbatim
**************************************************
*/

/* Includes */
#include "dot.h"

/* Static / global variables */

/**
 * @brief Global TIM handle structure used for TIM1 configuration.
 */
TIM_HandleTypeDef tim_handle_struct;

/* Public functions */

/**
 * @brief  Initializes GPIOs for dot-LED and its digit control pins.
 * @param  None
 * @return None
 */
void dot_esd_init(void)
{
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_GPIOE_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_InitStruct;

    /* Configure digit control pins on GPIOD as outputs */
    GPIO_InitStruct.Pin   = DOT_ALL_CNTL_GPIO_PINS;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_MEDIUM;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    /* Configure dot output pin on GPIOE as alternate function (TIM1 CH2) */
    GPIO_InitStruct.Pin       = DOT_GPIO_PIN;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Alternate = GPIO_AF1_TIM1;
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

    /* Turn all digit positions on*/
    HAL_GPIO_WritePin(GPIOD, DOT_ALL_CNTL_GPIO_PINS, GPIO_PIN_SET);
}

/**
 * @brief  Initializes TIM1 in the requested mode (blinking or dimming).
 * @param  mode DOT_BLINKING_MODE or DOT_DIMMING_MODE.
 * @return None
 */
void dot_timer_init(uint8_t mode)
{
    if (mode > DOT_MODES_MAX) {
        mode = DOT_MODES_MAX;
    }

    __HAL_RCC_TIM1_CLK_ENABLE();

    TIM_OC_InitTypeDef tim_oc_handle_struct;

    tim_handle_struct.Instance = TIM1;

    /* Base configuration depends on mode */
    if (mode == DOT_BLINKING_MODE) {
        /* Slow blinking: lower base frequency, higher period */
        tim_handle_struct.Init.Prescaler = (SystemCoreClock / 10000U) - 1U;
        tim_handle_struct.Init.Period    = 10000U - 1U;
    } else {
        /* Dimming: higher base frequency, 8-bit resolution period */
        tim_handle_struct.Init.Prescaler = (SystemCoreClock / 100000U) - 1U;
        tim_handle_struct.Init.Period    = 255U - 1U;
    }

    tim_handle_struct.Init.CounterMode         = TIM_COUNTERMODE_UP;
    tim_handle_struct.Init.ClockDivision       = TIM_CLOCKDIVISION_DIV1;
    tim_handle_struct.Init.AutoReloadPreload   = TIM_AUTORELOAD_PRELOAD_DISABLE;
    tim_handle_struct.Init.RepetitionCounter   = 0;

    /* Configure PWM output on TIM1 CH2 */
    tim_oc_handle_struct.OCMode       = TIM_OCMODE_PWM1;
    tim_oc_handle_struct.Pulse        = 5000;                 /* Initial duty / compare value */
    tim_oc_handle_struct.OCIdleState  = TIM_OCIDLESTATE_SET;
    tim_oc_handle_struct.OCPolarity   = TIM_OCPOLARITY_LOW;
    tim_oc_handle_struct.OCNIdleState = TIM_OCNIDLESTATE_RESET;
    tim_oc_handle_struct.OCNPolarity  = TIM_OCNPOLARITY_LOW;
    tim_oc_handle_struct.OCFastMode   = TIM_OCFAST_DISABLE;

    HAL_TIM_Base_Init(&tim_handle_struct);
    HAL_TIM_OC_ConfigChannel(&tim_handle_struct, &tim_oc_handle_struct, TIM_CHANNEL_2);

    HAL_TIM_OC_Start(&tim_handle_struct, TIM_CHANNEL_2);
}

/**
 * @brief  Changes the blink speed by updating the TIM1 prescaler.
 * @param  frequency Desired effective frequency value.
 * @return None
 */
void dot_change_blink_speed(uint32_t frequency)
{
    if (frequency > DOT_MAX_BLINKSPEED) {
        frequency = DOT_MAX_BLINKSPEED;
    }
    if (frequency < DOT_MIN_BLINKSPEED) {
        frequency = DOT_MIN_BLINKSPEED;
    }

    __HAL_TIM_SET_PRESCALER(&tim_handle_struct, (SystemCoreClock / frequency) - 1U);
}

/**
 * @brief  Changes the brightness of the dot-LED by updating the PWM compare value.
 * @param  brightness Requested brightness (0..DOT_MAX_BRIGHTNESS).
 * @return None
 */
void dot_change_brightness(uint32_t brightness)
{
    if (brightness > DOT_MAX_BRIGHTNESS) {
        brightness = DOT_MAX_BRIGHTNESS;
    }
    __HAL_TIM_SET_COMPARE(&tim_handle_struct, TIM_CHANNEL_2, brightness);
}
