

// #include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <pic16f917.h>

#include "LCD.h"


// VARIABLES
unsigned char cnt1, cnt2, cnt3, nmr1, nmr2, nmr3, char1, dp, s4;

#define BUTTON_PIN      (PORTDbits.RD1)
#define LED_PIN         (PORTDbits.RD5)
#define MOTOR_NEG_PIN   (PORTDbits.RD2)
#define MOTOR_POS_PIN   (PORTDbits.RD7)
#define OPT_SENSOR_PIN  (PORTCbits.RC5)

void main(void) 
{
    // Port setup
    TRISD |= _TRISD_TRISD1_MASK | _TRISD_TRISD5_MASK;                           // Set TRISD pins as input
    TRISD &= ~(_TRISD_TRISD2_MASK | _TRISD_TRISD5_MASK | _TRISD_TRISD7_MASK);   // Set TRISD pins as output
    TRISC |= _TRISC_TRISC5_MASK;                                                // Set TRISC pins as input
    
    // TIM1 setup
    T1CON = 0x00 | _T1CON_TMR1CS_MASK | _T1CON_T1SYNC_MASK | _T1CON_TMR1ON_MASK;
    
    // TIMER2 for PWM
    T2CON = 0x05;			// fosc/4 -> 0,5us * 4 = 2us
 	PR2 = 200;          	// 2u * 200 = 400us === PERIODE
 	PIR1bits.TMR2IF = 0;    // iclear interrupt flag
 	PIE1bits.TMR2IE = 0;    // interrupt disable
    
    //PWM2
    unsigned char pwm2;   // variables for duty cycle registers of PWM
    CCP2CON = 0x0C;         // LSB bits 0, PWM mode
    pwm2 = 100;             // duty cycle to variable - must be smaller than PR2
    CCPR2L = pwm2;          // to register of duty cycle
    
    MOTOR_NEG_PIN = 1;  // Activate LOW side transistor to connect motor negative pin to GND
    
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
        
        nmr3 = TMR1L / 100;
        nmr2 = (TMR1L % 100) / 10;
        nmr1 = TMR1L % 10;
        
        setNumberLcdDisplay(3, nmr3);
        setNumberLcdDisplay(2, nmr2);
        setNumberLcdDisplay(1, nmr1);
    }
}

