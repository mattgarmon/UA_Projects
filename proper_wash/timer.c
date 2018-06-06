#include "timer.h"

void timer_config(){ //configure timer to overflow every 40 ms
    INTCON = 0x80; //turn global interrupts on
    OPTION_REG = 0x86; //All weak pullups are disabled (except MCLR), prescaler = 128, interrupt every 43.69ms
    TMR0 = 0;
}

void timer_int_enable(){
    INTCONbits.TMR0IE = 1; //set timer overflow inturrupt enable bit
}

void timer_int_disable(){
    INTCONbits.TMR0IE = 0;//clear timer overflow inturrupt enable bit
}

void timer_reset(){
    TMR0 = 0;
}

