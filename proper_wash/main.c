#include <xc.h>
#include <stdint.h>
#include <stdbool.h>
#include "config.h"
#include "define.h"
#include "timer.h"
#include "i2c.h"
#include "lcd.h"
#include "adc.h"
#include "buzzer.h"
#include "utility.h"

//global state variables for timers
volatile uint8_t count = 0, seconds = 0;
volatile bool update = false;

//function prototypes
void io_config();
void display_banner();
void lather_cycle();
void condition_checking();
void rinse_cycle();

void main(){
    io_config(); //configure inputs and outputs
    adc_config(); //configure adc
    timer_config(); //configure timer
    pwm_config(); //configure buzzer (default 2500 Hz, 75% duty)
    i2c_config(); //configure mssp module to i2c master mode
    expander_init(); //initialize io expander
    lcd_init(); //initialize lcd
    display_banner();
    
    //main loop
    while(1){
       lather_cycle();
       condition_checking();
       rinse_cycle();
    }
}

void io_config(){
    ANSELA = 0x0C; //set PORTA to digital input (except for VREFs)
    ANSELB = 0x04; //set PORTB for analog on T_SENSE
    PORTA = 0x00; //initialize ports to zero
    PORTB = 0x00; 
    
    //configure inputs
    T_SENSE_TRIS = 1;
    T_SEL_TRIS = 1;
    VREF_5_TRIS = 1;
    VREF_1_TRIS = 1;
    HAND_DETECT_TRIS = 1;
    I2C_DATA_TRIS = 1;
    I2C_CLK_TRIS = 1;
    
    //configure outputs
    GRN_LED_TRIS = 0;
    RED_LED_TRIS = 0;
    BUZZER_TRIS = 0;
}

void display_banner(){
    lcd_write_line("     Proper     ", FIRST_LINE);
    lcd_write_line("      Wash      ", SECOND_LINE);
    for(uint8_t i = 0; i < 4; ++i) blink_both();
    on_notification(); 
}

void lather_cycle(){
    lcd_write_line("Please wet hands", FIRST_LINE);
    lcd_write_line("and lather soap " , SECOND_LINE);
    
    while(!HAND_DETECT) blink_green(); //wait for hands
    GRN_LED = 1; //make sure green led is on
    
    //display lather timer
    lcd_clear();
    lcd_write_line("Lather:        s", FIRST_LINE);
    lcd_set_pos(FIRST_LINE, 12);
    lcd_print_num((uint8_t)(LATHER_TIME), 2);
   
    timer_reset();
    timer_int_enable(); //start timing
    while(seconds < LATHER_TIME){
        if(update){ //every second, update countdown
            lcd_print_num((uint8_t)(LATHER_TIME - seconds), 2);
            update = false;
        }
        if(HAND_DETECT && seconds > 1){ //early rinse (FIXME?)
            timer_int_disable();
            GRN_LED = 0;
            lcd_write_line("Please continue ", FIRST_LINE);
            lcd_write_line("lathering hands ", SECOND_LINE);
            while(HAND_DETECT) red_error();
            GRN_LED = 1;
            lcd_clear();
            
            lcd_write_line("Lather:        s", FIRST_LINE);
            lcd_set_pos(FIRST_LINE, 12);
            lcd_print_num((uint8_t)(LATHER_TIME - seconds), 2);    
            timer_int_enable();  
        }
    }
    timer_int_disable(); //stop timing
    
    //reset global variables and clear screen
    count = 0;
    seconds = 0;
    lcd_clear();
}

void condition_checking(){
    bool unit = T_SEL; //represents temp unit (F = 1, C = 0)
    
    if(!check_temp(unit)){ //check water temperature
        lcd_write_line("Temp is too low ", FIRST_LINE);
        lcd_write_line("Please adjust!  ", SECOND_LINE);
        GRN_LED = 0; //blink red led
        for(uint8_t i = 0; i < 5; ++i){
            if(i<2) red_error();
            else blink_red();
        }
        RED_LED = 1;

        //display current temp while user is adjusting it
        lcd_write_line("Minimum:        ", FIRST_LINE);
        lcd_write_line("Current:        ", SECOND_LINE);
        uint8_t temp_c = ((TEMP-32)*5)/9; //minimum temp in Celsius
        while(!check_temp(unit)){
            unit = T_SEL; //check if unit switch has changed
            
            lcd_set_pos(FIRST_LINE, 10); //update minimum temp
            if(unit){
                lcd_print_num((uint8_t)TEMP, 3);
                lcd_set_pos(FIRST_LINE, 14);
                lcd_send_byte(0xDF, DATA); //degree symbol
                lcd_send_byte('F', DATA);   
            }else{
                lcd_print_num(temp_c, 3);
                lcd_set_pos(FIRST_LINE, 14);
                lcd_send_byte(0xDF, DATA);
                lcd_send_byte('C', DATA);
            }  
            
            lcd_set_pos(SECOND_LINE, 10); //write current temp
            lcd_print_num(read_temp(unit), 3);
            lcd_set_pos(SECOND_LINE, 14);
            lcd_send_byte(0xDF, DATA);
            if(unit) lcd_send_byte('F', DATA);
            else lcd_send_byte('C', DATA);
            __delay_ms(200);
        }
        lcd_clear();
        
        RED_LED = 0;
        GRN_LED = 1; //turn green led back on
    }
}

void rinse_cycle(){  
    lcd_write_line("Begin rinsing   ", FIRST_LINE);
    while(!HAND_DETECT){
        if(!check_temp(T_SEL)) {
            condition_checking();
            lcd_write_line("Begin rinsing   ", FIRST_LINE);
            lcd_write_line("                ", SECOND_LINE);
        }
        else blink_green();
    }
    GRN_LED = 1;
    lcd_clear();
    
    lcd_write_line("Rinse:         s", FIRST_LINE);
    lcd_set_pos(FIRST_LINE, 12);
    lcd_print_num((uint8_t)(RINSE_TIME), 2);
    
    timer_reset();
    timer_int_enable();
    while(seconds < RINSE_TIME){
        if(update){
            lcd_print_num((uint8_t)(RINSE_TIME - seconds), 2);
            update = false;
        }
        
        if(!check_temp(T_SEL)){ //temp detected as too low
            timer_int_disable();
            condition_checking();
            
            lcd_write_line("Rinse:         s", FIRST_LINE);
            lcd_set_pos(FIRST_LINE, 12);
            lcd_print_num((uint8_t)(RINSE_TIME - seconds), 2);
            timer_int_enable();
        }
        
        if(!HAND_DETECT){ //hands not present in sink
            __delay_ms(100); //buffer (FIXME?)
            if(!HAND_DETECT){
                timer_int_disable();
                lcd_write_line("Please continue ", FIRST_LINE);
                lcd_write_line("rinsing hands   ", SECOND_LINE);
                while(!HAND_DETECT){
                    if(!check_temp(T_SEL)) {
                        condition_checking();
                        lcd_write_line("Please continue ", FIRST_LINE);
                        lcd_write_line("rinsing hands   ", SECOND_LINE);
                    }
                    else blink_green();
                }
                GRN_LED = 1;
                lcd_clear();

                lcd_write_line("Rinse:         s", FIRST_LINE);
                lcd_set_pos(FIRST_LINE, 12);
                lcd_print_num((uint8_t)(RINSE_TIME - seconds), 2);    
                timer_int_enable();
            }
        }
    }
    timer_int_disable();
    
    //reset global variables and clear screen
    count = 0;
    seconds = 0;
    lcd_clear();
    
    lcd_write_line("     Done!      ", FIRST_LINE);
    done_notification();
    __delay_ms(2000);
    while(HAND_DETECT);
}

void interrupt Timer0_ISR(){
    //verify overflow interrupt is enabled and flag is set
    if(INTCONbits.TMR0IE && INTCONbits.TMR0IF){ 
        INTCONbits.TMR0IF = 0; //clear the flag
        count++;
        if(count == 23){ //one second has passed (actually 1.0049 s)
            count = 0;
            seconds++;
            update = true;
        }
    }
}