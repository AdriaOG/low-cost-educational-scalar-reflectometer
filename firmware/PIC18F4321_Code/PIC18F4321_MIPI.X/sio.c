#include <pic18f4321.h>

#include "sio.h"

void initEUSART(void) {
    TRISCbits.TRISC6 = 0;  // Set TX pin as output
    TRISCbits.TRISC7 = 1;  // Set RX pin as input
    SPBRG = 51;           // Set baud rate to 9600 
                          // SPBRG: 64 (40 MHz oscillator)
                          // SPBRG: 51 (32 MHz oscillator)
    BAUDCONbits.BRG16 = 0;
    TXSTAbits.SYNC = 0;
    TXSTAbits.BRGH = 0;    // High-Speed Baud Rate Select
    TXSTAbits.TXEN = 1;    // Enable transmitter
    RCSTAbits.SPEN = 1;    // Enable EUSART
    RCSTAbits.CREN = 1;    // Enable continuous receive mode
}

char SIOAvailableRX(void){
    return PIR1bits.RCIF;
}

char SIOAvailableTX(void){
    return TXSTAbits.TRMT;
}

void SIPutChar(char value){
    TXREG = value;
}

char SIGetChar(void){
    return RCREG;
}

void SIPuts(char *value){
    while (*value){
        while (!SIOAvailableTX());
        SIPutChar(*value++);
    }
}