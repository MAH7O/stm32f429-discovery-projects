/**
 ******************************************************************************
 * @file        fan.c
 * @author      Mahmoud Mahmoud / Judy Abou Rmeh
 * @date        21.12.2025
 * @version     V1.0
 * @brief       Fan control implementation (PWM + tacho + PI controller)
 *
 * Functionality:
 * - Generates a PWM signal on TIM9 CH1 for fan speed control
 * - Measures fan tacho pulses via EXTI and computes RPM using TIM2 timestamps
 * - Applies a median filter to RPM values (median module)
 * - Provides a PI controller to reach a target RPM
 *
 * Peripherals:
 * - GPIOE:
 *   - PWM pin configured as AF (TIM9)
 *   - Tacho pin configured as EXTI rising edge interrupt
 * - TIM9: PWM generator
 * - TIM2: free-running timer @ 1 MHz
 * - EXTI9_5_IRQn: interrupt for tacho pulses
 ******************************************************************************
 */

#include "fan.h"
#include "median/median.h"

/* Static module variables -------------------------------------------------- */
/**
 * @brief TIM9 handle used for PWM generation.
 */
static TIM_HandleTypeDef g_fan_tim9_handle_struct;

/**
 * @brief TIM2 handle used as free-running timestamp timer for tacho.
 */
static TIM_HandleTypeDef g_fan_tim2_handle_struct;

/**
 * @brief Last TIM2 tick count at previous tacho pulse.
 */
static volatile uint32_t g_u32_ticks_last = 0u;

/**
 * @brief Time difference (ticks) between two tacho pulses (TIM2 @ 1 MHz).
 */
static volatile uint32_t g_u32_time_diff = 0u;

/**
 * @brief CPU tick timestamp (ms) when last tacho pulse was received.
 */
static volatile uint32_t g_u32_cpu_ticks_now = 0u;

/**
 * @brief Target RPM set by application.
 */
static uint32_t g_u32_target_rpm = 0u;

/* PI controller parameters ------------------------------------------------- */
/**
 * @brief Controller sampling time in seconds.
 */
static float g_f_ta = 0.02f;

/**
 * @brief Proportional gain of PI controller.
 */
static float g_f_kp = 0.04f;

/**
 * @brief Integral gain of PI controller.
 */
static float g_f_ki = 0.03f;

/* Static function prototypes ---------------------------------------------- */
static void fan_gpio_init(void);
static void fan_timer_init(void);
static void fan_tacho_timer_init(void);
static void fan_init_interrupt(void);

/* Public functions --------------------------------------------------------- */
void fan_control_init(void)
{
    fan_gpio_init();
    fan_timer_init();
    fan_init_interrupt();
    fan_tacho_timer_init();
}

uint32_t fan_get_filtered_rpm(void)
{
    if ((HAL_GetTick() - g_u32_cpu_ticks_now) > 1000u) {
        return 0u;
    }

    if (g_u32_time_diff == 0u) {
        return 0u;
    }

    uint32_t u32_rpm =
        (60u * 1000000ul) / (2u * g_u32_time_diff);

    return median_get_median(u32_rpm);
}

void fan_change_target_rpm(uint32_t target_rpm)
{
    g_u32_target_rpm = target_rpm;
}

uint32_t fan_get_target_rpm(void)
{
    return g_u32_target_rpm;
}

void fan_update_pi_controller(void)
{
    static float f_esum = 0.0f;

    float f_error  =
        (float)g_u32_target_rpm - (float)fan_get_filtered_rpm();

    float f_output =
        g_f_kp * f_error + g_f_ki * f_esum;

    if (f_output > 100.0f) {
        f_output = 100.0f;
    } else if (f_output < 0.0f) {
        f_output = 0.0f;
    } else {
        f_esum += f_error * g_f_ta;
    }

    __HAL_TIM_SET_COMPARE(&g_fan_tim9_handle_struct,
                          TIM_CHANNEL_1,
                          (g_fan_tim9_handle_struct.Init.Period + 1u)
                          * f_output / 100.0f);
}

/* Static module functions -------------------------------------------------- */
static void fan_gpio_init(void)
{
    __HAL_RCC_GPIOE_CLK_ENABLE();

    GPIO_InitTypeDef gpio_init_struct;

    /* PWM output (TIM9 AF) */
    gpio_init_struct.Pin       = FAN_PWM_INPUT;
    gpio_init_struct.Mode      = GPIO_MODE_AF_OD;
    gpio_init_struct.Pull      = GPIO_NOPULL;
    gpio_init_struct.Speed     = GPIO_SPEED_FREQ_MEDIUM;
    gpio_init_struct.Alternate = GPIO_AF3_TIM9;
    HAL_GPIO_Init(GPIOE, &gpio_init_struct);

    /* Tacho input (EXTI) */
    gpio_init_struct.Pin  = FAN_TACHO_OUTPUT;
    gpio_init_struct.Mode = GPIO_MODE_IT_RISING;
    gpio_init_struct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(GPIOE, &gpio_init_struct);
}

static void fan_timer_init(void)
{
    __HAL_RCC_TIM9_CLK_ENABLE();

    TIM_OC_InitTypeDef tim_oc_handle_struct;

    g_fan_tim9_handle_struct.Instance               = TIM9;
    g_fan_tim9_handle_struct.Init.Prescaler         =
        (SystemCoreClock / 10000000u) - 1u;
    g_fan_tim9_handle_struct.Init.Period            = 400u - 1u;
    g_fan_tim9_handle_struct.Init.CounterMode       =
        TIM_COUNTERMODE_UP;
    g_fan_tim9_handle_struct.Init.ClockDivision     =
        TIM_CLOCKDIVISION_DIV1;
    g_fan_tim9_handle_struct.Init.RepetitionCounter = 0u;

    tim_oc_handle_struct.OCMode     = TIM_OCMODE_PWM1;
    tim_oc_handle_struct.Pulse      = (400u / 100u) * 50u;
    tim_oc_handle_struct.OCPolarity = TIM_OCPOLARITY_HIGH;
    tim_oc_handle_struct.OCFastMode = TIM_OCFAST_DISABLE;

    HAL_TIM_Base_Init(&g_fan_tim9_handle_struct);
    HAL_TIM_OC_ConfigChannel(&g_fan_tim9_handle_struct,
                             &tim_oc_handle_struct,
                             TIM_CHANNEL_1);
    HAL_TIM_OC_Start(&g_fan_tim9_handle_struct,
                     TIM_CHANNEL_1);
}

static void fan_tacho_timer_init(void)
{
    __HAL_RCC_TIM2_CLK_ENABLE();

    g_fan_tim2_handle_struct.Instance           = TIM2;
    g_fan_tim2_handle_struct.Init.Prescaler     =
        (SystemCoreClock / 1000000u) - 1u;
    g_fan_tim2_handle_struct.Init.Period        = 0xFFFFFFFF;
    g_fan_tim2_handle_struct.Init.CounterMode   =
        TIM_COUNTERMODE_UP;
    g_fan_tim2_handle_struct.Init.ClockDivision =
        TIM_CLOCKDIVISION_DIV1;

    HAL_TIM_Base_Init(&g_fan_tim2_handle_struct);
    HAL_TIM_Base_Start(&g_fan_tim2_handle_struct);
}

static void fan_init_interrupt(void)
{
    HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0u, 0u);
    HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
}

/* Interrupt / callback section -------------------------------------------- */
void HAL_GPIO_EXTI_Callback(uint16_t gpio_pin)
{
    if (gpio_pin == FAN_TACHO_OUTPUT) {
        uint32_t u32_ticks_now =
            __HAL_TIM_GET_COUNTER(&g_fan_tim2_handle_struct);

        g_u32_time_diff  = u32_ticks_now - g_u32_ticks_last;
        g_u32_ticks_last = u32_ticks_now;
        g_u32_cpu_ticks_now = HAL_GetTick();
    }
}

void EXTI9_5_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(FAN_TACHO_OUTPUT);
}
