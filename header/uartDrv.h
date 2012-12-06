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

#define BUFF_SIZE 128
#define BAUD_RATE 9600
#define DATA_FRAME (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_TWO |UART_CONFIG_PAR_EVEN)

int openUART(int nPort);
void closeUART(int nPort);
//TODO: yo diria que el receive sobra
int recvUART(int nPort, unsigned char *p, int *pSize);
int sendUART(int nPort, unsigned char *p, int *pSize);

unsigned char * getSwFIFO(int nPort);

#endif
