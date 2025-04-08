

#include <xc.h>
// #include <stdio.h>
// #include <stdlib.h>
// #include <pic16f917.h>

#include "LCD.h"


#define BUTTON_PIN      (PORTDbits.RD1)
#define LED_PIN         (PORTDbits.RD5)
#define MOTOR_NEG_PIN   (PORTDbits.RD2)
#define MOTOR_POS_PIN   (PORTDbits.RD7)
// #define OPT_SENSOR_PIN  (PORTCbits.RC5)
#define OPT_SENSOR_PIN  (PORTAbits.RA4) // T0CKI pin

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



// interrupt TMR1
unsigned char interrupt_cnt = 0;
void __interrupt() isr()
{
    if(PIR1bits.TMR1IF)         // interrupt of timer2
    {
        TMR1H = 0x0B;
        TMR1L = 0xB9;
        PIR1bits.TMR1IF = 0;    // clear timer0 interrupt falg

        LED_PIN = !LED_PIN;
        if (interrupt_cnt++) {
            interrupt_cnt = 0;
            
            DisplayNumber(TMR0);
            TMR0 = 0;
        }
    }
}

void main(void) 
{
    // Port setup,          1 = input, 0 = output
    TRISDbits.TRISD1 = 1;   // BUTTON_PIN setup
    TRISDbits.TRISD5 = 0;   // LED_PIN setup
    TRISDbits.TRISD2 = 0;   // MOTOR_NEG_PIN setup
    TRISDbits.TRISD7 = 0;   // MOTOR_POS_PIN setup
    // TRISCbits.TRISC5 = 1;   // OPT_SENSOR_PIN setup
    TRISAbits.TRISA4 = 1;   // OPT_SENSOR_PIN setup
    

    // TIM1 setup as 1 second timer
    T1CONbits.TMR1CS = 0;   // Internal clock (F_OSC/4)
    T1CONbits.T1SYNC = 1;   // Do not synchronize external clock input
    // T1CONbits.T1CKPS = 0x11;// Prescaler 1:8 -> fosc/4 -> 0,5us * 8 = 4us
    T1CONbits.T1CKPS0 = 1;
    T1CONbits.T1CKPS1 = 1;
    T1CONbits.TMR1ON = 1;   // Enable timer 1
    // 262 ms

    // TMR1H = 0x2F;
    // TMR1L = 0x6C;
    TMR1H = 0x0B;
    TMR1L = 0xB9;
    // Enable interuupts on overflow
    PIE1bits.TMR1IE = 1;
    INTCONbits.PEIE = 1;
    INTCONbits.GIE = 1;



    // TIM0 setup as counter
    OPTION_REGbits.T0CS = 1;    // External clock from T0CKI pin
    OPTION_REGbits.T0SE = 0;    // Increment on low-to-high transition on T0CKI pin
    // prescaler shared with WDT
    
    // TIMER2 for PWM
    T2CON = 0x05;			// fosc/4 -> 0,5us * 4 = 2us
 	PR2 = 200;          	// 2u * 200 = 400us === PERIODE
 	PIR1bits.TMR2IF = 0;    // iclear interrupt flag
 	PIE1bits.TMR2IE = 0;    // interrupt disable
    
    //PWM2
    unsigned char pwm2;     // variables for duty cycle registers of PWM
    CCP2CON = 0x0C;         // LSB bits 0, PWM mode
    pwm2 = 100;             // duty cycle to variable - must be smaller than PR2
    CCPR2L = pwm2;          // to register of duty cycle
    

    MOTOR_NEG_PIN = 1;      // Activate LOW side transistor to connect motor negative pin to GND
    initLcdDisplay();
        
    
    while(1) {
        if (BUTTON_PIN == 0){   // If button is pressed
            MOTOR_POS_PIN = 1;  // Turn motor ON
        } else {
            MOTOR_POS_PIN = 0;  // Turn motor OFF
        }
        
        if (OPT_SENSOR_PIN){    // If optical sensor is high
            LED_PIN = 1;        // Turn ON LED
        } else {
            LED_PIN = 0;        // Turn OFF LED
        }
        
        // DisplayNumber(TMR0);
    }
}

