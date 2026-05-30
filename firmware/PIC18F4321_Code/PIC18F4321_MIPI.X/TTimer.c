#include <xc.h>
#include "TTimer.h"

/*
 * 
 * Timer 0 must interrupt every 15 uS - 12 instructions!
 * 
 *  For HSPLL  (100ns)  0x76
 *  For INTIO  (125nS)  148
 */

#define VALUE_TIMER0 148

unsigned char bitData = 0;
unsigned int counter = 0;
unsigned char trashwait = 0;
unsigned char auxVar = 0;
unsigned char hasBeenTimer = 0;
unsigned int internalCounter = 0;
unsigned char newLATA = 0;


void setBitData (unsigned char data) {
    bitData = data;
}

unsigned int getInternalCounter(void) {
    return internalCounter;
}

unsigned int getTimerCounter(void) {
    return counter;
}

void resetTimerCounter(void) {
    counter = 0;
    internalCounter = 0;
}

unsigned char getHasBeenTimer(void) {
    return hasBeenTimer;
}

void resetHasBeenTimer(void) {
    hasBeenTimer = 0;
}

void tiStart(void) {
    resetHasBeenTimer();
    //TMR0H = 0xFD;          // Set high byte of Timer0 period (calculated to overflow after 30 µs)
    //TMR0L = 0xE8; 
    TMR0 = VALUE_TIMER0;
    newLATA = 0;
    T0CONbits.TMR0ON = 1;
}

void tiStop(void) {
    T0CONbits.TMR0ON = 0;
    resetTimerCounter();
}

void TiInitTimer(void) {
    // Timer0 Configuration
    T0CONbits.TMR0ON = 1;
    T0CONbits.T08BIT = 1;
    T0CONbits.PSA = 1;
    T0CONbits.T0CS = 0;    // Select internal instruction cycle as the clock source for the timer
    
// Set high byte of Timer0 period (calculated to overflow after 30 µs)         
/* 22.07.2025: JNM: 
 * WRONG! According to the report, it should interrupt every 15 uS. I have seen that with HSPLL, TMR0L is set to 0x76 (118 in decimal)
 *    which is 138 steps of timer -> 138 * 100 nS = 13.8 uS  --> There are 12 instructions missing (probably the instructions from the RSI)
 * 
 */
    TMR0 = VALUE_TIMER0;
    
    INTCONbits.TMR0IF = 0;
    INTCONbits.TMR0IE = 1;
    INTCONbits.PEIE = 1; // Enable peripheral interrupts
    INTCONbits.GIE = 1; // Enable global interrupts
    counter = 0;
    internalCounter = 0;
    tiStop();
}

void _TiRSITimer(void) {
    INTCONbits.TMR0IF = 0; // Clear the interrupt flag
//Timer Interrupt Service Routine
    //@ 40MHz (Tinst = 100nS), we want 1ms/Tinst=10.000 tics 2*16-10.000=0xD8F0
    //TMR0H = 0xEC;
    //TMR0L = 0x78;
   // TMR0H = 0xEF;//Timer at 8300, DF94 EFCA
    //TMR0 = 0x10;            // Reload Timer0 with preload value
    //TMR0H = 0xFD;          // Set high byte of Timer0 period (calculated to overflow after 30 µs)
    //TMR0L = 0xE8; 
    
    TMR0 = VALUE_TIMER0;
    internalCounter++;
    auxVar = 0;
    auxVar = ((bitData & 0x80) >> 6);
    if (internalCounter > 2) {
        newLATA ^= 0x01;
        if (!(internalCounter & 1)) {
            trashwait = 23;
            trashwait = 23;
        }
    }
    else {
        newLATA = auxVar;
        if (!(internalCounter & 1)) {
            trashwait = 23;
            trashwait = 23;
            trashwait = 0;
        }
    }
    if (internalCounter & 1) {
        newLATA &= (0xFD);
        newLATA |= auxVar;                // Set LATA5 based on bitData
        counter++;                  //an entire duty cycle will have happened
        hasBeenTimer++;
    }
    LATD = newLATA;
    /*
    if ((internalCounter > 5) && !(internalCounter & 1)) {
        newLATA = 0;
        newLATA |= (LATD & 0x01) ^ 0x01;                           // Set the toggled LATA4
        newLATA |= ((bitData & 0x80) >> 6);                // Set LATA5 based on bitData
        LATD = newLATA;
        counter++;                  //an entire duty cycle will have happened
        hasBeenTimer++;
    }
    else if (internalCounter > 5) {
        newLATA = 0;
        newLATA = 0;
        newLATA = 0;
        newLATA = 0;
        LATDbits.LATD0 ^= 1;
    }
    else if (!(internalCounter & 1)) {
        LATDbits.LATD1 ^= 1;
        counter++;                  //an entire duty cycle will have happened
        hasBeenTimer++;
    }*/
}

