#pragma once
#include "stdint.h"
#include <stdbool.h>
#include <stdint.h>

struct ssd1306_t {
    uint16_t height;
    uint16_t width;
    uint8_t * framebuffer;
    uint8_t address;
};
typedef struct ssd1306_t ssd1306_t;

void i2c_master_init(void);
void ssd1306_init();
void ssd1306_flush();
void ssd1306_set_pixel(int x, int y, bool color);
void draw_char(int x, int y, char c);
void ssd1306_clear();
void draw_string(int x, int y, const char *str);
void progress_bar(int x, int y, int height, int width, uint8_t percent);
