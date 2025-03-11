/**
 * @file bme280_driver.h
 * @brief Main file for ESP32 SCIR project.
 *
 * This project aims to control BME280 sensor.
 *
 * @author Kamil Kośnik
 * @date   2025-03-07
 *
 * @par License:
 * - This code is released under the MIT License.
 */


#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_system.h"
#include "esp_err.h"

#include "tasks_info.h"
#include "driver/i2c.h"
#include "driver/i2c_types.h"
#include "bmx280.h"

#define BMX280_SDA_NUM GPIO_NUM_13
#define BMX280_SCL_NUM GPIO_NUM_14
#define CONFIG_BMX280_I2C_CLK_SPEED_HZ 100000

void bme380_task( void *pvParameters );
float bme280_getTemperature();
float bme280_getPressure();
float bme280_getHumidity();