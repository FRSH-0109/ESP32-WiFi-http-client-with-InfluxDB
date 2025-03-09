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

#include "cJSON.h"
#include "SETTINGS_PRIVATE.h"
#include "wifi_custom.h"
#include "http_custom.h"
#include "adc_custom.h"

static const char *TAG = "Main";

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

    //Initialize ADC in oneShot mode
    ESP_LOGI(TAG, "ADC  init");
    adc_init();

    uint8_t i = 0;
    int raw = 0;
    while (true)
    {
        vTaskDelay(pdMS_TO_TICKS(2000));
        ESP_ERROR_CHECK(adc_read(&raw));
        ESP_LOGI(TAG, "ADC RAW read: %d", raw);
        float temperature = adc_externalTemperatureCalc((uint16_t)raw);
        ESP_LOGI(TAG, "ADC read: %.3f", temperature);

        cJSON *json = cJSON_CreateObject(); 
        cJSON_AddNumberToObject(json, "a", i); 
        cJSON_AddNumberToObject(json, "NTC", temperature);
        char *post_data = cJSON_Print(json); 

        http_post_data(post_data);
        cJSON_Delete(json);

        ++i;
    }
}