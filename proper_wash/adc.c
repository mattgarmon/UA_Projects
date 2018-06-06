#include "adc.h"

void adc_config(){
    ADCON0 = 0x28; //AN10 channel only
    ADCON1 = 0xC6; //result right justified, Fosc/4 source (1.33 us Tad), VREFs
    __delay_ms(2); //general aquisition time for channel selection
}

uint16_t adc_read(){
    ADCON0bits.ADON = 1; //ADC on
    ADCON0bits.GO_nDONE = 1; //set initalize bit
    while(ADCON0bits.GO_nDONE); //wait while conversion is in progress
    ADCON0bits.ADON = 0; //ADC off
    return (uint16_t)(ADRESH << 8 | ADRESL); //return result in lower 10 bits
}

uint8_t read_temp(bool f){
    //read adc result and convert to rounded temperature representation
    if(f) return (adc_read()*(uint32_t)144 + 512)/1023 + 32; //F
    else return (adc_read()*(uint32_t)80 + 512)/1023; //C
}

bool check_temp(bool f){
    //checks if temp is above threshold (different results for different units)
    if(f) return (bool)(read_temp(f) >= TEMP);
    else return (bool)(read_temp(f) >= ((TEMP-32)*5)/9);
}