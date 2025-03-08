idf_component_register(SRCS "http_custom.c" "wifi_custom.c" "cJSON.c" "main.c"
                    PRIV_REQUIRES esp_wifi nvs_flash esp_event esp_netif esp_http_client esp-tls
                    INCLUDE_DIRS ".")
