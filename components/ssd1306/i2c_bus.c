#include "esp_err.h"
#include "soc/gpio_num.h"
#include <driver/i2c.h>
#include <stdint.h>

esp_err_t i2c_master_init(gpio_num_t sda_pin, gpio_num_t scl_pin, uint32_t clk_speed, int i2c_port) {
    esp_err_t status;
    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = sda_pin,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_io_num = scl_pin,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = clk_speed,
    };
    status = i2c_param_config(i2c_port, &conf);
    if (status != ESP_OK) return status;
    status = i2c_driver_install(i2c_port, conf.mode, 0, 0, 0);
    if (status != ESP_OK) return status;
    return ESP_OK;
}