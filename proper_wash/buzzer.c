#include "buzzer.h"

//modes:
//0 : 2500 Hz, 75% duty (default)
//1 : 4000 Hz, 75% duty
//2 : 1800 Hz, 75% duty

void pwm_config(){ //initializes to default (mode = 0)
    BUZZER_TRIS = 1;
    APFCON0bits.CCP1SEL = 1; //set CCP1 to RB0
    PR2 = 74; //2500 Hz (default))
    CCP1CONbits.DC1B = 0b01;  //75% duty (default)
    CCPR1L = 0x38;
    CCPTMRSbits.C1TSEL = 0; //CCP1 configured to timer 2
    PIR1bits.TMR2IF = 0;
    T2CON = 0x05; //prescaler = 4, enabled   
    while(PIR1bits.TMR2IF != 1);
    BUZZER_TRIS = 0;   
}

void pwm_init(uint8_t mode){ //change modes
    if(mode == 1){
        PR2 = 53; //3500 Hz 
        CCP1CONbits.DC1B = 0b10;  //75% duty
        CCPR1L = 0x24;
    }
    else if(mode == 2){
        PR2 = 93; //2000 Hz 
        CCP1CONbits.DC1B = 0b10;  //75% duty
        CCPR1L = 0x46;
    }
    else{ //default
        PR2 = 74; //2500 Hz 
        CCP1CONbits.DC1B = 0b01;  //75% duty
        CCPR1L = 0x38;
    }
    
    //wait for next cycle
    PIR1bits.TMR2IF = 0;
    T2CON = 0x05; //prescaler = 4, enabled   
    while(PIR1bits.TMR2IF != 1);
}

void pwm_enable(){
    CCP1CON |= 0x0C; //PWM mode on
}

void pwm_disable(){
    CCP1CON &= 0xF3; //PWM mode on
}

void pwm_play(uint16_t ms){
    pwm_enable();
    for(uint16_t i = ms; i > 0; --i) __delay_ms(1);
    pwm_disable();
}