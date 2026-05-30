#ifndef PACKETLOGIC_H
#define	PACKETLOGIC_H
#include "TTimer.h"

void setData(unsigned char newData);

void setAddress(unsigned char newAddress);

unsigned char getData(void);

void SDATA(void);


#endif	/* PACKETLOGIC_H */