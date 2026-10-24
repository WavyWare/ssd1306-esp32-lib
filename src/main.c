#include "hal/i2c_types.h"
#include "soc/gpio_num.h"
#include "ssd1306.h"
#include "i2c_bus.h"
#include <stdlib.h>

void app_main() {    
    i2c_master_init(GPIO_NUM_21, GPIO_NUM_22, 400000, I2C_NUM_0);
    uint8_t * framebuffer = calloc(ssd1306_get_framebuffer_size(128, 32), 1);
    ssd1306_t display_context = {
        .height = 32,
        .width = 128,
        .framebuffer = framebuffer,
        .address = 0x3C,
        .i2c_port = I2C_NUM_0
    };
    ssd1306_default_init(&display_context);
    draw_string(&display_context, 0, 0, "Witaj w wielkim wyswietlaczu I2C! Oto progress bar:");
    progress_bar(&display_context, 1, 27, 5, 96, 50);
    draw_string(&display_context, 100, 24, "100%%");
    ssd1306_flush(&display_context);    
} 