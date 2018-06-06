#include "i2c.h"

void i2c_config(){
    //configure MSSP
    SSP1CON1 = 0x28; //i2c Master Mode 
    SSP1CON2 = 0x00;
    SSP1ADD = _XTAL_FREQ/(4*400000); //ignoring -1 for rounding purposes
    SSP1STAT = 0x00;
}

void i2c_wait(){
    //wait while module is busy
    while(SSP1STAT & 0x04 || SSP1CON2 & 0x1F);
}

void i2c_start(){
    i2c_wait();
    SSP1CON2bits.SEN = 1; //set START condition
}

void i2c_stop(){
    i2c_wait();
    SSP1CON2bits.PEN = 1; //set STOP condition
}

void i2c_send(uint8_t data){
    i2c_wait();
    SSP1BUF = data; //put data in transmit buffer
}

void i2c_register_write(uint8_t addr, uint8_t reg, uint8_t data){
    i2c_start(); //start transmission
    i2c_send(addr); //send address
    i2c_send(reg); //send destination register
    i2c_send(data); //send data
    i2c_stop(); //stop transmission
    i2c_wait();   
}