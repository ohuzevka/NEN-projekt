/* 
 * File:   main.c
 * Author: xhuzev00
 *
 * Created on 25. b?ezna 2025, 16:22
 */

#include <stdio.h>
#include <stdlib.h>
#include <pic16f917.h>

/*
 * RD1 - button
 * RD2 - LOWsdie motor
 * RD5 - LED
 * RC5 - Optical interrupter
 * RD7 - High side motor
 */

#define BUTTON_PIN      (PORTDbits.RD1)
#define LED_PIN         (PORTDbits.RD5)
#define MOTOR_NEG_PIN   (PORTDbits.RD2)
#define MOTOR_POS_PIN   (PORTDbits.RD7)
#define OPT_SENSOR_PIN  (PORTCbits.RC5)

/*
 * 
 */
int main(int argc, char** argv) {
    
    // TRISD |= (1<<1);    // RD1 input 
    // TRISC |= (1<<5);    // RC5 input 
    // TRISD &= ~(1<<2);    // RD2 output
    // TRISD &= ~(1<<7);    // RD7 output
    // TRISD &= ~(1<<5);    // RD5 output

    // PORTD |= (1<<2);    // RD2 write HIGH
    
    // while(1) {
    //     if (PORTDbits.RD1){    // IF RD1 is HIGH
    //         PORTD &= ~(1<<7);    // Write LOW to RD7
    //     } else {
    //         PORTD |= (1<<7);    // Write HIGH to RD7
    //     }
        
    //     if (PORTCbits.RC5){    // IF RB7 is HIGH
    //         PORTD |= (1<<5);    // Write HIGH to RD5
    //     } else {
    //         PORTD &= ~(1<<5);    // Write LOW to RD5
    //     }
    // }

    // Port setup
    TRISD |= _TRISD_TRISD1_MASK | _TRISD_TRISD5_MASK;                           // Set TRISD pins as input
    TRISD &= ~(_TRISD_TRISD2_MASK | _TRISD_TRISD5_MASK | _TRISD_TRISD7_MASK);   // Set TRISD pins as output
    TRISC |= _TRISC_TRISC5_MASK;                                                // Set TRISC pins as input
    
    MOTOR_NEG_PIN = 1;  // Activate LOW side transistor to connect motor negative pin to GND

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
    }
    
    return (EXIT_SUCCESS);
}

