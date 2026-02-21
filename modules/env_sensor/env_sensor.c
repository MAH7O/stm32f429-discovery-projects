/**
 ******************************************************************************
 * @file        env_sensor.c
 * @author      Mahmoud Mahmoud, Judy Abou Rmeh
 * @version     V1.0
 * @date        14.03.2019
 * @brief       Implementierung des Umweltsensor-Moduls (BME280 über I2C)
 *
 * @details
 * Dieses Modul initialisiert die I2C-Schnittstelle (GPIO + I2C1) sowie den
 * Umweltsensor (BME280) und stellt Funktionen zum Auslesen von Temperatur,
 * Luftdruck und Luftfeuchtigkeit bereit.
 *
 * Verwendete Module:
 *  - bme280
 *
 * Verwendete Peripherie / Ressourcen:
 *  - I2C1
 *  - GPIOB Pin 6 (SCL), GPIOB Pin 7 (SDA) im Alternate Function Mode (AF4)
 *
 ******************************************************************************
 */

/* Includes */
#include "env_sensor.h"

/* Static module variables */
static I2C_HandleTypeDef i2c_handle;

static struct bme280_dev dev;
static struct bme280_data sensor_data;
static struct bme280_settings settings;

/* Static module functions (prototypes) */
static void env_sensor_init_gpio(void);
static void env_sensor_init_i2c(void);
static void env_sensor_bme280_init(void);

static void env_sensor_delay_us(uint32_t period, void *intf_ptr);

static int8_t env_sensor_i2c_read(uint8_t reg_addr, uint8_t *data, uint32_t len, void *intf_ptr);
static int8_t env_sensor_i2c_write(uint8_t reg_addr, const uint8_t *data, uint32_t len, void *intf_ptr);

/* Public functions */

/**
 * @brief   Initialisiert das Umweltsensor-Modul
 *
 * @details
 * Initialisiert die GPIO-Pins für I2C, konfiguriert I2C1 und initialisiert
 * anschließend den BME280 inklusive Oversampling- und Filter-Einstellungen.
 *
 * @param   None
 * @return  None
 */
void env_sensor_init(void)
{
    env_sensor_init_gpio();
    env_sensor_init_i2c();
    env_sensor_bme280_init();
}

/**
 * @brief   Liest Messwerte des Umweltsensors aus
 *
 * @details
 * Setzt den BME280 in den Forced-Mode, wartet bis die Messung abgeschlossen ist
 * und liest anschließend Temperatur, Luftdruck und Luftfeuchtigkeit aus.
 *
 * @param   temperature Zeiger auf die Temperatur in Grad Celsius
 * @param   pressure    Zeiger auf den Luftdruck in hPa
 * @param   humidity    Zeiger auf die relative Luftfeuchtigkeit in Prozent
 *
 * @return  None
 */
void env_sensor_read_data(float *temperature, float *pressure, float *humidity)
{
    bme280_set_sensor_mode(BME280_POWERMODE_FORCED, &dev);
    dev.delay_us(50000, dev.intf_ptr);

    bme280_get_sensor_data(BME280_ALL, &sensor_data, &dev);

    *temperature = sensor_data.temperature;
    *pressure    = sensor_data.pressure;
    *humidity    = sensor_data.humidity;
}

/* Static module functions (implementation) */

/**
 * @brief   Initialisiert die GPIOs für I2C (SCL/SDA)
 *
 * @details
 * Aktiviert den GPIOB-Clock und konfiguriert PB6/PB7 als Alternate Function
 * Open-Drain mit Pull-Up für I2C1 (AF4).
 *
 * @param   None
 * @return  None
 */
static void env_sensor_init_gpio(void)
{
    __HAL_RCC_GPIOB_CLK_ENABLE();

    GPIO_InitTypeDef gpio_i2c_init;

    gpio_i2c_init.Pin       = GPIO_PIN_6 | GPIO_PIN_7;
    gpio_i2c_init.Mode      = GPIO_MODE_AF_OD;
    gpio_i2c_init.Pull      = GPIO_PULLUP;
    gpio_i2c_init.Speed     = GPIO_SPEED_FREQ_MEDIUM;
    gpio_i2c_init.Alternate = GPIO_AF4_I2C1;

    HAL_GPIO_Init(GPIOB, &gpio_i2c_init);
}

/**
 * @brief   Initialisiert I2C1
 *
 * @details
 * Aktiviert den I2C1-Clock und konfiguriert I2C mit 100 kHz, 7-bit Addressing
 * und Standard-Einstellungen (kein Dual-Address, kein General Call).
 *
 * @param   None
 * @return  None
 */
static void env_sensor_init_i2c(void)
{
    __HAL_RCC_I2C1_CLK_ENABLE();

    i2c_handle.Instance             = I2C1;
    i2c_handle.Init.ClockSpeed      = 100000;
    i2c_handle.Init.DutyCycle       = I2C_DUTYCYCLE_2;
    i2c_handle.Init.OwnAddress1     = 0;
    i2c_handle.Init.AddressingMode  = I2C_ADDRESSINGMODE_7BIT;
    i2c_handle.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
    i2c_handle.Init.OwnAddress2     = 0;
    i2c_handle.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
    i2c_handle.Init.NoStretchMode   = I2C_NOSTRETCH_DISABLE;

    HAL_I2C_Init(&i2c_handle);
}

/**
 * @brief   Initialisiert den BME280 und setzt Sensor-Einstellungen
 *
 * @details
 * Bindet die I2C Read/Write/Delay Callbacks in die bme280_dev Struktur ein,
 * initialisiert den Sensor und setzt Oversampling- sowie Filterparameter.
 *
 * @param   None
 * @return  None
 */
static void env_sensor_bme280_init(void)
{
    dev.intf_ptr = &i2c_handle;
    dev.intf     = BME280_I2C_INTF;
    dev.read     = env_sensor_i2c_read;
    dev.write    = env_sensor_i2c_write;
    dev.delay_us = env_sensor_delay_us;

    bme280_init(&dev);

    settings.osr_h  = BME280_OVERSAMPLING_1X;
    settings.osr_p  = BME280_OVERSAMPLING_16X;
    settings.osr_t  = BME280_OVERSAMPLING_2X;
    settings.filter = BME280_FILTER_COEFF_16;

    uint8_t settings_sel = (uint8_t)(BME280_SEL_OSR_PRESS |
                                     BME280_SEL_OSR_TEMP  |
                                     BME280_SEL_OSR_HUM   |
                                     BME280_SEL_FILTER);

    bme280_set_sensor_settings(settings_sel, &settings, &dev);
}

/**
 * @brief   I2C-Read Callback für die BME280 Library
 *
 * @param   reg_addr Registeradresse
 * @param   data     Zielpuffer
 * @param   len      Anzahl Bytes
 * @param   intf_ptr Zeiger auf das I2C-Handle
 *
 * @return  0 bei Erfolg, -1 bei Fehler
 */
static int8_t env_sensor_i2c_read(uint8_t reg_addr, uint8_t *data, uint32_t len, void *intf_ptr)
{
    I2C_HandleTypeDef *hi2c = (I2C_HandleTypeDef *)intf_ptr;

    if (HAL_I2C_Mem_Read(hi2c,
                        (uint16_t)(BME280_I2C_ADDR_SEC << 1),
                        reg_addr,
                        I2C_MEMADD_SIZE_8BIT,
                        data,
                        (uint16_t)len,
                        TIMEOUT) == HAL_OK)
    {
        return 0;
    }

    return -1;
}

/**
 * @brief   I2C-Write Callback für die BME280 Library
 *
 * @param   reg_addr Registeradresse
 * @param   data     Quelldaten
 * @param   len      Anzahl Bytes
 * @param   intf_ptr Zeiger auf das I2C-Handle
 *
 * @return  0 bei Erfolg, -1 bei Fehler
 */
static int8_t env_sensor_i2c_write(uint8_t reg_addr, const uint8_t *data, uint32_t len, void *intf_ptr)
{
    I2C_HandleTypeDef *hi2c = (I2C_HandleTypeDef *)intf_ptr;

    if (HAL_I2C_Mem_Write(hi2c,
                         (uint16_t)(BME280_I2C_ADDR_SEC << 1),
                         reg_addr,
                         I2C_MEMADD_SIZE_8BIT,
                         (uint8_t *)data,
                         (uint16_t)len,
                         TIMEOUT) == HAL_OK)
    {
        return 0;
    }

    return -1;
}

/**
 * @brief   Delay-Funktion für die BME280 Library
 *
 * @details
 * Die BME280 Library erwartet eine Delay-Funktion in Mikrosekunden.
 * Hier wird vereinfacht auf HAL_Delay (Millisekunden) abgebildet.
 *
 * @param   period   Zeitdauer in Mikrosekunden (laut Library)
 * @param   intf_ptr Unbenutzt (Kompatibilität zur Library)
 *
 * @return  None
 */
static void env_sensor_delay_us(uint32_t period, void *intf_ptr)
{
    (void)intf_ptr;
    HAL_Delay(period / 1000);
}
