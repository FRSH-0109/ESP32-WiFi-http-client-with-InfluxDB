#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_system.h"
#include "esp_err.h"

#include "tasks_info.h"
#include "driver/i2c.h"
#include "driver/i2c_types.h"
#include "SHTC3_driver/shtc3.h"

#define CONFIG_SHTC3_I2C_CLK_SPEED_HZ 100000

void shtc3_task ( void *pvParameters);
float shtc3_getTemperature();
float shtc3_getHumidity();