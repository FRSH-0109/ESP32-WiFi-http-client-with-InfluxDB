idf_component_register(SRCS "adc_custom.c" "http_custom.c" "wifi_custom.c" "cJSON.c" "main.c" "BME280_driver/bmx280.c" "BME280_driver/bme280_driver.c" "SHTC3_driver/shtc3.c" "SHTC3_driver/shtc3_driver.c"
                    PRIV_REQUIRES esp_wifi nvs_flash esp_event esp_netif esp_http_client esp-tls esp_adc driver
                    INCLUDE_DIRS "." "BME280_driver" "SHTC3_driver")
