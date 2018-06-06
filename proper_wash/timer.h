#ifndef TIMER_H
#define	TIMER_H

#include <xc.h>
#include <stdint.h>
#include "define.h"

void timer_config();
void timer_int_enable();
void timer_int_disable();
void timer_reset();

#endif
