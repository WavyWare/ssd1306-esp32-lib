#pragma once
#include "esp_err.h"
#include "hal/i2c_types.h"
#include "stdint.h"
#include <stdbool.h>

struct ssd1306_t {
    uint16_t height;
    uint16_t width;
    uint8_t * framebuffer;
    uint8_t address;
    i2c_port_t i2c_port;
};
typedef struct ssd1306_t ssd1306_t;

size_t ssd1306_get_framebuffer_size(uint8_t width, uint8_t height);
esp_err_t ssd1306_write_cmds(ssd1306_t *context, size_t n, const uint8_t * cmds);
esp_err_t ssd1306_default_init(ssd1306_t *context);
esp_err_t ssd1306_flush(ssd1306_t *context);
void ssd1306_set_pixel(ssd1306_t * context, int x, int y, bool color);
esp_err_t ssd1306_clear(ssd1306_t *context);
esp_err_t draw_string(ssd1306_t *context, int x, int y, const char *str);
esp_err_t progress_bar(ssd1306_t *context, int x, int y, int height, int width, uint8_t percent);
