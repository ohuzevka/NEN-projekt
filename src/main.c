

#include <xc.h>
// #include <stdio.h>
// #include <stdlib.h>
// #include <pic16f917.h>

#include "LCD.h"


// VARIABLES
unsigned char cnt, nmr1, nmr2, nmr3;

#define BUTTON_PIN      (PORTDbits.RD1)
#define LED_PIN         (PORTDbits.RD5)
#define MOTOR_NEG_PIN   (PORTDbits.RD2)
#define MOTOR_POS_PIN   (PORTDbits.RD7)
// #define OPT_SENSOR_PIN  (PORTCbits.RC5)
#define OPT_SENSOR_PIN  (PORTAbits.RA4) // T0CKI pin

void main(void) 
{
    // Port setup
    // TRISD |= _TRISD_TRISD1_MASK;                                                 // Set TRISD pins as input
    // TRISD &= ~(_TRISD_TRISD2_MASK | _TRISD_TRISD5_MASK | _TRISD_TRISD7_MASK);   // Set TRISD pins as output
    // TRISC |= _TRISC_TRISC5_MASK;                                                // Set TRISC pins as input
    // Port setup,          1 = input, 0 = output
    TRISDbits.TRISD1 = 1;   // BUTTON_PIN setup
    TRISDbits.TRISD5 = 0;   // LED_PIN setup
    TRISDbits.TRISD2 = 0;   // MOTOR_NEG_PIN setup
    TRISDbits.TRISD7 = 0;   // MOTOR_POS_PIN setup
    // TRISCbits.TRISC5 = 1;   // OPT_SENSOR_PIN setup
    TRISAbits.TRISA4 = 1;   // OPT_SENSOR_PIN setup
    

    // TIM1 setup as counter
    // T1CON = 0x00 | _T1CON_TMR1CS_MASK | _T1CON_T1SYNC_MASK | _T1CON_TMR1ON_MASK;
    // T1CONbits.TMR1CS = 1;   // External clock from T1CKI pin 
    // T1CONbits.T1SYNC = 1;   // Do not synchronize external clock input
    // T1CONbits.TMR1ON = 1;   // Enable timer 1

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
        
        // Show number on LCD display
        cnt = TMR0;
        nmr3 = cnt / 100;
        nmr2 = (cnt % 100) / 10;
        nmr1 = cnt % 10;
        
        setNumberLcdDisplay(3, nmr3);
        setNumberLcdDisplay(2, nmr2);
        setNumberLcdDisplay(1, nmr1);
    }
}

