/**
**************************************************
* @file stopwatch.c
* @author Mahmoud Mahmoud / Judy Abou Rmeh
* @version v1.0
* @date 30.11.25
* @brief Stopwatch implementation using TIM1 for time base and
*        EXTI0 (PA0) as start/lap button with debounce.
@verbatim
==================================================
				### Resources used ###
	TIMER:
  	  - TIM1 as 1 Hz time base (period elapsed = 1 second)

	GPIO / EXTI:
  	  - PA0 as external interrupt (EXTI0) for start/lap button

	NVIC:
  	  - TIM1_UP_TIM10_IRQn for timer update interrupt
  	  - EXTI0_IRQn for button interrupt
==================================================
					### Behavior ###
	(#) On first button press:
    	- TIM1 is started with interrupt enabled
    	- Stopwatch begins counting

	(#) On subsequent button presses:
    	- Current time (minutes, seconds, milliseconds) is stored as a lap
    	- Up to STOPWATCH_LAPS laps are stored in circular fashion
    	- 'bool_stopwatch_timer_status' is set and the lap index recorded

	(#) Internally:
    	- Each timer period elapsed interrupt increments the seconds counter
    	- On 60 seconds, minutes are incremented and seconds reset
    	- Milliseconds are derived from the TIM1 counter value
==================================================
@endverbatim
**************************************************
*/

/* Includes */
#include "stopwatch.h"

/* Global variables */

/**
 * @brief TIM1 handle used as the stopwatch time base.
 */
TIM_HandleTypeDef stopwatch_timer;

/**
 * @brief Arrays holding each lap time in milliseconds, seconds, and minutes.
 *
 *        Index range: 0..STOPWATCH_LAPS-1
 */
volatile uint16_t u16_stopwatch_laps_in_milliseconds[STOPWATCH_LAPS];
volatile uint16_t u16_stopwatch_laps_in_seconds[STOPWATCH_LAPS];
volatile uint16_t u16_stopwatch_laps_in_minutes[STOPWATCH_LAPS];

/**
 * @brief Current stopwatch time (seconds and minutes).
 */
volatile uint16_t u16_stopwatch_current_in_seconds = 0;
volatile uint16_t u16_stopwatch_current_in_minutes = 0;

/**
 * @brief Internal state flags and indices.
 */
volatile bool     bool_stopwatch_timer_status   = false;
volatile uint8_t  u8_stopwatch_lap_index        = 0;
volatile uint16_t u16_stopwatch_lap_counter     = 0;

/**
 * @brief Public flags used for lap notification.
 */
volatile uint8_t u8_stopwatch_lap_added_index  = 0;
volatile bool    bool_stopwatch_lap_added_flag = false;

/**
 * @brief Tick count at last valid button press, used for debounce.
 */
volatile uint32_t u32_stopwatch_last_button_tick = 0;

/* Public function implementations */

void stopwatch_init_timer(void)
{
    __HAL_RCC_TIM1_CLK_ENABLE();

    stopwatch_timer.Instance = TIM1;
    /* Timer tick: SystemCoreClock / (Prescaler+1) = 10 kHz
       Period: 10000-1 => 1 second per overflow */
    stopwatch_timer.Init.Prescaler         = (SystemCoreClock / 10000) - 1;
    stopwatch_timer.Init.Period           = 10000 - 1;
    stopwatch_timer.Init.CounterMode      = TIM_COUNTERMODE_UP;
    stopwatch_timer.Init.ClockDivision    = TIM_CLOCKDIVISION_DIV1;
    stopwatch_timer.Init.AutoReloadPreload= TIM_AUTORELOAD_PRELOAD_DISABLE;
    stopwatch_timer.Init.RepetitionCounter= 0;

    HAL_TIM_Base_Init(&stopwatch_timer);
}

void stopwatch_init_gpio(void)
{
    __HAL_RCC_GPIOA_CLK_ENABLE();

    GPIO_InitTypeDef gpio_init_user;
    gpio_init_user.Pin  = GPIO_PIN_0;
    gpio_init_user.Mode = GPIO_MODE_IT_RISING;
    gpio_init_user.Pull = GPIO_NOPULL;

    HAL_GPIO_Init(GPIOA, &gpio_init_user);
}

void stopwatch_init_interrupt(void)
{
    /* Timer interrupt: highest priority group 0, subpriority 0 */
    HAL_NVIC_SetPriority(TIM1_UP_TIM10_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(TIM1_UP_TIM10_IRQn);

    /* EXTI interrupt for PA0: group priority 0, subpriority 1 */
    HAL_NVIC_SetPriority(EXTI0_IRQn, 0, 1);
    HAL_NVIC_EnableIRQ(EXTI0_IRQn);
}

uint16_t stopwatch_get_current_milliseconds(void)
{
    /* Timer counter ticks at 10 kHz => 0.1 ms per tick
       Divide by 10 to obtain milliseconds (0..999) */
    return (__HAL_TIM_GET_COUNTER(&stopwatch_timer) / 10U);
}

uint16_t stopwatch_get_current_seconds(void)
{
    return u16_stopwatch_current_in_seconds;
}

uint16_t stopwatch_get_current_minutes(void)
{
    return u16_stopwatch_current_in_minutes;
}

volatile uint16_t* stopwatch_get_lap_milliseconds(void)
{
    return u16_stopwatch_laps_in_milliseconds;
}

volatile uint16_t* stopwatch_get_lap_seconds(void)
{
    return u16_stopwatch_laps_in_seconds;
}

volatile uint16_t* stopwatch_get_lap_minutes(void)
{
    return u16_stopwatch_laps_in_minutes;
}

uint16_t stopwatch_get_lap_counts(void)
{
    return u16_stopwatch_lap_counter;
}

uint8_t stopwatch_get_current_lap_index(void)
{
    return u8_stopwatch_lap_index;
}

/* HAL callback implementations */

/**
 * @brief Timer period elapsed callback (called by HAL on TIM update event).
 *
 *        Increments stopwatch time by one second and handles minute rollover.
 *
 * @param htim Pointer to timer handle triggering the callback.
 * @return None
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (&stopwatch_timer == htim) {
        u16_stopwatch_current_in_seconds++;
        if (u16_stopwatch_current_in_seconds == 60U) {
            u16_stopwatch_current_in_minutes++;
            u16_stopwatch_current_in_seconds = 0U;
        }
    }
}

/**
 * @brief EXTI callback for GPIO external interrupts.
 *
 *        Handles button press on PA0:
 *        - If timer is stopped: start stopwatch
 *        - If timer is running: store current lap time
 *
 *        Includes debounce based on HAL tick time.
 *
 * @param GPIO_Pin Pin number that triggered the interrupt.
 * @return None
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if (GPIO_Pin == GPIO_PIN_0) {

        uint32_t u32_ticks_now = HAL_GetTick();

        /* Debounce: ignore if time since last press < STOPWATCH_DEBOUNCE_MS */
        if ((u32_ticks_now - u32_stopwatch_last_button_tick) < STOPWATCH_DEBOUNCE_MS) {
            return;
        }

        u32_stopwatch_last_button_tick = u32_ticks_now;

        if (!bool_stopwatch_timer_status) {
            /* First valid button press: start the stopwatch */
            HAL_TIM_Base_Start_IT(&stopwatch_timer);
            bool_stopwatch_timer_status = true;
        } else {
            /* Subsequent presses: store lap time */
            uint8_t u8_index = u8_stopwatch_lap_index;

            u16_stopwatch_laps_in_minutes[u8_index]      = stopwatch_get_current_minutes();
            u16_stopwatch_laps_in_seconds[u8_index]      = stopwatch_get_current_seconds();
            u16_stopwatch_laps_in_milliseconds[u8_index] = stopwatch_get_current_milliseconds();

            /* Advance circular index */
            u8_stopwatch_lap_index = (uint8_t)((u8_index + 1U) % STOPWATCH_LAPS);


            u16_stopwatch_lap_counter++;


            /* Notify application about new lap */
            u8_stopwatch_lap_added_index = u8_index;
            bool_stopwatch_lap_added_flag     = true;
        }
    }
}

/* IRQ handlers */

/**
 * @brief TIM1 update / TIM10 global interrupt handler.
 * @return None
 */
void TIM1_UP_TIM10_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&stopwatch_timer);
}

/**
 * @brief EXTI line 0 interrupt handler (for PA0).
 * @return None
 */
void EXTI0_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);
}
