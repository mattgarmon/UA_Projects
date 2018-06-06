#ifndef BUZZER_H
#define	BUZZER_H

#include <xc.h>
#include <stdint.h>
#include "define.h"

void pwm_config();
void pwm_init(uint8_t);
void pwm_enable();
void pwm_disable();
void pwm_play(uint16_t);

#endif