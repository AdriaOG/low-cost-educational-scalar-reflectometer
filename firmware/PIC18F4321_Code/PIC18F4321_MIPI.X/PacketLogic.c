#include <xc.h>
#include "TTimer.h"

unsigned char configuration;
unsigned char address;
unsigned char parity[2];
unsigned char data;


void setAddress(unsigned char addressName) {
    address = addressName;
}

unsigned char getAddress(void) {
    return data;
}

void setData(unsigned char newData) {
    data = newData;
}

unsigned char getData(void) {
    return data;
}

unsigned char calculateParity(unsigned char data, unsigned char initial) {
    unsigned char parity = initial;
    unsigned char i = 0;
    
    // Count the number of 1 bits in the data
    while (i < 8){
        parity ^= (data & 1);  // XOR the LSB with the parity bit
        data >>= 1;            // Shift the data to the right
        i++;
    }
    
    parity = ~parity & 1;  // Invert the parity bit (0->1, 1->0)
    parity <<= 7;
    return parity;  // Return the parity bit (0 or 1)
}

void usidFrame(unsigned char initialData) {
    while (5 > getTimerCounter()) {
        if (getHasBeenTimer()) {
            setBitData(configuration);
            configuration <<= 1;                //rotates the address left
            resetHasBeenTimer();
        }
    }
}

void dataFrame (unsigned char pointer, unsigned char counter, unsigned char infoSent){       //rotates the address left
    while (getTimerCounter() < counter) {
        if (getHasBeenTimer()) {
            setBitData(infoSent);
            infoSent <<= 1;                //rotates the address left
            resetHasBeenTimer();
        }
    }
    setBitData(parity[pointer]);
    resetHasBeenTimer();
}

void busPark() {
    unsigned char counter = 0;
    while (counter < 2) {           //Not sure if <3 or <2
        if (getHasBeenTimer()) {
            setBitData(0);
            resetHasBeenTimer();
            counter++;
        }
    }
    while (getInternalCounter() & 1);           //Not sure if <3 or <2
}

void SDATA(void) {
    configuration = 0x60;                           //The configuration must be put on the MSB, therefore slave 0110 will be 0x60
    //address = 0x40;                                //Register Write (010) + address (00000)
    setBitData(0x80);
    parity[0] = calculateParity(address, 0);          //Configuration 0x70 (0111), so we send a 1. If configuration = 0x60, we send a 0
    parity[1] = calculateParity(data, 0);             //Always 0 here
    //pwmStart();
    tiStart();
    
    while (getInternalCounter() < 1){                 //Initialization Sequence   
    }             
    setBitData(0);
    //resetHasBeenTimer();
    usidFrame(2);                               //AddressFrame for USID
     
    //First Frame
    dataFrame(0, 13, address);
    
    //Second Frame
    dataFrame(1, 22, data);
    //etc until end
    
    busPark();
    tiStop();
}