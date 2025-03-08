#include "esp_log.h"
#include "esp_http_client.h"
#include "SETTINGS_PRIVATE.h"

#ifndef HTTP_CUSTOM_H
#define HTTP_CUSTOM_H

#ifndef TELEGRAF_BROKER_URL
    #define TELEGRAF_BROKER_URL "http://192.168.X.X:XXXX/telegraf"
#endif

esp_err_t client_event_post_handler(esp_http_client_event_handle_t evt);
void http_post_data();

#endif