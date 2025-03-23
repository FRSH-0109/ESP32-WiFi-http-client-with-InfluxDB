#include "SHTC3_driver/shtc3_driver.h"

i2c_master_dev_handle_t shtc3_handle;

static const char *TAG = "SHTC3";

float temperature, humidity;
esp_err_t err = ESP_OK;
shtc3_register_rw_t reg = SHTC3_REG_T_CSD_NM;

static void shtc3_init(i2c_master_bus_handle_t* i2c_handle)
{
    shtc3_handle = shtc3_device_create(*i2c_handle, SHTC3_I2C_ADDR, CONFIG_SHTC3_I2C_CLK_SPEED_HZ);
    ESP_LOGI(TAG, "Sensor initialization success");
}

void shtc3_task( void *pvParameters )
{
    i2c_master_bus_handle_t* i2c_handle = (i2c_master_bus_handle_t*)pvParameters;
    shtc3_init(i2c_handle);

    // Probe the sensor to check if it is connected to the bus with a 10ms timeout
    esp_err_t err = i2c_master_probe(*i2c_handle, SHTC3_I2C_ADDR, 200);

    if(err != ESP_OK)
    {
        ESP_LOGE(TAG, "SHTC3 sensor not found");
        shtc3_device_delete(shtc3_handle);

        while (true) //lock task
        {
            vTaskDelay(pdMS_TO_TICKS(taskDelay_SHTC3));
        }
    }

    ESP_LOGI(TAG, "SHTC3 sensor found");
    uint8_t sensor_id[2];
    err = shtc3_get_id(shtc3_handle, sensor_id);
    ESP_LOGI(TAG, "Sensor ID: 0x%02x%02x", sensor_id[0], sensor_id[1]);

    if(err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to read SHTC3 ID");
        while (true) //lock task
        {
            vTaskDelay(pdMS_TO_TICKS(taskDelay_SHTC3));
        }
    }
    ESP_LOGI(TAG, "SHTC3 ID read successfully");

    static bool readTodo = false; 
    while (true)
    {   
        if(!readTodo)
        {
            err = shtc3_start_measure(shtc3_handle, reg);
            if(err != ESP_OK) {
                ESP_LOGE(TAG, "WRITE: Failed to start measurement on SHTC3 sensor");
            } else {
                // ESP_LOGI(TAG, "WRITE: Start measurement on SHTC3 sensor");
                readTodo = true;
            }
        } 
        else 
        {
            err = shtc3_read_measure(shtc3_handle, reg, &temperature, &humidity);
            if(err != ESP_OK) {
                ESP_LOGE(TAG, "READ: Failed to read data from SHTC3 sensor");
            } else {
                // ESP_LOGI(TAG, "READ: Temperature: %.2f C, Humidity: %.2f %%", temperature, humidity);
                readTodo = false;
            }
        }        
        vTaskDelay(pdMS_TO_TICKS(taskDelay_SHTC3));
    }
}

float shtc3_getTemperature()
{
    return temperature;
}

float shtc3_getHumidity()
{
    return humidity;
}

