/* Main.c file generated by New Project wizard
 *
 * Created:   Mon Apr 7 2025
 * Processor: MSP430F2011
 * Compiler:  GCC for MSP430
 */

#include <msp430.h>

#define LED_Red    	BIT0
#define LED_Yellow  	BIT1
#define LED_Green   	BIT2
#define SW_Yellow      	BIT3
#define SW_Red  		BIT4
#define SW_Green    	BIT5
#define Motor       		BIT6

void Switch_check (void);

void main (void)
{
    WDTCTL = WDTPW | WDTHOLD;           // Watchdog Password and Hold for dubug mode
    TACTL = MC_2|ID_2 |TASSEL_2|TACLR;  // Start with low speed, Continuous up mode , divide clock by 8, clock from SMCLK , clear timer

    P1DIR   = 0b01000111;    // BIT0-2 = OUTPUT, BIT3-5 = INPUT
    //P1REN |= 0b00111000;    // Internal pull resisters for switch
    ///P1OUT |= 0b00111000;    // Select Pull-up 
    P1OUT |= 0b01000111;     // OFF all LEDs (active low)  
  
    while (1) {    
           if ((TACTL&TAIFG) == 1)  //Timer overflow
           {   
           TACTL &= ~TAIFG;    //Reset overflow flag (TAIFG = 11111110B)
           P1OUT ^= Motor;    //Toggle red LED by timer	   
           }               
      // Switch_check ();        
    }
}

void Switch_check (void)
{             
        if ((P1IN&SW_Red) == 0)
        {
            while ((P1IN&SW_Red) == 0);
            //TACTL = MC_2|ID_3 |TASSEL_2|TACLR;   //FAST SPEED
            //P1OUT |= 0b00000111; //OFF all LEDs       
            //P1OUT &= ~LED_Red; //On LED red (Active Low)
	   P1OUT &=~LED_Red ;
        }
	
        if ((P1IN&SW_Yellow) == 0)    //SW Yellow Push
        {
            while ((P1IN&SW_Yellow) == 0);  //Wait SW Release 
            //TACTL = MC_2|ID_2 |TASSEL_2|TACLR;     //MIDDLE SPEED
            //P1OUT |= 0b00000111; //OFF all LEDs 
            P1OUT &= ~LED_Yellow; //On LED Yellow (Active Low)           
        }
        if ((P1IN & SW_Green) == 0)
        {             
           while ((P1IN & SW_Green) == 0);
	   //TACTL = MC_2|ID_1 |TASSEL_2|TACLR;   //LOW SPEED
            //P1OUT |= 0b00000111; //OFF all LEDs 
            P1OUT &= ~LED_Green; //On LED Green (Active Low)   
        }       
}