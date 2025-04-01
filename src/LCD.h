//*****************************************************************************
// source:  mainLcdExample.c 
// mcu:     PIC16F917
// author:  LL
// created: 03/2024
// project: mechatronic Kit 
// example: LCD display
//*****************************************************************************
// programm timer call each segments and characters on LCD display 
// and alternates them

//    --DxA--
//   |       |	
//  DxF     DxB
//   |       |
//    --DxG--
//   |       |
//  DxE     DxC
//   |       |
//    --DxD--

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef XC_HEADER_TEMPLATE_H
#define	XC_HEADER_TEMPLATE_H

#include <xc.h> // include processor files - each processor file is guarded.  

// CONFIG
#pragma config FOSC = INTOSCCLK // Oscillator Selection bits (INTOSC oscillator: CLKOUT function on RA6/OSC2/CLKOUT/T1OSO pin, I/O function on RA7/OSC1/CLKIN/T1OSI)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT enabled)
#pragma config PWRTE = OFF      // Power Up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = ON       // RE3/MCLR pin function select bit (RE3/MCLR pin function is MCLR)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown-out Reset Selection bits (BOR enabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is enabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is enabled)

// define segments
// DIGIT 1
#define D1A     SEG22COM0   // LCDDATA2bits.SEG22			
#define D1B     SEG23COM0   // LCDDATA2bits.SEG23
#define D1C     SEG23COM2   // LCDDATA8bits.SEG23
#define D1D     SEG22COM3   // LCDDATA11bits.SEG22
#define D1E     SEG22COM2   // LCDDATA8bits.SEG22
#define D1F     SEG22COM1   // LCDDATA5bits.SEG22
#define D1G     SEG23COM1   // LCDDATA5bits.SEG23
// DIGIT 2
#define D2A     SEG6COM0    // LCDDATA0bits.SEG6			
#define D2B     SEG21COM0   // LCDDATA2bits.SEG21
#define D2C     SEG21COM2   // LCDDATA8bits.SEG21
#define D2D     SEG6COM3    // LCDDATA9bits.SEG6
#define D2E     SEG6COM2    // LCDDATA6bits.SEG6
#define D2F     SEG6COM1    // LCDDATA3bits.SEG6
#define D2G     SEG21COM1   // LCDDATA5bits.SEG21
// DIGIT 3
#define D3A     SEG3COM0    // LCDDATA0bits.SEG3			
#define D3B     SEG11COM0   // LCDDATA1bits.SEG11
#define D3C     SEG11COM2   // LCDDATA7bits.SEG11
#define D3D     SEG3COM3    // LCDDATA9bits.SEG3
#define D3E     SEG3COM2    // LCDDATA6bits.SEG3
#define D3F     SEG3COM1    // LCDDATA3bits.SEG3
#define D3G     SEG11COM1   // LCDDATA4bits.SEG11
// DIGIT 4 (ONLY SEGMENTS B-C)
#define D4BC    SEG2COM2    // LCDDATA6bits.SEG2			
// DP (DECIMAL POINTS)
#define D2DP    SEG21COM3   // LCDDATA11bits.SEG21		
#define D3DP    SEG11COM3   // LCDDATA10bits.SEG11		
#define	D4DP    SEG2COM3    // LCDDATA9bits.SEG2			
// OTHER SHAPES
#define	SA      SEG0COM0    // LCDDATA0bits.SEG0   A
#define	SV      SEG0COM1    // LCDDATA3bits.SEG0   V
#define	SK      SEG0COM2    // LCDDATA6bits.SEG0   K
#define	SOMEGA  SEG0COM3    // LCDDATA9bits.SEG0   Omega
#define	SRC     SEG1COM0    // LCDDATA0bits.SEG1   RC
#define	SBATT   SEG1COM1    // LCDDATA3bits.SEG1   BATT
#define	SNEG    SEG1COM2    // LCDDATA6bits.SEG1   Neg sign/Dash
#define SAC     SEG1COM3    // LCDDATA9bits.SEG1   AC
#define SDH     SEG2COM0    // LCDDATA0bits.SEG2   DH
#define	SRH     SEG2COM1    // LCDDATA3bits.SEG2   RH
#define	SS1     SEG16COM0   // LCDDATA2bits.SEG16  S1
#define	SS2     SEG16COM1   // LCDDATA5bits.SEG16  S2
#define	Sm      SEG16COM2   // LCDDATA8bits.SEG16  m
#define	SM      SEG16COM3// LCDDATA11bits.SEG16 M

typedef enum
{
    SEGMENT_A = 0,
    SEGMENT_V,
    SEGMENT_K,
    SEGMENT_OMEGA,
    SEGMENT_RC,
    SEGMENT_BATT,
    SEGMENT_NEG,
    SEGMENT_AC,
    SEGMENT_DH,
    SEGMENT_RH,
    SEGMENT_S1,
    SEGMENT_S2,
    SEGMENT_m,
    SEGMENT_M,
}_segments;

// PRIVATE FUNCTION
void initLcdDisplay(void);
void clearLcdDisplay(void);
void setNumberLcdDisplay(unsigned char pos, unsigned char nmr);
void setDPLcdDisplay(unsigned char dp);
void setCharacterLcdDisplay(_segments seg);



#endif	/* XC_HEADER_TEMPLATE_H */

