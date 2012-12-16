#ifndef UARTDRV_H
#define UARTDRV_H

#include "uartDrv.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_ints.h"
#include "driverlib/uart.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "comunicacion.h"
#include <stdlib.h>

int openUART(int nPort);
void closeUART(int nPort);
int recvUART(int nPort, unsigned char *p, int *pSize);
int sendUART(int nPort, unsigned char *p, int *pSize);

unsigned char * getSwFIFO(int nPort);

#endif
