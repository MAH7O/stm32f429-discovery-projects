/**
**************************************************
* @file potis_dma.c
* @author Mahmoud Mahmoud & Judy Abou Rmeh
* @version v1.0
* @date 15.11.25
* @brief ADC1 + DMA based potentiometer reading module for STM32F4.
@verbatim
==================================================
			### Resources used ###
	GPIO:  PA6 (POTENTIOMETER1_GPIO_PIN, ADC1_IN6)
       	   PA7 (POTENTIOMETER2_GPIO_PIN, ADC1_IN7)
	ADC:   ADC1 (2 channels: CH6, CH7)
	DMA:   DMA2 Stream0 Channel 0 (ADC1)
==================================================
				### Usage ###
	(#) Call 'potis_dma_init()' once during system initialization to:
    	- Enable GPIOA, ADC1 and DMA2 clocks
    	- Configure PA6 and PA7 as analog inputs
    	- Configure DMA2 Stream0 Channel 0 for ADC1
    	- Set up ADC1 for continuous, scan-mode, 2-channel conversion

	(#) Call 'potis_dma_start()' to start ADC1 with DMA transfers.

	(#) To access the potentiometer values you have two options:
    	1) Simple access:
       	   - Call 'potis_dma_get_val(POTI_1)' or 'potis_dma_get_val(POTI_2)'
       	   - The function internally filters the DMA buffer and returns
         the averaged value of the selected potentiometer.

    	2) Manual filtering in the application:
       	   - Periodically call 'potis_dma_filter_data()' in your main loop
         	 or in a timer callback
       	   - Then read the filtered values directly from:
           	   g_u32_potis_filtered_data[POTI_1]
           	   g_u32_potis_filtered_data[POTI_2]
==================================================
@endverbatim
**************************************************
*/

/* Includes */
#include "potis_dma.h"

/* Preprocessor defines */
/* Preprocessor macros */
/* Module intern type definitions */

/* Static module variables */
/**
 * @brief DMA handle structure for DMA2 Stream0 Channel 0.
 */
DMA_HandleTypeDef DMA_handle_structure;

/**
 * @brief ADC handle structure for ADC1.
 */
ADC_HandleTypeDef ADC_handle_structure;

/**
 * @brief Raw DMA data buffer (interleaved samples of both potentiometers).
 *
 *        Even indices  (0,2,4,...) belong to POTI_1.
 *        Odd indices   (1,3,5,...) belong to POTI_2.
 */
uint32_t u32_potis_data[NON_FILTERED_DATA_ARRAY_LENGTH];

/**
 * @brief Filtered potentiometer values (averaged).
 *
 *        Index with POTI_1 and POTI_2.
 */
uint32_t g_u32_potis_filtered_data[FILTERED_DATA_ARRAY_LENGTH];

/* Static module functions (prototypes) */
/**
 * @brief  Initializes GPIO pins for the ADC channels used by the potentiometers.
 *         Configures PA6 and PA7 as analog inputs.
 * @param  None
 * @return None
 */
void potis_gpio_init(void);

/**
 * @brief  Initializes DMA2 Stream0 Channel 0 for ADC1.
 * @param  None
 * @return None
 */
void potis_dma_hardware_init(void);

/* Public functions */

/**
 * @brief  Initializes potentiometer module hardware for ADC+DMA operation.
 *         - Configures GPIOA pins PA6/PA7 as analog inputs
 *         - Configures DMA2 Stream0 for ADC1
 *         - Configures ADC1 for continuous scan conversion of 2 channels
 * @param  None
 * @return None
 */
void potis_dma_init(void)
{
    potis_gpio_init();
    potis_dma_hardware_init();

    __HAL_RCC_ADC1_CLK_ENABLE();

    ADC_handle_structure.Instance = ADC1;
    ADC_handle_structure.DMA_Handle = &DMA_handle_structure;
    ADC_handle_structure.Init.ClockPrescaler      = ADC_CLOCK_SYNC_PCLK_DIV4;
    ADC_handle_structure.Init.Resolution          = ADC_RESOLUTION_12B;
    ADC_handle_structure.Init.DataAlign           = ADC_DATAALIGN_RIGHT;
    ADC_handle_structure.Init.ScanConvMode        = ENABLE;
    ADC_handle_structure.Init.EOCSelection        = ADC_EOC_SEQ_CONV;
    ADC_handle_structure.Init.ContinuousConvMode  = ENABLE;
    ADC_handle_structure.Init.NbrOfConversion     = 2;
    ADC_handle_structure.Init.ExternalTrigConv    = ADC_SOFTWARE_START;
    ADC_handle_structure.Init.DMAContinuousRequests = ENABLE;

    HAL_ADC_Init(&ADC_handle_structure);

    /* Channel configuration */
    ADC_ChannelConfTypeDef ADC_channel_structure1;

    /* Configure channel 6 (POTI_1) */
    ADC_channel_structure1.Channel      = ADC_CHANNEL_6;
    ADC_channel_structure1.Rank         = 1;
    ADC_channel_structure1.SamplingTime = ADC_SAMPLETIME_84CYCLES;
    HAL_ADC_ConfigChannel(&ADC_handle_structure, &ADC_channel_structure1);

    /* Configure channel 7 (POTI_2) */
    ADC_channel_structure1.Channel      = ADC_CHANNEL_7;
    ADC_channel_structure1.Rank         = 2;
    ADC_channel_structure1.SamplingTime = ADC_SAMPLETIME_84CYCLES;
    HAL_ADC_ConfigChannel(&ADC_handle_structure, &ADC_channel_structure1);
}

/**
 * @brief  Starts ADC1 conversions with DMA into the raw data buffer.
 * @param  None
 * @return None
 */
void potis_dma_start(void)
{
    HAL_ADC_Start_DMA(&ADC_handle_structure, u32_potis_data, NON_FILTERED_DATA_ARRAY_LENGTH);
}

/**
 * @brief  Filters the raw DMA data and stores averaged values per potentiometer.
 *
 *         Even indices of 'u32_potis_data' belong to POTI_1,
 *         odd indices belong to POTI_2. The function sums all values
 *         per potentiometer and divides by (NON_FILTERED_DATA_ARRAY_LENGTH / 2)
 *         to obtain the average.
 *
 * @param  None
 * @return None
 */
void potis_dma_filter_data(void)
{
    g_u32_potis_filtered_data[POTI_1] = 0;
    g_u32_potis_filtered_data[POTI_2] = 0;

    for (int i = 0; i < NON_FILTERED_DATA_ARRAY_LENGTH; i++) {
        if (i % 2 == 0) {
            g_u32_potis_filtered_data[POTI_1] += u32_potis_data[i];
        } else {
            g_u32_potis_filtered_data[POTI_2] += u32_potis_data[i];
        }
    }

    g_u32_potis_filtered_data[POTI_1] /= (NON_FILTERED_DATA_ARRAY_LENGTH / 2);
    g_u32_potis_filtered_data[POTI_2] /= (NON_FILTERED_DATA_ARRAY_LENGTH / 2);
}

/**
 * @brief  Returns the filtered ADC value of the selected potentiometer.
 *
 *         Internally this function calls 'potis_dma_filter_data()'
 *         before returning the value from 'g_u32_potis_filtered_data'.
 *
 * @param  poti_num  Potentiometer index:
 *                   - POTI_1 : potentiometer 1
 *                   - POTI_2 : potentiometer 2
 * @return uint32_t  Filtered ADC value for the selected potentiometer.
 *         Returns 0 if poti_num is invalid (>=2).
 */
uint32_t potis_dma_get_val(uint8_t poti_num)
{
    if (poti_num >= 2) {
        return 0;
    }

    potis_dma_filter_data();

    return g_u32_potis_filtered_data[poti_num];
}

/* Static module functions (implementation) */

/**
 * @brief  Initializes the necessary GPIOs for the ADC channels.
 *         Configures PA6 and PA7 as analog inputs without pull resistors.
 * @param  None
 * @return None
 */
void potis_gpio_init(void)
{
    __HAL_RCC_GPIOA_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_InitStruct;

    GPIO_InitStruct.Pin   = POTENTIOMETER1_GPIO_PIN | POTENTIOMETER2_GPIO_PIN;
    GPIO_InitStruct.Mode  = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_MEDIUM;

    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

/**
 * @brief  Initializes DMA2 Stream0 Channel 0 for ADC1 transfers.
 * @param  None
 * @return None
 */
void potis_dma_hardware_init(void)
{
    __HAL_RCC_DMA2_CLK_ENABLE();

    DMA_handle_structure.Instance                 = DMA2_Stream0;
    DMA_handle_structure.Init.Channel             = DMA_CHANNEL_0;
    DMA_handle_structure.Init.Direction           = DMA_PERIPH_TO_MEMORY;
    DMA_handle_structure.Init.PeriphInc           = DMA_PINC_DISABLE;
    DMA_handle_structure.Init.MemInc              = DMA_MINC_ENABLE;
    DMA_handle_structure.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
    DMA_handle_structure.Init.MemDataAlignment    = DMA_MDATAALIGN_WORD;
    DMA_handle_structure.Init.Mode                = DMA_CIRCULAR;
    DMA_handle_structure.Init.Priority            = DMA_PRIORITY_MEDIUM;
    DMA_handle_structure.Init.FIFOMode            = DMA_FIFOMODE_DISABLE;

    HAL_DMA_Init(&DMA_handle_structure);
}
