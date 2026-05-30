#include <xc.h>
#include "TTimer.h"
#include "PacketLogic.h"
#include "sio.h"
#include "adc.h"

#pragma config OSC = INTIO2
#pragma config PBADEN = DIG
#pragma config WDT = OFF
#pragma config LVP = OFF

void HighInterruptRSI(void);
void main(void);

unsigned char newConfiguration;
unsigned char oldConfiguration;
unsigned char oldAdc;

const unsigned char lookUpTable[30] = { 
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 
    0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0xF0, 0xF1, 0xE2, 0xE3, 0xB4, 
    0xB1, 0x96, 0x97, 0x78, 0x71, 0x5A, 0x53, 0x3C, 0x35, 0x1E 
};

void __interrupt() RSI_High(void){
	_TiRSITimer();
}




void newConfigEusart(unsigned char receivedData) {
    while (!TXSTAbits.TRMT);
    TXREG = receivedData;
    switch (receivedData) {
        case '0':
            newConfiguration = 0x00;
            break;
        case '1':
            newConfiguration = 0x01;
            break;
        case '2':
            newConfiguration = 0x02;
            break;
        case '3':
            newConfiguration = 0x03;
            break;
        case '4':
            newConfiguration = 0x04;
            break;
        case '5':
            newConfiguration = 0x05;
            break;
        case '6':
            newConfiguration = 0x06;
            break;
        case '7':
            newConfiguration = 0x07;
            break;
        case '8':
            newConfiguration = 0x08;
            break;
        case 'A':
            newConfiguration = 0x05;
            break;
        case 'B':
            newConfiguration = 0x0B;
            break;
        case 'C':
            newConfiguration = 0x0E;
            break;
        case 'D':
            newConfiguration = 0x08;
            break;
    }
}

void newConfigADC(unsigned char receivedData) {
    if ((receivedData > oldAdc && (receivedData - oldAdc > 10)) || 
    (receivedData < oldAdc && (oldAdc - receivedData > 10))) {
        oldAdc = receivedData;
        if (receivedData < 32) {
            newConfiguration = 0x00;
        } else if (receivedData < 64) {
            newConfiguration = 0x01;
        } else if (receivedData < 96) {
            newConfiguration = 0x02;
        } else if (receivedData < 128) {
            newConfiguration = 0x03;
        } else if (receivedData < 160) {
            newConfiguration = 0x04;
        } else if (receivedData < 192) {
            newConfiguration = 0x05;
        } else if (receivedData < 224) {
            newConfiguration = 0x06;
        } else {
            newConfiguration = 0x07;
        }
    }
}

void main(void){
    ADCON1 = 0xFF;
    TRISA = 0xFF;
    TRISD = 0xFF;
    OSCCONbits.IRCF = 7;  //Internal oscillator @ 8MHz
    OSCTUNEbits.PLLEN=1;  //Fosc *=4; Final frequency 32 MHz
    TiInitTimer();
    initEUSART();
    tiStart();
    initADC();
    
    SIPuts("\n\rWelcome to Chamaleon! \n\r\0");
    
    
    
    unsigned char valor = 0;
    
    while (1){
        MotorADC();
        
        /*
         * if (getInternalCounter() > 33333){
            LATA = valor;
            valor = 255-valor;
            resetTimerCounter();
        }*/
    }
}

void main_old(void){
    ADCON1 = 0x0D;
    TRISA = 0x01;
    TRISB = 0xFF;
    TRISC = 0x00;
    TRISD = 0x00;
    INTCON2 = 0X00;
    LATDbits.LATD0 = 0;
    LATDbits.LATD1 = 0;
    LATAbits.LATA3 = 0;
    LATAbits.LATA4 = 0;
    newConfiguration = 0x0;
    oldConfiguration = 0x0;
    oldAdc = 0;
    unsigned int data = 0;
    unsigned char receivedData;
    
    // Initialize EUSART, ADC and TIMER
    initEUSART();
    initADC();
    TiInitTimer();
 
    //Initialization
    setData(0x0);
    setAddress(0x5C);
    SDATA();
    while (data < 10000) {
            data++;
    }
    data = 0;
    while(1){
         // Check if a byte is received
        if (PIR1bits.RCIF) { // RCIF is the EUSART Receive Interrupt Flag
            receivedData = RCREG; // Read the received byte
            newConfigEusart(receivedData);
        } /*else {
            // Perform ADC Conversion
            receivedData = readADC();
            newConfigADC(receivedData);
        }*/
        /*SDATA();
        while (data < 10000) {
            data++;
        }
        data = 0;*/
        
        if (newConfiguration != oldConfiguration) {
            oldConfiguration = newConfiguration;
            setAddress(0x40);
            setData(newConfiguration);
            resetHasBeenTimer();
            SDATA();
            setAddress(0x5C);
            setData(0x07);
            SDATA();
           /* while (data < 65000) {
                data++;
            }
            data = 0;*/
        }
    }		
}