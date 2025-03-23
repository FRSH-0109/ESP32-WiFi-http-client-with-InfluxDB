/**
 * @file main.c
 * @brief Main file for ESP32 SCIR project.
 *
 * This project aims to get data form various sensors and send it to a database
 * through http POST requests, using external telegraf broker and InfluxDB.
 * Wireless communication is handled by the ESP32 WiFi module.
 *
 * @author Kamil Kośnik
 * @date   2025-03-07
 *
 * @note
 * Ensure that the WiFi SSID, password and other paramerersa are defined in
 * your header file included like "SETTINGS_PRIVATE.h".
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

#include "esp_system.h"

#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_netif.h"

#include "driver/i2c_types.h"
#include "driver/i2c_master.h"

#include "tasks_info.h"
#include "cJSON.h"
#include "SETTINGS_PRIVATE.h"
#include "wifi_custom.h"
#include "http_custom.h"
#include "adc_custom.h"
#include "BME280_driver/bme280_driver.h"
#include "SHTC3_driver/shtc3_driver.h"

static const char *TAG = "Main";

float sensor1Temp, sensor2Temp, sensor3Temp;
float sensor1Hum, sensor2Hum;
float sensor1Press;

void adc_task( void *pvParameters );
void bme280_task( void *pvParameters );

#define I2C_SDA_GPIO GPIO_NUM_21
#define I2C_SCL_GPIO GPIO_NUM_22

i2c_master_bus_handle_t bus_handle;

void i2c_bus_init(uint8_t sda_io, uint8_t scl_io)
{
    i2c_master_bus_config_t i2c_bus_config = {
        .i2c_port = -1,
        .sda_io_num = sda_io,
        .scl_io_num = scl_io,
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .glitch_ignore_cnt = 7,
        .flags.enable_internal_pullup = true,
    };

    ESP_ERROR_CHECK(i2c_new_master_bus(&i2c_bus_config, &bus_handle));
    ESP_LOGI(TAG, "I2C master bus created");
}

void app_main(void)
{
    //Initialize NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      ESP_ERROR_CHECK(nvs_flash_erase());
      ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    //Initialize WiFi in STA
    ESP_LOGI(TAG, "ESP_WIFI_MODE_STA");
    wifi_init();

    i2c_bus_init(I2C_SDA_GPIO, I2C_SCL_GPIO);

    // // Create a task for adc temperature reading
    TaskHandle_t* adc_task_handle = NULL;
    xTaskCreate(adc_task, "Adc Task", TASK_SIZE_ADC, NULL, TASK_PRIO_ADC, adc_task_handle);

    // Create a task for BME280 sensor handling
    TaskHandle_t* bme280_task_handle = NULL;
    xTaskCreate(bme280_task, "BME280 Task", TASK_SIZE_BME280, (void *)&bus_handle, TASK_PRIO_BME280, bme280_task_handle);

    // Create a task for SHTC3 sensor handling
    TaskHandle_t* shtc3_task_handle = NULL;
    xTaskCreate(shtc3_task, "SHTC3 Task", TASK_SIZE_SHTC3, (void *)&bus_handle, TASK_PRIO_SHTC3, shtc3_task_handle);

    while (true)
    {
        vTaskDelay(pdMS_TO_TICKS(taskDelay_Main));
        
        sensor1Temp = adc_getTemperature();
        sensor2Temp = bme280_getTemperature();
        sensor3Temp = shtc3_getTemperature();
        ESP_LOGI(TAG, "ADC %.3f *C, BME280 %.3f *C, SHTC3 %.3f *C", sensor1Temp, sensor2Temp, sensor3Temp);

        sensor1Hum = bme280_getHumidity();
        sensor2Hum = shtc3_getHumidity();

        sensor1Press = bme280_getPressure();

        cJSON *json = cJSON_CreateObject();  
        cJSON *analog_NTC = cJSON_AddObjectToObject(json, "Analog NTC");  
        cJSON *BME280 = cJSON_AddObjectToObject(json, "BME280");  
        cJSON *SHTC3 = cJSON_AddObjectToObject(json, "SHTC3");  
        
        cJSON_AddNumberToObject(analog_NTC, "Temperature", sensor1Temp);
        cJSON_AddNumberToObject(BME280, "Temperature", sensor2Temp);
        cJSON_AddNumberToObject(SHTC3, "Temperature", sensor3Temp);

        cJSON_AddNumberToObject(BME280, "Humidity", sensor1Hum);
        cJSON_AddNumberToObject(SHTC3, "Humidity", sensor2Hum);

        cJSON_AddNumberToObject(BME280, "Pressure", sensor1Press);

        char *post_data = cJSON_Print(json); 
        http_post_data(post_data);
        ESP_LOGI(TAG, "HTTP POST data sent");
        cJSON_Delete(json);
        free(post_data);
    }
}