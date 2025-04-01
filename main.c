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

#include <xc.h>

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

//*****************************************************************************
// initialization
void initLcdDisplay(void)
{
    LCDSE0 = 0x4F;      // enabled SEG0, SEG1, SEG2, SEG3, SEG6
    LCDSE1 = 0x08;      // enabled SEG11
    LCDSE2 = 0xE1;      // enabled SEG16, SEG21, SEG22, SEG23
    LCDPS = 0x30;       // LCDA active, LCDDATAx enabled
    LCDCON = 0x93;      // enabled, VLCD enabled, COM0-COM3
    clearLcdDisplay();  // clear all segments
}

//*****************************************************************************
// clear all segments
void clearLcdDisplay(void)
{
    LCDDATA0 = 0;       
    LCDDATA1 = 0;
    LCDDATA2 = 0;
    LCDDATA3 = 0;
    LCDDATA4 = 0;
    LCDDATA5 = 0;
    LCDDATA6 = 0;
    LCDDATA7 = 0;
    LCDDATA8 = 0;
    LCDDATA9 = 0;
    LCDDATA10 = 0;
    LCDDATA11 = 0;
}
//*****************************************************************************
// position 1-4, number 0-9, code segments
void setNumberLcdDisplay(unsigned char pos, unsigned char nmr)
{
   switch(pos)
   {
       case 1:
           D1A = D1B = D1C = D1D = D1E = D1F = D1G = 0;
           switch(nmr)
           {
               case 0: D1A = D1B = D1C = D1D = D1E = D1F = 1; break;
               case 1: D1B = D1C = 1; break;
               case 2: D1A = D1B = D1D = D1E = D1G = 1; break;
               case 3: D1A = D1B = D1C = D1D = D1G = 1; break;
               case 4: D1B = D1C = D1F = D1G = 1; break;
               case 5: D1A = D1C = D1D = D1F = D1G = 1; break;
               case 6: D1A = D1C = D1D = D1E = D1F = D1G = 1; break;
               case 7: D1A = D1B = D1C = 1; break;
               case 8: D1A = D1B = D1C = D1D = D1E = D1F = D1G = 1; break;
               case 9: D1A = D1B = D1C = D1D = D1F = D1G = 1; break;
               default: break;
           }
           break;
       case 2:
           D2A = D2B = D2C = D2D = D2E = D2F = D2G = 0;
           switch(nmr)
           {
               case 0: D2A = D2B = D2C = D2D = D2E = D2F = 1; break;
               case 1: D2B = D2C = 1; break;
               case 2: D2A = D2B = D2D = D2E = D2G = 1; break;
               case 3: D2A = D2B = D2C = D2D = D2G = 1; break;
               case 4: D2B = D2C = D2F = D2G = 1; break;
               case 5: D2A = D2C = D2D = D2F = D2G = 1; break;
               case 6: D2A = D2C = D2D = D2E = D2F = D2G = 1; break;
               case 7: D2A = D2B = D2C = 1; break;
               case 8: D2A = D2B = D2C = D2D = D2E = D2F = D2G = 1; break;
               case 9: D2A = D2B = D2C = D2D = D2F = D2G = 1; break;
               default: break;
           }
           break;
       case 3:
           D3A = D3B = D3C = D3D = D3E = D3F = D3G = 0;
           switch(nmr)
           {
               case 0: D3A = D3B = D3C = D3D = D3E = D3F = 1; break;
               case 1: D3B = D3C = 1; break;
               case 2: D3A = D3B = D3D = D3E = D3G = 1; break;
               case 3: D3A = D3B = D3C = D3D = D3G = 1; break;
               case 4: D3B = D3C = D3F = D3G = 1; break;
               case 5: D3A = D3C = D3D = D3F = D3G = 1; break;
               case 6: D3A = D3C = D3D = D3E = D3F = D3G = 1; break;
               case 7: D3A = D3B = D3C = 1; break;
               case 8: D3A = D3B = D3C = D3D = D3E = D3F = D3G = 1; break;
               case 9: D3A = D3B = D3C = D3D = D3F = D3G = 1; break;
               default: break;
           }
           break;
       case 4:
           if(nmr == 1)
               D4BC = 1;
           else
               D4BC = 0;
           break;
       default:
           break;
   }
}

//*****************************************************************************
// decimalpoint on position 2, 3, 4
void setDPLcdDisplay(unsigned char dp)
{
    if(dp == 2) D2DP = 1; else D2DP = 0;
    if(dp == 3) D3DP = 1; else D3DP = 0;
    if(dp == 4) D4DP = 1; else D4DP = 0;
}

//*****************************************************************************
// other paterns on display
void setCharacterLcdDisplay(_segments seg)
{
    if(seg == SEGMENT_A) SA = 1; else SA = 0;
    if(seg == SEGMENT_V) SV = 1; else SV = 0;
    if(seg == SEGMENT_K) SK = 1; else SK = 0;
    if(seg == SEGMENT_OMEGA) SOMEGA = 1; else SOMEGA = 0;
    if(seg == SEGMENT_RC) SRC = 1; else SRC = 0;
    if(seg == SEGMENT_BATT) SBATT = 1; else SBATT = 0;
    if(seg == SEGMENT_NEG) SNEG = 1; else SNEG = 0;
    if(seg == SEGMENT_AC) SAC = 1; else SAC = 0;
    if(seg == SEGMENT_DH) SDH = 1; else SDH = 0;
    if(seg == SEGMENT_RH) SRH = 1; else SRH = 0;
    if(seg == SEGMENT_S1) SS1 = 1; else SS1 = 0;
    if(seg == SEGMENT_S2) SS2 = 1; else SS2 = 0;
    if(seg == SEGMENT_m) Sm = 1; else Sm = 0;
    if(seg == SEGMENT_M) SM = 1; else SM = 0;
}
