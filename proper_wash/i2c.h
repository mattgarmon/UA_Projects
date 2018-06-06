#ifndef I2C_H
#define I2C_H

#include <xc.h>
#include <stdint.h>
#include "define.h"

void i2c_config();
void i2c_wait();
void i2c_start();
void i2c_stop();
void i2c_send(uint8_t data);
void i2c_register_write(uint8_t addr, uint8_t reg, uint8_t data);

#endif