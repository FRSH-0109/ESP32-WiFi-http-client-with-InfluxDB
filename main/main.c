/* WiFi station Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
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

    ESP_LOGI(TAG, "ESP_WIFI_MODE_STA");
    wifi_init_sta();

    uint8_t i = 0;
    while (true)
    {
        vTaskDelay(pdMS_TO_TICKS(2000));
        
        cJSON *json = cJSON_CreateObject(); 
        cJSON_AddNumberToObject(json, "a", i); 
        cJSON_AddNumberToObject(json, "age", 30);
        char *post_data = cJSON_Print(json); 

        http_post_data(post_data);
        cJSON_Delete(json);

        ++i;
    }
}