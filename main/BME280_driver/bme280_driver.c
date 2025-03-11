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

static void bme280_init(void)
{
    ESP_LOGI(TAG, "BME280 init started.");

    //ESP_ERROR_CHECK(nvs_flash_init());
    i2c_config_t i2c_cfg = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = BMX280_SDA_NUM,
        .scl_io_num = BMX280_SCL_NUM,
        .sda_pullup_en = false,
        .scl_pullup_en = false,

        .master = {
            .clk_speed = CONFIG_BMX280_I2C_CLK_SPEED_HZ
        }
    };

    ESP_ERROR_CHECK(i2c_param_config(I2C_NUM_0, &i2c_cfg));
    ESP_ERROR_CHECK(i2c_driver_install(I2C_NUM_0, I2C_MODE_MASTER, 0, 0, 0));

    bmx280_t* bmx280 = bmx280_create(I2C_NUM_0);

    if (!bmx280) { 
        ESP_LOGE(TAG, "Could not create bmx280 driver.");
        return;
    }

    if(bmx280_init(bmx280) != ESP_OK)
    {
        bmx280_close(bmx280);;
        ESP_LOGE(TAG, "Could not initialize/detect bmx280 driver.");
        return;
    }
    bmx280_config_t bmx_cfg = BMX280_DEFAULT_CONFIG;
    ESP_ERROR_CHECK(bmx280_configure(bmx280, &bmx_cfg));
    ESP_ERROR_CHECK(bmx280_setMode(bmx280, BMX280_MODE_CYCLE));
}

void bme280_task( void *pvParameters )
{
    bme280_init();
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
                vTaskDelay(pdMS_TO_TICKS(1));
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

