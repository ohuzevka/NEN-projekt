#include "LCD.h"

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