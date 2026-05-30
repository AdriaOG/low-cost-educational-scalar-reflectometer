#ifndef TTIMER_H
#define	TTIMER_H
#define FALSE 0
#define TRUE 1

void setBitData(unsigned char data);

unsigned int getInternalCounter(void);

unsigned int getTimerCounter(void);

void resetTimerCounter(void);

unsigned char getHasBeenTimer(void);

void resetHasBeenTimer(void);

void tiStart(void);

void tiStop(void);

void pwmStart(void);

void pwmStop(void);

void PwmSCLKCOnfig(void);

void TiInitTimer(void);
//Pre: --
//Post: Initializes the timer0 to interrupt each 1ms.

void  _TiRSITimer(void);


#endif	/* TTIMER_H */
