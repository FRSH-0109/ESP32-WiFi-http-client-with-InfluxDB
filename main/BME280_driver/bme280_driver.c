/**
 * @file bme280_driver.c
 * @brief Main file for ESP32 SCIR project.
 *
 * This project aims to controll BME280 sensor.
 *
 * @author Kamil Kośnik
 * @date   2025-03-07
 *
 * @par License:
 * - This code is released under the MIT License.
 */

#include "BME280_driver/bme280_driver.h"

static const char *TAG = "BME280";
bmx280_t* bmx280 = NULL;           // BME280 driver structure pointer
float temp = 0, pres = 0, hum = 0; // Measured: Temperature, Pressure, Humidity

i2c_master_dev_handle_t bme280_handle;

static esp_err_t bme280_init(i2c_master_bus_handle_t* i2c_handle)
{
    bmx280 = bmx280_create_master(*i2c_handle);
    if (!bmx280) { 
        ESP_LOGE(TAG, "Could not create bmx280 driver.");
        return ESP_FAIL;
    }

    if(bmx280_init(bmx280) != ESP_OK)
    {
        bmx280_close(bmx280);;
        ESP_LOGE(TAG, "Could not initialize/detect bmx280 driver.");
        return ESP_FAIL;
    }
    bmx280_config_t bmx_cfg = BMX280_DEFAULT_CONFIG;
    ESP_ERROR_CHECK(bmx280_configure(bmx280, &bmx_cfg));
    ESP_ERROR_CHECK(bmx280_setMode(bmx280, BMX280_MODE_CYCLE));
    return ESP_OK;
}

void bme280_task( void *pvParameters )
{
    i2c_master_bus_handle_t* i2c_handle = (i2c_master_bus_handle_t*)pvParameters;
    esp_err_t err = bme280_init(i2c_handle);
    if(err != ESP_OK)
    {
        ESP_LOGE(TAG, "BME280 task error with initialization.");
        while (true)    // infinite loop for error handling
        {
            vTaskDelay(pdMS_TO_TICKS(taskDelay_BME280));
        }
    }
    ESP_LOGI(TAG, "BME280 task started.");
    while (true)
    {   
        if(bmx280 == NULL)
        {
            ESP_LOGI(TAG, "BME280 task error with structure.");
            while (true)    // infinite loop for error handling
            {
                vTaskDelay(pdMS_TO_TICKS(taskDelay_BME280));
            }
        }
        else
        {
            do {
                vTaskDelay(pdMS_TO_TICKS(taskDelay_BME280));
            } while(bmx280_isSampling(bmx280));
    
            ESP_ERROR_CHECK(bmx280_readoutFloat(bmx280, &temp, &pres, &hum));
            
            vTaskDelay(pdMS_TO_TICKS(taskDelay_BME280));
        }
    }
}

float bme280_getTemperature()
{
    return temp;
}

float bme280_getPressure()
{
    return pres;
}

float bme280_getHumidity()
{
    return hum;
}

