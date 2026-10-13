#pragma once
#include "esp_err.h"
#include "soc/gpio_num.h"
#include <stdint.h>

esp_err_t i2c_master_init(gpio_num_t sda_pin, gpio_num_t scl_pin, uint32_t clk_speed, int i2c_port);