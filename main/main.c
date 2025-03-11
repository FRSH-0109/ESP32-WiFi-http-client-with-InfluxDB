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

#include "tasks_info.h"
#include "cJSON.h"
#include "SETTINGS_PRIVATE.h"
#include "wifi_custom.h"
#include "http_custom.h"
#include "adc_custom.h"
#include "bme280_driver.h"

static const char *TAG = "Main";

float sensor1, sensor2, sensor3;

void adc_task( void *pvParameters );
void bme280_task( void *pvParameters );

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

    // Create a task for adc temperature reading
    TaskHandle_t* adc_task_handle = NULL;
    xTaskCreate(adc_task, "Adc Task", TASK_SIZE_ADC, NULL, TASK_PRIO_ADC, adc_task_handle);

    // Create a task for BME280 sensor handling
    TaskHandle_t* bme280_task_handle = NULL;
    xTaskCreate(bme280_task, "BME280 Task", TASK_SIZE_ADC, NULL, TASK_PRIO_ADC, bme280_task_handle);

    while (true)
    {
        vTaskDelay(pdMS_TO_TICKS(taskDelay_Main));
        
        sensor1 = adc_getTemperature();
        sensor2 = bme280_getTemperature();
        sensor3 = adc_getTemperature();

        ESP_LOGI(TAG, "ADC %.3f *C, BME280 %.3f *C, Ten trzeci %.3f *C", sensor1, sensor2, sensor3);

        cJSON *json = cJSON_CreateObject();  
        cJSON_AddNumberToObject(json, "Analog NTC", sensor1);
        cJSON_AddNumberToObject(json, "BME280", sensor2);
        cJSON_AddNumberToObject(json, "Fermion", sensor3);
        char *post_data = cJSON_Print(json); 

        http_post_data(post_data);
        ESP_LOGI(TAG, "HTTP POST data sent");
        cJSON_Delete(json);
    }
}