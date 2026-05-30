#ifndef _SIO_H_
#define _SIO_H_

void initEUSART(void);

char SIOAvailableRX(void);

char SIOAvailableTX(void);

void SIPutChar(char);

char SIGetChar(void);

void SIPuts(char *);



#endif