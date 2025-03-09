/**
 * @file http_custom.c
 * @brief Custom HTTP client functions for ESP32.
 *
 * This file contains functions to handle HTTP client events and
 * send HTTP POST requests.
 *
 * @author Kamil Kośnik
 * @date   2025-03-07
 *
 * @note
 * - Ensure that the Telegraf broker URL is defined in this file or
 *   another header file included like "SETTINGS_PRIVATE.h".
 * 
 * @par License:
 * - This code is released under the MIT License.
 */

#include "esp_log.h"
#include "esp_http_client.h"

#include "http_custom.h"

esp_err_t client_event_post_handler(esp_http_client_event_handle_t evt)
{
    switch (evt->event_id)
    {
    case HTTP_EVENT_ON_DATA:
        printf("HTTP_EVENT_ON_DATA: %.*s\n", evt->data_len, (char *)evt->data);
        break;

    default:
        //printf("HTTP_EVENT_OTHER: %.*s\n", evt->data_len, (char *)evt->data);
        break;
    }
    return ESP_OK;
}

void http_post_data(char *post_data)
{
    esp_http_client_config_t config_post = {
        .url = TELEGRAF_BROKER_URL,
        .method = HTTP_METHOD_POST,
        .cert_pem = NULL,
        .event_handler = client_event_post_handler};
        
    esp_http_client_handle_t client = esp_http_client_init(&config_post);
 
    esp_http_client_set_post_field(client, post_data, strlen(post_data));
    esp_http_client_set_header(client, "Content-Type", "application/json");

    esp_http_client_perform(client);
    esp_http_client_cleanup(client);
}