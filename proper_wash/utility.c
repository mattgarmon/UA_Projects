#include "utility.h"

void blink_red(){ //one period of red blinking, leave off
    RED_LED = 1;
    __delay_ms(200);
    RED_LED = 0;
    __delay_ms(200);
}

void blink_green(){ //one period of green blinking, leave off
    GRN_LED = 1;
    __delay_ms(200);
    GRN_LED = 0;
    __delay_ms(200);
}

void blink_both(){ //one period of both leds blinking, leaves green on
    GRN_LED = 0;
    RED_LED = 1;   
    __delay_ms(200);
    GRN_LED = 1;
    RED_LED = 0;   
    __delay_ms(200);
}

void red_error(){
    RED_LED = 1;
    pwm_play(200);
    RED_LED = 0;
    __delay_ms(200);
}

void on_notification(){
    pwm_init(1);
    pwm_play(200);
    pwm_init(2);
    pwm_play(400);
    pwm_init(0);
}
void done_notification(){
    pwm_init(2);
    pwm_play(400);
    pwm_init(1);
    pwm_play(200);
    pwm_init(0);
}

void check_adc_values(){
        lcd_write_line("ADC:            ", FIRST_LINE);
        lcd_set_pos(FIRST_LINE, 12);
        lcd_print_num(adc_read(), 4);
        lcd_write_line("TEMP:           ", SECOND_LINE);
        lcd_set_pos(SECOND_LINE, 12);
        lcd_print_num(read_temp(false),4);
        __delay_ms(500);
}

