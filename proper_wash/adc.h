#ifndef ADC_H
#define ADC_H

#include <xc.h>
#include <stdint.h>
#include <stdbool.h>
#include "define.h"
#include "lcd.h"

void adc_config();
uint16_t adc_read();
uint8_t read_temp(bool f);
bool check_temp(bool f);

#endif
