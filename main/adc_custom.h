/**
 * @file adc_custom.h
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

#ifndef ADC_CUSTOM_H
#define ADC_CUSTOM_H

/* Extenal Temperature defines */
#define TEMPERATURE_EXTERNAL_UP_RESISTOR	10000
#define TEMPERATURE_EXTERNAL_OFFSET	0.0f
/* Battery Temperature defines */

/* NTC  Thermistor 103AT-2 NTC defines */
#define NTC_THERMISTOR_DEFAULT	10000	//at 25*C
#define NTC_THERMISTOR_BETA		3435.0
#define CELSIUS_0_TO_KELVIN		273.15f
#define CELSIUS_25_TO_KELVIN	298.15f
/* NTC  Thermistor 103AT-2 NTC defines */

float adc_getTemperature(void);

#endif