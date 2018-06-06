//parameter settings
#define TEMP                90     //default desired temp in fahrenheit
#define LATHER_TIME         15      //lather cycle time in seconds
#define RINSE_TIME          10      //rinse cycle time in seconds

//pin definitions
#define GRN_LED             PORTAbits.RA4
#define RED_LED             PORTAbits.RA0
#define HAND_DETECT         PORTBbits.RB3
#define I2C_DATA            PORTBbits.RB1
#define I2C_CLK             PORTBbits.RB4
#define T_SENSE             PORTBbits.RB2
#define T_SEL               PORTAbits.RA1
#define BUZZER              PORTBbits.RB0
#define VREF_5              PORTAbits.RA3
#define VREF_1              PORTAbits.RA2

//pin tristate definitions
#define GRN_LED_TRIS        TRISAbits.TRISA4
#define RED_LED_TRIS        TRISAbits.TRISA0
#define HAND_DETECT_TRIS    TRISBbits.TRISB3
#define I2C_DATA_TRIS       TRISBbits.TRISB1
#define I2C_CLK_TRIS        TRISBbits.TRISB4
#define T_SENSE_TRIS        TRISBbits.TRISB2
#define T_SEL_TRIS          TRISAbits.TRISA1
#define VREF_5_TRIS         TRISAbits.TRISA3
#define VREF_1_TRIS         TRISAbits.TRISA2
#define BUZZER_TRIS         TRISBbits.TRISB0

//oscillator frequency
#define _XTAL_FREQ          3000000









