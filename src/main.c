#include "hal/i2c_types.h"
#include "soc/gpio_num.h"
#include "ssd1306.h"
#include "i2c_bus.h"

void app_main() {    
    i2c_master_init(GPIO_NUM_21, GPIO_NUM_22, 4000000, I2C_NUM_0);
    uint8_t framebuffer[ssd1306_get_framebuffer_size(32, 128)];
    ssd1306_t display_context = {
        .height = 32,
        .width = 128,
        .framebuffer = framebuffer,
        .address = 0x3C,
        .i2c_port = I2C_NUM_0
    };
    draw_string(0, 0, "Witaj w wielkim wyswietlaczu I2C! Oto progress bar:");
    progress_bar(1, 27, 5, 96, 50);
    draw_string(100, 24, "100%%");

    ssd1306_flush();
    
} 