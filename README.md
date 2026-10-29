# SSD1306 ESP32 Library

A lightweight, driver for most popular oled display. Written in C99

## All functions
1. Writing text on screen with simple wrapping
2. All functions that have to, return esp errors
3. Static memory allocation

## Requirements
1. Board that works with ESP environment (like ESP32)
2. Platformio or other C99 compiler
3. I2C OLED display based on SSD1306

## Quick start

At first initialize i2c using
```c
esp_err_t i2c_master_init(gpio_num_t sda_pin, gpio_num_t scl_pin, uint32_t clk_speed, int i2c_port);
```
method.

Then, you can create framebuffer array that will from now on contain all bytes shown on display. It's recommended to add validation, if memory isn't full.
```c
uint8_t * framebuffer = calloc(ssd1306_get_framebuffer_size(SSD1306_WIDTH, SSD1306_HEIGHT), 1);

if (framebuffer == NULL) {
    return;
}
```

Next step is to define display context, which will be used in most of next display functions

```c
ssd1306_t display_context = {
    .height = 128,
    .width = 64,
    .framebuffer = framebuffer,
    .address = 0x3C,
    .i2c_port = I2C_NUM_0
};
```

Last step is to push default initialization to display with command:
```c
ssd1306_default_init(&display_context);
```

And now you can work with display using other commands.

## Commands

### Init, life cycle and buffer

#### ssd1306_get_framebuffer_size
```c
size_t ssd1306_get_framebuffer_size(uint8_t width, uint8_t height);
```
ssd1306_get_framebuffer_size allows you to get size of framebuffer for initialization with `calloc()`.

#### ssd1306_default_init 
```c
esp_err_t ssd1306_default_init(ssd1306_t *context);
```
ssd1306_default_init sends default command list to display.

#### ssd1306_flush
```c
esp_err_t ssd1306_flush(ssd1306_t *context);
```
ssd1306_flush sends values from framebuffer to display.

#### ssd1306_clear
```c
esp_err_t ssd1306_clear(ssd1306_t *context);
```
ssd1306_clear clears values from framebuffer.


