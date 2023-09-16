#include <Wire.h>
#include "display.h"
#include "oled.h"

OLED::OLED(uint8_t addr, uint32_t interval) : addr(addr), interval(interval) {
}

void OLED::init_ssd1306(uint8_t addr) {
    // display off
    command_ssd1306(addr, 0xAE);
    // clock, upper nibble is rate, lower nibble is divisor
    command_ssd1306(addr, 0xD5, 0x80);
    // mux ratio, 32 pixels
    command_ssd1306(addr, 0xA8, COLPIXELS-1U);
    // display offset, 0
    command_ssd1306(addr, 0xD3, 0x00);
    // set start line, 0
    command_ssd1306(addr, 0x40);
    // charge pump, enable
    command_ssd1306(addr, 0x8D, 0x14);
    // memory addr mode, horizontal
    command_ssd1306(addr, 0x20, 0x00);
#ifdef OLED_ROTATE
    // segment remap
    command_ssd1306(addr, 0xA0);
    // com scan direction
    command_ssd1306(addr, 0xC0);
#else
    // segment remap
    command_ssd1306(addr, 0xA1);
    // com scan direction
    command_ssd1306(addr, 0xC8);
#endif
    // com hardware cfg, alt com cfg    
    if (COLPIXELS == 32U) {
        command_ssd1306(addr, 0xDA, 0x02);
    } else if (COLPIXELS == 64U) {
        command_ssd1306(addr, 0xDA, 0x12);
    }
    // 0 - 128
    command_ssd1306(addr, 0x21, 0x00, COLUMNS-1U);
    // 0 - 4
    command_ssd1306(addr, 0x22, 0x00, LINES-1U);
    // start page = 0
    command_ssd1306(addr, 0xB0);
    // display on
    command_ssd1306(addr, 0xA5);
    // contrast aka current, 128 is midpoint    
    command_ssd1306(addr, 0x81, 0xCF);
    // prechage, rtfm   
    command_ssd1306(addr, 0xD9, 0xF1);
    // vcomh deselect level, 0.77 VDD
    command_ssd1306(addr, 0xDB, 0x40);
    // scroll off
    command_ssd1306(addr, 0x2E);
    // display scan on
    command_ssd1306(addr, 0xA4);
    // non-inverted
    command_ssd1306(addr, 0xA6 | INVERT);
    // drivers on
    command_ssd1306(addr, 0xAF);
}


void OLED::command_ssd1306(uint8_t addr, uint8_t cmd) {
    Wire.beginTransmission(addr);
    Wire.write(OLED_CMD);
    Wire.write(cmd);
    Wire.endTransmission(true);
}

void OLED::command_ssd1306(uint8_t addr, uint8_t cmd, uint8_t conf) {
    Wire.beginTransmission(addr);
    Wire.write(OLED_CMD);
    Wire.write(cmd);
    Wire.write(conf);
    Wire.endTransmission(true);
}

void OLED::command_ssd1306(uint8_t addr, uint8_t cmd, uint8_t conf, uint8_t param) {
    Wire.beginTransmission(addr);
    Wire.write(OLED_CMD);
    Wire.write(cmd);
    Wire.write(conf);
    Wire.write(param);
    Wire.endTransmission(true);
}

void OLED::command_ssd1306(uint8_t addr, uint8_t cmd, uint8_t a, uint8_t b, uint8_t c, uint8_t d, uint8_t e, uint8_t f) {
    Wire.beginTransmission(addr);
    Wire.write(OLED_CMD);
    Wire.write(cmd);
    Wire.write(a);
    Wire.write(b);
    Wire.write(c);
    Wire.write(d);
    Wire.write(e);
    Wire.write(f);
    Wire.endTransmission(true);
}

void OLED::write_data_ssd1306(uint8_t addr, uint8_t* data, uint32_t len) {
    Wire.beginTransmission(addr);
    Wire.write(OLED_DATA);
    Wire.write(0x00);
    Wire.write(0x00);
    Wire.write(data, len);
    Wire.endTransmission(true);
}

void OLED::start(int sda, int scl) {
    (void)sda;
    (void)scl;
    init_ssd1306(addr);
    init_display(COLUMNS, LINES);

    memset(display_buf, 0x00, sizeof(display_buf));
    update_ssd1306(addr, display_buf);
}

void OLED::update_ssd1306(uint8_t addr, uint32_t* data) {
    // Start from page 0
    command_ssd1306(addr, 0xB0);
    // Start from column 0
    command_ssd1306(addr, 0x00);
    command_ssd1306(addr, 0x10);
    uint32_t x = 0U;
    for (uint32_t i = 0U; i < (COLUMNS)*(LINES); i += 128U) {
        if ((0x01 << x) & new_content) {
            command_ssd1306(addr, 0xB0 | x);
            command_ssd1306(addr, 0x00);
            command_ssd1306(addr, 0x10);
            write_data_ssd1306(addr, ((uint8_t*)data)+i, 128U);
        }
        x++;
    }
    new_content = 0x00;
}

void OLED::refresh(void) {
    if (new_content > 0) {
        update_ssd1306(addr, display_buf);
    }
}

void OLED::set_unit(enum UNITS unit) {
    print_unit(display_buf, unit);
    if (value_height == text_height::single_line) {
        new_content |= (0x01 << (0U + VALUE_START_LINE));
    } else if (value_height == text_height::double_line) {
        new_content |= (0x01 << (1U + VALUE_START_LINE));
    } else if (value_height == text_height::triple_line) {
        new_content |= (0x01 << (2U + VALUE_START_LINE));
    } else if (value_height == text_height::quad_line) {
        new_content |= (0x01 << (3U + VALUE_START_LINE));
    } 
}

void OLED::set_header(const char *buf, Alignment alignment) {
    print_header(display_buf, buf, alignment);
    if (header_height == text_height::single_line) {
        new_content |= (0x01 << HEADER_START_LINE);
    } else if (header_height == text_height::double_line) {
        new_content |= (0x03 << HEADER_START_LINE);
    }
}

void OLED::set_value(const char *buf) {
    print_value(display_buf, buf);
    if (value_height == text_height::single_line) {
        new_content |= (0x01 << VALUE_START_LINE);
    } else if (value_height == text_height::double_line) {
        new_content |= (0x03 << VALUE_START_LINE);
    } else if (value_height == text_height::triple_line) {
        new_content |= (0x07 << VALUE_START_LINE);
    } else if (value_height == text_height::quad_line) {
        new_content |= (0x0F << VALUE_START_LINE);
    } 
    // debug_data(display_buf);
}


void OLED::set_value(int32_t value, uint8_t decimals) {
    char str[8] = "";
    bool negative = false;

    if (value < 0U) {
        negative = true;
        value = -value;
    }

    switch (decimals) {
        case 0U:
            value = value%1000000;
            if (negative) {
                sprintf(str, "-%0ld", value);
            } else {
                sprintf(str, "%0ld", value);
            }
            set_value(str);
            break;
        case 1U:
            value = value%100000;
            if (negative) {
                sprintf(str, "-%0ld.%01ld", value/10U, value%10U);
            } else {
                sprintf(str, "%0ld.%01ld", value/10U, value%10U);
            }
            set_value(str);
            break;
        case 2U:
            value = value%100000;
            if (negative) {
                sprintf(str, "-%0ld.%02ld", value/100U, value%100U);
            } else {
                sprintf(str, "%0ld.%02ld", value/100U, value%100U);
            }
            set_value(str);
            break;
        case 3U:
            value = value%100000;
            if (negative) {
                sprintf(str, "-%0ld.%03ld", value/1000U, value%1000U);
            } else {
                sprintf(str, "%0ld.%03ld", value/1000U, value%1000U);
            }
            set_value(str);
            break;
        default:
                break;
    }

    //cprintf("%s\n", str);
}