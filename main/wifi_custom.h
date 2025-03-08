
#include "SETTINGS_PRIVATE.h"

#ifndef WIFI_CUSTOM_H
#define WIFI_CUSTOM_H

/*
* Defines for
*/
#ifndef MY_ESP_WIFI_SSID
    #define MY_ESP_WIFI_SSID "MY_WIFI_SSID"
#endif

#ifndef MY_ESP_WIFI_PASS
    #define MY_ESP_WIFI_PASS "MY_WIFI_PASSOWRD"
#endif

#define EXAMPLE_ESP_MAXIMUM_RETRY  10
#define ESP_WIFI_SAE_MODE WPA3_SAE_PWE_BOTH
#define ESP_WIFI_SCAN_AUTH_MODE_THRESHOLD WIFI_AUTH_OPEN
#define EXAMPLE_H2E_IDENTIFIER CONFIG_ESP_WIFI_PW_ID


/* The event group allows multiple bits for each event, but we only care about two events:
 * - we are connected to the AP with an IP
 * - we failed to connect after the maximum amount of retries */
#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT      BIT1

void event_handler(void* arg, esp_event_base_t event_base,
    int32_t event_id, void* event_data);
void wifi_init_sta(void);

#endif