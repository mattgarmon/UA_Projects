#ifndef LCD_H
#define LCD_H

#include <xc.h>
#include <stdint.h>
#include "i2c.h"
#include "define.h"

//gpio register pins
//7   6   5   4   3   2   1   0
//L   DB7 DB6 DB5 DB4 EN  RS  RW

//io expander info
#define IOEXP_ADDRESS       0x40
#define IOEXP_IODIR         0x00
#define IOEXP_GPIO          0x09

typedef enum{
    COMMAND,
    DATA,
}MODE;

typedef enum{
    FIRST_LINE,
    SECOND_LINE,
}LINE;

void expander_init();
void lcd_send_nibble(uint8_t nibble, MODE mode);
void lcd_send_byte(uint8_t byte, MODE mode);
void lcd_set_pos(LINE line, uint8_t pos);
void lcd_clear();
void lcd_clear_line(LINE line);
void lcd_init();
void lcd_print(const char* text);
void lcd_print_num(uint16_t num, uint8_t len);
void lcd_write_line(const char* text, LINE line);
void lcd_write_pos(const char* text, LINE line, uint8_t pos);

#endif