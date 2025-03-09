/**
 * @file adc_custom.c
 * @brief Custom ADC functions for ESP32.
 *
 * This file contains functions to initialize the ADC, read voltage values,
 * and calculate external temperature from raw ADC values.
 *
 * @author Kamil Kośnik
 * @date   2025-03-07
 * 
 * @par License:
 * - This code is released under the MIT License.
 */

#include "math.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_system.h"
#include "esp_err.h"
#include "esp_adc/adc_oneshot.h"
#include "esp_adc/adc_cali.h"
#include "esp_adc/adc_cali_scheme.h"

#include "adc_custom.h"
#include "tasks_info.h"

static const char *TAG = "ADC";
static adc_oneshot_unit_handle_t adc_handle;
static adc_oneshot_chan_cfg_t adc_channel_config;

static float temperature_sensor_adc = 0.0f;

static void adc_init(void)
{
    adc_oneshot_unit_init_cfg_t adc_unit_config =
    {
        .unit_id = ADC_UNIT_1,
        .clk_src = ADC_RTC_CLK_SRC_DEFAULT,
        .ulp_mode = ADC_ULP_MODE_DISABLE,
    };
    esp_err_t err = adc_oneshot_new_unit(&adc_unit_config, &adc_handle);
    ESP_ERROR_CHECK(err);
    adc_channel_config.atten = ADC_ATTEN_DB_12,
    adc_channel_config.bitwidth = ADC_BITWIDTH_12,
    err = adc_oneshot_config_channel(adc_handle, ADC_CHANNEL_6, &adc_channel_config);
    ESP_ERROR_CHECK(err);
}

static esp_err_t adc_read(int* raw_ptr)
{
    esp_err_t err = adc_oneshot_read(adc_handle, ADC_CHANNEL_6, raw_ptr);
    return err;
}

static float adc_externalTemperatureCalc(uint16_t raw)
{
    uint16_t avg = raw;
    uint16_t ADC_RESOLUTION = pow(2.0, (double)adc_channel_config.bitwidth);
	// uint32_t sum = 0;
	// for (uint8_t var = 0; var < ADC_EXTERNAL_TEMPERATURE_BUFFER_SIZE; ++var)
	// {
	// 	sum += AdcCustomHandle.temperatureExternalRaw[var];
	// }
	// uint32_t avg = sum / ADC_EXTERNAL_TEMPERATURE_BUFFER_SIZE;
	// if(avg == 0){avg = 1;} //prevent dividing by 0

	/* Calculate temperature of thermistor in *C using adc measurement */
	float l = log((float)(avg*TEMPERATURE_EXTERNAL_UP_RESISTOR) / (float)(NTC_THERMISTOR_DEFAULT*(ADC_RESOLUTION-avg)));
	float step = 1/((float)(1/CELSIUS_25_TO_KELVIN) + (float)(1/(float)NTC_THERMISTOR_BETA) * l);
	return (step - CELSIUS_0_TO_KELVIN) + TEMPERATURE_EXTERNAL_OFFSET;
}

float adc_getTemperature(void)
{
    return temperature_sensor_adc;
}

void adc_task( void *pvParameters )
{
    //Initialize ADC in oneShot mode
    ESP_LOGI(TAG, "ADC  init");
    adc_init();

    int raw = 0;
    for( ;; )
    {
        ESP_ERROR_CHECK(adc_read(&raw));
        // ESP_LOGI(TAG, "ADC RAW read: %d", raw);
        temperature_sensor_adc = adc_externalTemperatureCalc((uint16_t)raw);
        ESP_LOGI(TAG, "ADC read: %.3f *C", temperature_sensor_adc);

        vTaskDelay(pdMS_TO_TICKS(taskDelay_ADC));
    }

    vTaskDelete( NULL );
}
