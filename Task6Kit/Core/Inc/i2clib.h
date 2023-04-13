#ifndef __I2CLIB_H
#define __I2CLIB_H

#include "main.h"

void turnOff(void);

void turnOn(void);

void setFreq(uint16_t PWMFreq);

void setDuty(uint16_t dutyPercent);

#endif
