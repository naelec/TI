/* --COPYRIGHT--,BSD_EX
 * Copyright (c) 2012, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *******************************************************************************
 * 
 *                       MSP430 CODE EXAMPLE DISCLAIMER
 *
 * MSP430 code examples are self-contained low-level programs that typically
 * demonstrate a single peripheral function or device feature in a highly
 * concise manner. For this the code may rely on the device's power-on default
 * register values and settings such as the clock configuration and care must
 * be taken when combining code from several examples to avoid potential side
 * effects. Also see www.ti.com/grace for a GUI- and www.ti.com/msp430ware
 * for an API functional library-approach to peripheral configuration.
 *
 * --/COPYRIGHT--*/
//******************************************************************************
//  MSP430F20xx Demo - Software Toggle P1.0
//
//  Description; Toggle P1.0 by xor'ing P1.0 inside of a software loop.
//  ACLK = n/a, MCLK = SMCLK = default DCO
//
//                MSP430F20xx
//             -----------------
//         /|\|              XIN|-
//          | |                 |
//          --|RST          XOUT|-
//            |                 |
//            |             P1.0|-->LED
//
//  M.Buccini / L. Westlund
//  Texas Instruments, Inc
//  October 2005
//  Built with CCE Version: 3.2.0 and IAR Embedded Workbench Version: 3.40A
//******************************************************************************

#include <msp430.h>

#define LED_Red     0b00000001
#define LED_Yellow  0b00000010
#define LED_Green   0b00000100
#define SW_Red      BIT3
#define SW_Yellow   BIT4
#define SW_Green    BIT5
#define Motor       BIT6

void main (void)
{
    WDTCTL = WDTPW | WDTHOLD;           // Watchdog Password and Hold for dubug mode
    TACTL = MC_2|ID_3 |TASSEL_2|TACLR;  // Start with low speed, Continuous up mode , divide clock by 8, clock from SMCLK , clear timer

    P1DIR  = 0b00000111;    // BIT0-2 = OUTPUT, BIT3-5 = INPUT
    P1REN |= 0b00111000;    // Internal pull resisters for switch
    P1OUT |= 0b00111000;    // Select Pull-up 
    P1OUT |= 0b00000111;     // OFF all LEDs (active low) 

    while (1) {    
        if ((TACTL&TAIFG) == 1)  //Timer overflow
        {   
            TACTL &= ~TAIFG;    //Reset overflow flag (TAIFG = 11111110B)
            P1OUT ^= Motor;    //Toggle red LED by timer
        }               
        Switch_check ();        
    }
}

void Switch_check (void)
{             
        if (SW_Red == 0)
        {
            while (SW_Red == 0);
            TACTL = MC_2|ID_3 |TASSEL_2|TACLR;   //FAST SPEED
            P1OUT |= 0b00000111; //OFF all LEDs       
            P1OUT &= 0b11111110; //On LED red (Active Low)
        }
        if (SW_Yellow == 0)    //SW Yellow Push
        {
            while (SW_Yellow == 0);  //Wait SW Release 
            TACTL = MC_2|ID_2 |TASSEL_2|TACLR;     //MIDDLE SPEED
            P1OUT |= 0b00000111; //OFF all LEDs 
            P1OUT &= 0b11111101; //On LED Yellow (Active Low)           
        }
        if (SW_Green == 0)
        {
            while (SW_Green == 0);
            TACTL = MC_2|ID_1 |TASSEL_2|TACLR;   //LOW SPEED
            P1OUT |= 0b00000111; //OFF all LEDs 
            P1OUT &= 0b11111011; //On LED Green (Active Low)   
        }       
}
