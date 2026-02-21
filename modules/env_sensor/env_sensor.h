/**
 ******************************************************************************
 * @file        env_sensor.h
 * @author      Mahmoud Mahmoud, Judy Abou Rmeh
 * @version     V1.0
 * @date        14.03.2019
 * @brief       Schnittstelle für den Umweltsensor
 *
 * @details
 * Dieses Modul stellt Funktionen zur Initialisierung und zum Auslesen
 * eines Umweltsensors (z. B. BME280) bereit. Es ermöglicht das Erfassen
 * von Temperatur, Luftdruck und Luftfeuchtigkeit.
 *
 * Verwendete Module:
 *  - bme280
 *
 * Verwendete Peripherie:
 *  - Umweltsensor (BME280)
 *
 ******************************************************************************
 */

#ifndef ENV_SENSOR_ENV_SENSOR_H_
#define ENV_SENSOR_ENV_SENSOR_H_

#include <bme280/bme280.h>
#include "stm32f4xx.h"

/* Public Preprocessor defines */

/**
 * @brief Timeout-Wert für Sensor-Kommunikation
 *
 * @details
 * Gibt die maximale Wartezeit für die Kommunikation mit dem
 * Umweltsensor in Millisekunden an.
 */
#define TIMEOUT 100

/* Public functions (prototypes) */

/**
 * @brief   Initialisiert den Umweltsensor
 *
 * @details
 * Initialisiert die notwendige Hardware und bereitet den
 * Umweltsensor für Messungen vor.
 *
 * @param   None
 * @return  None
 */
void env_sensor_init(void);

/**
 * @brief   Liest Messwerte des Umweltsensors aus
 *
 * @details
 * Liest Temperatur, Luftdruck und Luftfeuchtigkeit vom Sensor
 * und speichert die Werte in den übergebenen Variablen.
 *
 * @param   temperature Zeiger auf die Temperatur in Grad Celsius
 * @param   pressure    Zeiger auf den Luftdruck in hPa
 * @param   humidity    Zeiger auf die relative Luftfeuchtigkeit in Prozent
 *
 * @return  None
 */
void env_sensor_read_data(float *temperature,
                          float *pressure,
                          float *humidity);

#endif /* ENV_SENSOR_ENV_SENSOR_H_ */
