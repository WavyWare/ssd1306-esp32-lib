#include "stdint.h"
#include "ssd1306.h"


void app_main() {    
    i2c_master_init();
    ssd1306_init();
    draw_string(0, 0, "Witaj w wielkim wyswietlaczu I2C! Oto progress bar:");
    progress_bar(1, 25, 5, 96, 50);
    draw_string(100, 24, "100%%");

    ssd1306_flush();
    
} 