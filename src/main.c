#include <xc.h>
#include "LCD.h"
#include <stdio.h>


#define SW2_PIN         (PORTDbits.RD6)
#define SW3_PIN         (PORTDbits.RD4)
#define SW4_PIN         (PORTDbits.RD1)
#define LED_PIN         (PORTDbits.RD5)
#define MOTOR_NEG_PIN   (PORTDbits.RD2)
#define MOTOR_POS_PIN   (PORTDbits.RD7)
#define OPT_SENSOR_PIN  (PORTAbits.RA4) // T0CKI pin

#define PWM_REG CCPR2L  // Register to set PWM DC [%] 

#define PWM_STEP 5

volatile unsigned char SW2_pressed, SW3_pressed;
volatile unsigned char refresh_display_flag = 0;
volatile unsigned char RPS = 0;

void DisplayNumber(unsigned int number){
    unsigned char nmr1, nmr2, nmr3, nmr4;

    nmr4 = number / 1000;
    nmr3 = (number % 1000) / 100;
    nmr2 = (number % 100) / 10;
    nmr1 = number % 10;
    
    setNumberLcdDisplay(4, nmr4);
    setNumberLcdDisplay(3, nmr3);
    setNumberLcdDisplay(2, nmr2);
    setNumberLcdDisplay(1, nmr1);
}

void SendStrUART(char* str) {
    for (unsigned char i = 0; str[i]; i++) {
        while(PIR1bits.TXIF == 0);  // wait until ready for transmittion
        TXREG = str[i];             // transmitt char
    }
}

void main(void) 
{
    // Port setup,          1 = input, 0 = output
    TRISDbits.TRISD6 = 1;   // SW2_PIN setup
    TRISDbits.TRISD4 = 1;   // SW3_PIN setup
    TRISDbits.TRISD1 = 1;   // SW4_PIN setup
    TRISDbits.TRISD5 = 0;   // LED_PIN setup
    TRISDbits.TRISD2 = 0;   // MOTOR_NEG_PIN setup
    TRISDbits.TRISD7 = 0;   // MOTOR_POS_PIN setup
    TRISAbits.TRISA4 = 1;   // OPT_SENSOR_PIN setup
    
    // TIM1 setup as 1 seconds timer
    T1CONbits.TMR1CS = 0;   // Internal clock (F_OSC/4)
    T1CONbits.T1SYNC = 1;   // Do not synchronize external clock input
    // T1CONbits.T1CKPS = 0x01;// Prescaler 1:2 -> fosc/4 -> 1us * 2 = 2us
    T1CONbits.T1CKPS1 = 0;
    T1CONbits.T1CKPS0 = 1;
    T1CONbits.TMR1ON = 1;   // Enable timer 1
    // 262 ms

    // 1_000_000 / (2*8) = 62500 us
    // 65535 - 62500 = 3035 = 0x0BDB
    TMR1H = 0x0B;
    TMR1L = 0xDB;
    // Enable interuupts on TIM1 overflow
    PIE1bits.TMR1IE = 1;
    INTCONbits.PEIE = 1;
    INTCONbits.GIE = 1;


    // TIM0 setup as counter
    OPTION_REGbits.T0CS = 1;    // External clock from T0CKI pin
    OPTION_REGbits.T0SE = 0;    // Increment on low-to-high transition on T0CKI pin
    // prescaler shared with WDT
    
    // TIMER2 for PWM
    // T2CON = 0x05;			// fosc/4 -> 0,5us * 4 = 2us
    T2CONbits.T2CKPS1 = 0;  // Set prescaler to 4
    T2CONbits.T2CKPS0 = 1;  // Set prescaler to 4
    T2CONbits.TMR2ON = 1;   // TIM2 ON

 	PR2 = 100;          	// 2u * 100 = 200us === PERIODE
 	PIR1bits.TMR2IF = 0;    // iclear interrupt flag
 	PIE1bits.TMR2IE = 0;    // interrupt disable
    
    //PWM2
    CCP2CON = 0x0C;         // LSB bits 0, PWM mode
    PWM_REG = 50;           // to register of duty cycle
    

    // UART init
    TXSTAbits.BRGH = 1;     // Baud rate generator high
    SPBRG = 25;             // Baud rate = 9600
    // SPBRG = 23;             // Baud rate = 10417 - precise
    TXSTAbits.SYNC = 0;     // Assynchnonoous operation
    RCSTAbits.SPEN = 1;     // Enable AUSART
    TXSTAbits.TXEN = 1;     // Enable TX

    MOTOR_NEG_PIN = 1;      // Activate LOW side transistor to connect motor negative pin to GND
    initLcdDisplay();

    char TXstr[50];
   
    while(1) {
        if (SW2_pressed){
            SW2_pressed = 0;
            if ((PWM_REG - PWM_STEP) != 0) {
                PWM_REG -= PWM_STEP;
            }
        }
        if (SW3_pressed) {
            SW3_pressed = 0;
            if ((PWM_REG + PWM_STEP) <= 100) {
                PWM_REG += PWM_STEP;
            }
        }
        
        if (SW4_PIN == 0) {
            MOTOR_POS_PIN = 1;  // Turn motor ON
        } else {
            MOTOR_POS_PIN = 0;  // Turn motor OFF
        }
        
        if (OPT_SENSOR_PIN){    // If optical sensor is high
            LED_PIN = 1;        // Turn ON LED
        } else {
            LED_PIN = 0;        // Turn OFF LED
        }

        if (refresh_display_flag) {
            refresh_display_flag = 0;
            DisplayNumber(RPS);
            
            // UART send
            sprintf(TXstr, "RPS = %02d, RPM = %04d, PWM = %d %% \n\r", RPS, RPS * 60, PWM_REG);
            SendStrUART(TXstr);
        }
    }
}


volatile unsigned char SW2_last, SW3_last;

// interrupt TMR1
unsigned char interrupt_cnt = 0;
void __interrupt() isr()
{
    if(PIR1bits.TMR1IF)         // interrupt of timer1, executes every 125 ms
    {
        TMR1H = 0x0B;
        TMR1L = 0xDB;
        PIR1bits.TMR1IF = 0;    // clear timer0 interrupt flag

        // Button debounce
        if (SW2_last == 1 && SW2_PIN == 0)
            SW2_pressed = 1;
        if (SW3_last == 1 && SW3_PIN == 0)
            SW3_pressed = 1;
        
        SW2_last = SW2_PIN;
        SW3_last = SW3_PIN;

        if (interrupt_cnt++ >= 7) {   // divider by 8, executes every 1 s
            interrupt_cnt = 0;
            
            // DisplayNumber(TMR0);
            RPS = TMR0;
            refresh_display_flag = 1;
            TMR0 = 0;
        }
    }
}