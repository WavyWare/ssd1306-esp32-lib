#include "ssd1306.h"
#include "esp_err.h"
#include <driver/i2c.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <sys/types.h>

#define FONT5X7_FIRST_ASCII 32
#define FONT5X7_LAST_ASCII  126
#define FONT5X7_CHAR_COUNT  95
#define FONT5X7_WIDTH       5
#define FONT5X7_HEIGHT      7

static const uint8_t font5x7[95][5] = {
    /* 32 ' ' */ { 0x00, 0x00, 0x00, 0x00, 0x00 },
    /* 33 '!' */ { 0x00, 0x00, 0x5F, 0x00, 0x00 },
    /* 34 '"' */ { 0x00, 0x07, 0x00, 0x07, 0x00 },
    /* 35 '#' */ { 0x14, 0x7F, 0x14, 0x7F, 0x14 },
    /* 36 '$' */ { 0x24, 0x2A, 0x7F, 0x2A, 0x12 },
    /* 37 '%' */ { 0x23, 0x13, 0x08, 0x64, 0x62 },
    /* 3u8 '&' */ { 0x36, 0x49, 0x55, 0x22, 0x50 },
    /* 39 ''' */ { 0x00, 0x05, 0x03, 0x00, 0x00 },
    /* 40 '(' */ { 0x00, 0x1C, 0x22, 0x41, 0x00 },
    /* 41 ')' */ { 0x00, 0x41, 0x22, 0x1C, 0x00 },
    /* 42 '*' */ { 0x14, 0x08, 0x3E, 0x08, 0x14 },
    /* 43 '+' */ { 0x08, 0x08, 0x3E, 0x08, 0x08 },
    /* 44 ',' */ { 0x00, 0x50, 0x30, 0x00, 0x00 },
    /* 45 '-' */ { 0x08, 0x08, 0x08, 0x08, 0x08 },
    /* 46 '.' */ { 0x00, 0x60, 0x60, 0x00, 0x00 },
    /* 47 '/' */ { 0x20, 0x10, 0x08, 0x04, 0x02 },
    /* 48 '0' */ { 0x3E, 0x51, 0x49, 0x45, 0x3E },
    /* 49 '1' */ { 0x00, 0x42, 0x7F, 0x40, 0x00 },
    /* 50 '2' */ { 0x42, 0x61, 0x51, 0x49, 0x46 },
    /* 51 '3' */ { 0x21, 0x41, 0x45, 0x4B, 0x31 },
    /* 52 '4' */ { 0x18, 0x14, 0x12, 0x7F, 0x10 },
    /* 53 '5' */ { 0x27, 0x45, 0x45, 0x45, 0x39 },
    /* 54 '6' */ { 0x3C, 0x4A, 0x49, 0x49, 0x30 },
    /* 55 '7' */ { 0x01, 0x71, 0x09, 0x05, 0x03 },
    /* 56 '8' */ { 0x36, 0x49, 0x49, 0x49, 0x36 },
    /* 57 '9' */ { 0x06, 0x49, 0x49, 0x29, 0x1E },
    /* 58 ':' */ { 0x00, 0x36, 0x36, 0x00, 0x00 },
    /* 59 ';' */ { 0x00, 0x56, 0x36, 0x00, 0x00 },
    /* 60 '<' */ { 0x08, 0x14, 0x22, 0x41, 0x00 },
    /* 61 '=' */ { 0x14, 0x14, 0x14, 0x14, 0x14 },
    /* 62 '>' */ { 0x00, 0x41, 0x22, 0x14, 0x08 },
    /* 63 '?' */ { 0x02, 0x01, 0x51, 0x09, 0x06 },
    /* 64 '@' */ { 0x32, 0x49, 0x79, 0x41, 0x3E },
    /* 65 'A' */ { 0x7E, 0x11, 0x11, 0x11, 0x7E },
    /* 66 'B' */ { 0x7F, 0x49, 0x49, 0x49, 0x36 },
    /* 67 'C' */ { 0x3E, 0x41, 0x41, 0x41, 0x22 },
    /* 68 'D' */ { 0x7F, 0x41, 0x41, 0x22, 0x1C },
    /* 69 'E' */ { 0x7F, 0x49, 0x49, 0x49, 0x41 },
    /* 70 'F' */ { 0x7F, 0x09, 0x09, 0x09, 0x01 },
    /* 71 'G' */ { 0x3E, 0x41, 0x49, 0x49, 0x7A },
    /* 72 'H' */ { 0x7F, 0x08, 0x08, 0x08, 0x7F },
    /* 73 'I' */ { 0x00, 0x41, 0x7F, 0x41, 0x00 },
    /* 74 'J' */ { 0x20, 0x40, 0x41, 0x3F, 0x01 },
    /* 75 'K' */ { 0x7F, 0x08, 0x14, 0x22, 0x41 },
    /* 76 'L' */ { 0x7F, 0x40, 0x40, 0x40, 0x40 },
    /* 77 'M' */ { 0x7F, 0x02, 0x0C, 0x02, 0x7F },
    /* 78 'N' */ { 0x7F, 0x04, 0x08, 0x10, 0x7F },
    /* 79 'O' */ { 0x3E, 0x41, 0x41, 0x41, 0x3E },
    /* 80 'P' */ { 0x7F, 0x09, 0x09, 0x09, 0x06 },
    /* 81 'Q' */ { 0x3E, 0x41, 0x51, 0x21, 0x5E },
    /* 82 'R' */ { 0x7F, 0x09, 0x19, 0x29, 0x46 },
    /* 83 'S' */ { 0x46, 0x49, 0x49, 0x49, 0x31 },
    /* 84 'T' */ { 0x01, 0x01, 0x7F, 0x01, 0x01 },
    /* 85 'U' */ { 0x3F, 0x40, 0x40, 0x40, 0x3F },
    /* 86 'V' */ { 0x1F, 0x20, 0x40, 0x20, 0x1F },
    /* 87 'W' */ { 0x3F, 0x40, 0x38, 0x40, 0x3F },
    /* 88 'X' */ { 0x63, 0x14, 0x08, 0x14, 0x63 },
    /* 89 'Y' */ { 0x07, 0x08, 0x70, 0x08, 0x07 },
    /* 90 'Z' */ { 0x61, 0x51, 0x49, 0x45, 0x43 },
    /* 91 '[' */ { 0x00, 0x7F, 0x41, 0x41, 0x00 },
    /* 92 '\' */ { 0x02, 0x04, 0x08, 0x10, 0x20 },
    /* 93 ']' */ { 0x00, 0x41, 0x41, 0x7F, 0x00 },
    /* 94 '^' */ { 0x04, 0x02, 0x01, 0x02, 0x04 },
    /* 95 '_' */ { 0x40, 0x40, 0x40, 0x40, 0x40 },
    /* 96 '`' */ { 0x00, 0x01, 0x02, 0x04, 0x00 },
    /* 97 'a' */ { 0x20, 0x54, 0x54, 0x54, 0x78 },
    /* 98 'b' */ { 0x7F, 0x48, 0x44, 0x44, 0x38 },
    /* 99 'c' */ { 0x38, 0x44, 0x44, 0x44, 0x20 },
    /* 100 'd' */ { 0x38, 0x44, 0x44, 0x48, 0x7F },
    /* 101 'e' */ { 0x38, 0x54, 0x54, 0x54, 0x18 },
    /* 102 'f' */ { 0x08, 0x7E, 0x09, 0x01, 0x02 },
    /* 103 'g' */ { 0x0C, 0x52, 0x52, 0x52, 0x3E },
    /* 104 'h' */ { 0x7F, 0x08, 0x04, 0x04, 0x78 },
    /* 105 'i' */ { 0x00, 0x44, 0x7D, 0x40, 0x00 },
    /* 106 'j' */ { 0x20, 0x40, 0x44, 0x3D, 0x00 },
    /* 107 'k' */ { 0x7F, 0x10, 0x28, 0x44, 0x00 },
    /* 108 'l' */ { 0x00, 0x41, 0x7F, 0x40, 0x00 },
    /* 109 'm' */ { 0x7C, 0x04, 0x18, 0x04, 0x78 },
    /* 110 'n' */ { 0x7C, 0x08, 0x04, 0x04, 0x78 },
    /* 111 'o' */ { 0x38, 0x44, 0x44, 0x44, 0x38 },
    /* 112 'p' */ { 0x7C, 0x14, 0x14, 0x14, 0x08 },
    /* 113 'q' */ { 0x08, 0x14, 0x14, 0x18, 0x7C },
    /* 114 'r' */ { 0x7C, 0x08, 0x04, 0x04, 0x08 },
    /* 115 's' */ { 0x48, 0x54, 0x54, 0x54, 0x20 },
    /* 116 't' */ { 0x04, 0x3F, 0x44, 0x40, 0x20 },
    /* 117 'u' */ { 0x3C, 0x40, 0x40, 0x20, 0x7C },
    /* 118 'v' */ { 0x1C, 0x20, 0x40, 0x20, 0x1C },
    /* 119 'w' */ { 0x3C, 0x40, 0x30, 0x40, 0x3C },
    /* 120 'x' */ { 0x44, 0x28, 0x10, 0x28, 0x44 },
    /* 121 'y' */ { 0x0C, 0x50, 0x50, 0x50, 0x3C },
    /* 122 'z' */ { 0x44, 0x64, 0x54, 0x4C, 0x44 },
    /* 123 '{' */ { 0x00, 0x08, 0x36, 0x41, 0x00 },
    /* 124 '|' */ { 0x00, 0x00, 0x7F, 0x00, 0x00 },
    /* 125 '}' */ { 0x00, 0x41, 0x36, 0x08, 0x00 },
    /* 126 '~' */ { 0x08, 0x04, 0x08, 0x10, 0x08 }
};

size_t ssd1306_get_framebuffer_size(uint8_t width, uint8_t height) {
    return width*(height>>3);
}

static esp_err_t ssd1306_write_cmd(ssd1306_t *context, uint8_t cmd) {
    if (!context) return ESP_FAIL;
    esp_err_t status;
    i2c_cmd_handle_t cmd_link = i2c_cmd_link_create();
    i2c_master_start(cmd_link);
    i2c_master_write_byte(cmd_link, (context->address << 1) | I2C_MASTER_WRITE, true);
    i2c_master_write_byte(cmd_link, 0x00, true);
    i2c_master_write_byte(cmd_link, cmd, true);
    i2c_master_stop(cmd_link);
    status = i2c_master_cmd_begin(context->i2c_port, cmd_link, 10 / portTICK_PERIOD_MS);
    i2c_cmd_link_delete(cmd_link);
    return status;
}

esp_err_t ssd1306_write_cmds(ssd1306_t *context, size_t n, const uint8_t * cmds) {
    if (!context) return ESP_FAIL;
    esp_err_t status;
    i2c_cmd_handle_t cmd_link = i2c_cmd_link_create();
    i2c_master_start(cmd_link);
    i2c_master_write_byte(cmd_link, (context->address << 1) | I2C_MASTER_WRITE, true);
    i2c_master_write_byte(cmd_link, 0x00, true);
    i2c_master_write(cmd_link, cmds, n, true);
    i2c_master_stop(cmd_link);
    status = i2c_master_cmd_begin(context->i2c_port, cmd_link, 100 / portTICK_PERIOD_MS);
    i2c_cmd_link_delete(cmd_link);
    return status;
}

esp_err_t ssd1306_default_init(ssd1306_t *context) {
    if (!context) return ESP_FAIL;
    const uint8_t cmds[10] = {0xAE, 0xA8, 0x1F, 0xDA, (1 << 1), 0x20, 0x00, 0x8D, 0x14, 0xAF};
    return ssd1306_write_cmds(context, sizeof(cmds), cmds);
}

esp_err_t ssd1306_flush(ssd1306_t *context) {
    if (!context) return ESP_FAIL;
    const uint8_t cmds[6] = {0x21, 0x00, 0x7F, 0x22, 0x00, 0x03};
    esp_err_t status = ssd1306_write_cmds(context, sizeof(cmds), cmds);
    if (status != ESP_OK) return status;
    i2c_cmd_handle_t cmd_link = i2c_cmd_link_create();
    i2c_master_start(cmd_link);
    i2c_master_write_byte(cmd_link, (context->address << 1) | I2C_MASTER_WRITE, true);
    i2c_master_write_byte(cmd_link, (1 << 6), true);
    i2c_master_write(cmd_link, context->framebuffer, ssd1306_get_framebuffer_size(context->width, context->height), true);
    i2c_master_stop(cmd_link);
    status = i2c_master_cmd_begin(context->i2c_port, cmd_link, 100 / portTICK_PERIOD_MS);
    i2c_cmd_link_delete(cmd_link);
    return status;
}

void ssd1306_set_pixel(ssd1306_t * context, int x, int y, bool color) {
    if (!context || !context->width || !context->height || !context->framebuffer) {
        return;
    }
    if (x >= context->width || x < 0 || y >= context->height || y < 0) {
        return;
    }
    int page = y>>3;
    int arr_coordinate = context->width*page + x;
    uint8_t pixel_height = (1 << (y & 7)); // 1 is pushed up every y & 7 which is exact to y mod 8
    if (color) {
        context->framebuffer[arr_coordinate] |= pixel_height;
    } else {
        context->framebuffer[arr_coordinate] &= ~pixel_height;
    }
}

static esp_err_t ssd1306_draw_char(ssd1306_t *context, int x, int y, char c) {
    if (!context) return ESP_FAIL;
    if (c < 32 || c > 126) return ESP_FAIL;
    if (x >= context->width || x < 0 || y >= context->height || y < 0) return ESP_FAIL;
    int index = c - 32;
    const uint8_t *character = font5x7[index];
    for (int col = 0; col < 5; col++) {
        for (int row = 0; row < 8; row++) {
            uint8_t byte = (character[col] >> row) & 0x01;
            ssd1306_set_pixel(context, x+col, y+row, byte);
        }
    }
    return ESP_OK;
}

esp_err_t ssd1306_clear(ssd1306_t *context) {
    if (!context) return ESP_FAIL;
    memset(context->framebuffer, 0x00, ssd1306_get_framebuffer_size(context->width, context->height));
    return ESP_OK;
}

esp_err_t ssd1306_draw_string(ssd1306_t *context, int x, int y, const char *str) {
    if (!context) return ESP_FAIL; 
    if (x > context->width || x < 0 || y < 0 || y > context->height) {
        return ESP_FAIL;
    }
    int cursor_x = x;
    while (*str != '\0') {
        if (cursor_x + FONT5X7_WIDTH+1 >= context->width) {
            cursor_x = x;
            y += FONT5X7_HEIGHT+1;
            if (y >= context->height) break;
        }
        ssd1306_draw_char(context, cursor_x, y, *str);
        cursor_x += FONT5X7_WIDTH+1;
        str++;
    }
    return ESP_OK;
}

esp_err_t ssd1306_progress_bar(ssd1306_t *context, int x, int y, int height, int width, uint8_t percent) {
    if (!context) return ESP_FAIL;
    if (x + width > context->width || x < 0 || y < 0 || y + height > context->height || percent > 100) {
        return ESP_FAIL;
    }
    for (int i = 0; i <= width; i++) {
        ssd1306_set_pixel(context, x+i, y, 1);
        ssd1306_set_pixel(context, x+i, y+height, 1);
    }
    for (int i = 0; i <= height; i++) {
        ssd1306_set_pixel(context, x, y+i, 1);
        ssd1306_set_pixel(context, x+width, y+i, 1);
    }
    int min_x = x+1;
    int min_y = y+1;
    int width_pixels = ((width-2)*percent)/100;
    int height_pixels = height-1;
    
    for(int ix = 0; ix < width_pixels; ix++) {
        for (int iy = 0; iy < height_pixels; iy++) {
            ssd1306_set_pixel(context, min_x+ix, min_y+iy, 1);
        }
    }
    return ESP_OK;
}

esp_err_t ssd1306_deinit(ssd1306_t *context) {
    if (!context) return ESP_FAIL;
    ssd1306_write_cmd(context, 0xAE);
    free(context->framebuffer);
    return ESP_OK;
}
