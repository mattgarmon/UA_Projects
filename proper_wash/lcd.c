#include "lcd.h"

void expander_init(){
    i2c_register_write(IOEXP_ADDRESS, IOEXP_IODIR, 0x00); //set all gpio to output
}

//nibble: DB7 DB6 DB5 DB4
void lcd_send_nibble(uint8_t nibble, MODE mode){
    uint8_t byte = 0x80; //default with light on
    if(mode == DATA) byte |= 0x02; //set RS if data write
    byte |= (uint8_t)((nibble & 0x0F) << 3); //add data/command to byte
    
    //pulse EN
    i2c_register_write(IOEXP_ADDRESS, IOEXP_GPIO, byte); //put byte in GPIO
    __delay_us(1);
    i2c_register_write(IOEXP_ADDRESS, IOEXP_GPIO, (uint8_t)(byte | 0x04)); //toggle EN to 1
    __delay_us(1);
    i2c_register_write(IOEXP_ADDRESS, IOEXP_GPIO, byte);  //toggle EN back to 0
    
    __delay_us(50); //general wait time for instructions
    }

//send a byte in 4-bit mode
void lcd_send_byte(uint8_t byte, MODE mode){
    lcd_send_nibble((uint8_t)(byte >> 4), mode); //send higher 4 bits
    lcd_send_nibble((uint8_t)(byte & 0x0F), mode); //send lower four bits
}

void lcd_set_pos(LINE line, uint8_t pos){
    if(pos < 16){
        uint8_t cmd = 0x80; //set DDRAM address command
        //add position address
        if(line == FIRST_LINE) cmd |= pos;
        else cmd |= (uint8_t)(0x40 | pos);
        lcd_send_byte(cmd, COMMAND); //send command
    }
}

void lcd_clear(){
    lcd_send_byte(0x01, COMMAND); //send command to clear screen
    __delay_ms(2);
}

void lcd_clear_line(LINE line){
    lcd_set_pos(line, 0);
    for(uint8_t i = 0; i < 16; ++i)
        lcd_send_byte(' ', DATA);
    lcd_set_pos(line, 0);
}

void lcd_init(){
    __delay_ms(20); //wait for LCD internal initialization
    lcd_send_nibble(0x3, COMMAND); //function set (default 8 bit mode)
    __delay_ms(5);
    lcd_send_nibble(0x3, COMMAND); 
    __delay_ms(1);
    lcd_send_nibble(0x3, COMMAND);
    __delay_ms(5);
    lcd_send_nibble(0x2, COMMAND); //4-bit mode
    __delay_ms(5);
    lcd_send_byte(0x28, COMMAND); //4-bit, 2-line, 5x8 font
    __delay_ms(5);
    lcd_send_byte(0x0C, COMMAND); //turn on display, no cursor, no blinking
    __delay_us(50);
    lcd_send_byte(0x06, COMMAND); //entry mode set
    __delay_us(50);
    lcd_clear();
}

void lcd_print(const char* text){
    //print string where cursor is
    while(*text != '\0') lcd_send_byte(*(text++), DATA);
}

//prints an integer, uses len characters, returns cursor to origin
void lcd_print_num(uint16_t num, uint8_t len){
    for(uint8_t i = 0; i < len-1; ++i) lcd_send_byte(0x14, COMMAND); //move cursor right
    lcd_send_byte(0x04, COMMAND); //change entry mode to advance left
    for(uint8_t i = 0; i < len; ++i){ //write digits from right to left
        if(num > 0 || i == 0) lcd_send_byte((uint8_t)(0x30 | num%10), DATA);
        else lcd_send_byte(' ', DATA);
        num /= 10;
    }
    lcd_send_byte(0x14, COMMAND); //move cursor to original position
    lcd_send_byte(0x06, COMMAND); //change entry mode back
}

void lcd_write_line(const char* text, LINE line){
    lcd_set_pos(line, 0);
    lcd_print(text);
}

void lcd_write_pos(const char* text, LINE line, uint8_t pos){
    lcd_set_pos(line, pos);
    lcd_print(text);
}
