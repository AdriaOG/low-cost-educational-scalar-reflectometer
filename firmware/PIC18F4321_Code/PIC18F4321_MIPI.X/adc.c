#include "adc.h"
#include "sio.h"
#include <stdio.h>

#define BUFFERSIZE 64

static int samplesRA0[BUFFERSIZE];
static int samplesRA1[BUFFERSIZE];
static char sampleIndex=0;
static long int averageRA0 = 0;
static long int averageRA1 = 0;

char toPrint2[32];

void initADC(void) {;
    ADCON1 = 0x0D;        // Configure RA0 and RA1 as analog, others as digital
    ADCON0 = 0x00;
    ADCON2bits.ADCS = 0b110; // ADC conversion clock (Fosc/64)
    ADCON2bits.ADFM = 1;  // Right justify result for 10-bit resolution
    ADCON2bits.ACQT = 0b111; // Acquisition time of 8 TAD
    ADCON0bits.ADON = 1;
    ADCON1bits.VCFG0 = 0; //We are using an internal voltage reference + (SEE TABLE 26-24)

    
    for (sampleIndex=BUFFERSIZE-1; sampleIndex!=0; sampleIndex--) samplesRA0[sampleIndex]= samplesRA1[sampleIndex] = 0;
}

unsigned char readADC(void) {
    ADCON0bits.GO = 1;       // Start ADC conversion
    while (ADCON0bits.GO);   // Wait for conversion to complete
    return ADRESH;           // Return 8-bit ADC result
}

static int avgResultRA1, avgResultRA0;
static char temp;
void MotorADC(void){
    static char state=0;
    
    switch(state){
        case 0:
            ADCON0bits.CHS =0;
            ADCON0bits.GO = 1;       // Start ADC conversion for channel 0
            state = 1;
            break;
        case 1:
            if (ADCON0bits.GO == 0){
                averageRA0 -= samplesRA0[sampleIndex];
                samplesRA0[sampleIndex] = (ADRESH << 8) | ADRESL;
                averageRA0 += samplesRA0[sampleIndex];      
                ADCON0bits.CHS =1;
                ADCON0bits.GO = 1;       // Start ADC conversion for channel 1
                state = 2;
            }
            break;
        case 2:
            //while(!SIOAvailableTX())SIPutChar(SIOAvailableRX()+'0');
            //if (SIOAvailableRX()==1){
            //        temp=RCREG;
                    //sprintf(toPrint2,"\n\rCurrent values:\n\r-> RA0: %d <-\n\r-> RA1: %d <-\n\r\0", samplesRA0[sampleIndex], samplesRA1[sampleIndex]);
                    //SIPuts(toPrint2);
            //}
            if (ADCON0bits.GO == 0){
                averageRA1 -= samplesRA1[sampleIndex];
                samplesRA1[sampleIndex] = (ADRESH << 8) | ADRESL;
                averageRA1 += samplesRA1[sampleIndex];      
                
                sampleIndex = (sampleIndex+1);
                if (sampleIndex == BUFFERSIZE) sampleIndex = 0;
                if (sampleIndex == 0)state = 3;
                else state = 0;
            }    
            break;
            
        case 3:
            avgResultRA0 = (averageRA0 >> 6) & 0x03FF;
            avgResultRA1 = (averageRA1 >> 6) & 0x03FF;
            sprintf(toPrint2,"\r-> RA0: %d --- RA1: %d \n\0", avgResultRA0, avgResultRA1);
            SIPuts(toPrint2);
            state = 0;
            break;
  
    }
    return;
}