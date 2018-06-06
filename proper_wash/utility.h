#ifndef UTILITY_H
#define UTILITY_H

#include <xc.h>
#include <stdint.h>
#include <stdbool.h>
#include "define.h"
#include "buzzer.h"
#include "lcd.h"
#include "adc.h"

void blink_red();
void blink_green();
void blink_both();
void red_error();
void on_notification();
void done_notification();

//testing functions
void check_adc_values();

#endif