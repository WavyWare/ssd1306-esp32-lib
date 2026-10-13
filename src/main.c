#include "hal/i2c_types.h"
#include "soc/gpio_num.h"
#include "ssd1306.h"
#include "i2c_bus.h"

void app_main() {    
    i2c_master_init(GPIO_NUM_21, GPIO_NUM_22, 4000000, I2C_NUM_0);
    ssd1306_init();
    draw_string(0, 0, "Witaj w wielkim wyswietlaczu I2C! Oto progress bar:");
    progress_bar(1, 25, 5, 96, 50);
    draw_string(100, 24, "100%%");

    ssd1306_flush();
    
} 