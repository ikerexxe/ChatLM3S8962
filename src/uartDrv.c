/*********************************************************************
** 																	**
** project : ChatLM3S8962	 										**
** filename : uartDrv.c 											**
** version : 1 														**
** date : 2012-12-05	 											**
** 																	**
**********************************************************************
** 																	**
** Copyright (c) 2012,		 										**
** All rights reserved. 											**
** 																	**
**********************************************************************
**																	**
**VERSION HISTORY:													**
**----------------													**
**Version : 1														**
**Date : 2012-12-05													**
**Revised by : iker pedrosa											**
**Description : Original version.									**
*********************************************************************/
#define UARTDRV_C

/*********************************************************************
**																	**
** MODULES USED 													**
** 																	**
**********************************************************************/
#include "uartDrv.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_ints.h"
#include "driverlib/uart.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "comunicacion.h"
#include "uartConfig.h"
#include <stdlib.h>

/*****************************************************************************
**	 																		**
** TYPEDEFS AND STRUCTURES 													**
** 																			**
*****************************************************************************/
typedef struct s_UARTCls
{
	//for reception
   int inHead, inTail;
   unsigned char inBuf[BUFF_SIZE];
   //for transmission
   int outHead, outTail;
   int  writingToBuf;
   int intWhileWriting;
   unsigned char outBuf[BUFF_SIZE];
}UARTClass;

/*********************************************************************
** 																	**
** GLOBAL VARIABLES 												**
** 																	**
**********************************************************************/
unsigned long uartsBases[]={UART0_BASE,UART1_BASE}; /*Identificadores de las UART*/
unsigned long uartsInts[]={INT_UART0,INT_UART1}; /*Interrupciones de las UART*/
/*Creamos los objetos de comunicaciones: UARTClass*/
UARTClass uarts[3]; /*Clase que contiene los datos de la UART*/

/*********************************************************************
** 																	**
** PROTOTYPES OF LOCAL FUNCTIONS 									**
** 																	**
*********************************************************************/
static int nElementosIn(int nPort);
static int nHuecosIn(int nPort);
static int nElementosOut(int nPort);
static int nHuecosOut(int nPort);
void UART0IntHandler(void);
static void fromHwFIFO(int nPort);
static int toHwFIFO(int nPort);
static void initUartPins(int nPort);
void UARTIntHandlerLogic(int nPort);
void  __attribute__((interrupt)) UART0IntHandler(void);
void  __attribute__((interrupt)) UART1IntHandler(void);

/**
 * @brief  Función para inicializar los PINS de la UART.
 *
 * @return    -
 *
 * Inicializa los PINs para poder usar la UART.
*/
static void initUartPins(int nPort)
{
  switch(nPort)
  {
    case 0: SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
			SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
			GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
			break;
	default:
			break;
  }
}

/**
 * @brief  Función para inicializar uno de los puertos UART.
 *
 * @return    -
 *
 * Inicializa el puerto 0 de la UART.
*/
int openUART(int nPort)
{
  initUartPins(nPort);
  UARTDisable(uartsBases[nPort]);
  UARTConfigSetExpClk(uartsBases[nPort], SysCtlClockGet(), BAUD_RATE, DATA_FRAME);
  UARTEnable(uartsBases[nPort]);
  uarts[nPort].inHead = uarts[nPort].inTail = uarts[nPort].inHead = uarts[nPort].inTail = uarts[nPort].intWhileWriting = uarts[nPort].writingToBuf=0;
  UARTFIFOLevelSet(uartsBases[nPort], UART_FIFO_TX1_8, UART_FIFO_RX1_8);
  IntEnable(uartsInts[nPort]);
  UARTIntEnable(uartsBases[nPort], UART_INT_TX | UART_INT_RX | UART_INT_RT);
  return 0;
}

/**
 * @brief  Función para cerrar uno de los puertos UART.
 *
 * @return    -
 *
 * Cierra el puerto 0 de la UART.
*/
void closeUART(int nPort)
{
  UARTDisable(uartsBases[nPort]);
  IntDisable(INT_UART0);
}

/**
 * @brief  Función para retorna los datos recibidos por la UART.
 *
 * @return    Exito de la recepcion
 *
 * Retorna los datos que se han almacenado en el buffer de software a la
 * logica del usuario.
*/
int recvUART(int nPort, unsigned char *p, int *pSize)
{
  int i,j,n;

  n=nElementosIn(nPort);
  //TODO: esta es la linea que igual hay que comentar
  //if(n>*pSize) n=*pSize;
  *pSize=n;
  j=uarts[nPort].inTail;
  for(i=0;i<n;i++)
  {
    p[i]=uarts[nPort].inBuf[j++];
    if(j==BUFF_SIZE) j=0;
  }
  uarts[nPort].inTail=j;
  return 1;
}

/**
 * @brief  Función para enviar datos por la UART.
 *
 * @return    Existo del envio
 *
 * Guarda los datos a enviar en el el buffer de software.
*/
int sendUART(int nPort, unsigned char *p, int *pSize)
{
  int i,j,n,retry;

  n=nHuecosOut(nPort);
  if(n>*pSize) n=*pSize;
  *pSize=n;
  j=uarts[nPort].outHead;
  for(i=0;i<n;i++)
  {
    uarts[nPort].outBuf[j++]=p[i];
    if(j==BUFF_SIZE) j=0;
  }
  uarts[nPort].outHead=j;
  do
  {
	uarts[nPort].writingToBuf=1;
	n=toHwFIFO(nPort);
	uarts[nPort].writingToBuf=0;
	if(uarts[nPort].intWhileWriting)
	{
	  uarts[nPort].intWhileWriting=0;
	  retry=uarts[nPort].outHead!=uarts[nPort].outTail;
	}
	else retry=0;
  }while(retry);
  return 1;
}

/**
 * @brief  Guarda los datos en el buffer del software.
 *
 * @return    -
 *
 * Guardado de los datos del buffer de hardware en el buffer de
 * software para su posterior tratamiento por la logica del usuario.
*/
static void fromHwFIFO(int nPort)
{
  int n;
  int i = 0;
  unsigned int l;

  n=nHuecosIn(nPort);
  while(UARTCharsAvail(uartsBases[nPort])&& (n-->0))
  {
    l=UARTCharGetNonBlocking(uartsBases[nPort]);
	uarts[nPort].inBuf[uarts[nPort].inHead]=(unsigned char)l;
	uarts[nPort].inHead++;
	i++;
	if(uarts[nPort].inHead==BUFF_SIZE) uarts[nPort].inHead=0;
  }
}

/**
 * @brief  Guarda los datos en el buffer del hardware.
 *
 * @return    -
 *
 * Guardado de los datos del buffer de software en el buffer de
 * hardware para su posterior envio.
*/
static int toHwFIFO(int nPort)
{
	unsigned char c;
	int n,m;

	n=nElementosOut(nPort);
	m=0;
	while((n-->0)&&(UARTSpaceAvail(uartsBases[nPort])))
	{
		c=uarts[nPort].outBuf[uarts[nPort].outTail];
		uarts[nPort].outTail++;
		if(uarts[nPort].outTail==BUFF_SIZE) uarts[nPort].outTail=0;
		UARTCharPutNonBlocking(uartsBases[nPort], c);
	}
	return m;
}

/**
 * @brief  Rutina de interrupcion.
 *
 * @return    -
 *
 * Rutina de interrupcion de la UART 0.
*/
void  __attribute__((interrupt)) UART0IntHandler(void)
{
	UARTIntHandlerLogic(0);
}

/**
 * @brief  Rutina de interrupcion.
 *
 * @return    -
 *
 * Rutina de interrupcion de la UART 1.
*/
void  __attribute__((interrupt)) UART1IntHandler(void)
{
	UARTIntHandlerLogic(1);
}

/**
 * @brief  Tratamiento de interrupciones.
 *
 * @return    -
 *
 * Tratamiento de las interrupciones de la UART.
*/
void UARTIntHandlerLogic(int nPort)
{
  unsigned long ulStatus;
#if(BLOCKING_DRV==1)
  static signed portBASE_TYPE xHigherPriorityTaskWoken=pdFALSE;
#endif

  ulStatus = UARTIntStatus(uartsBases[nPort], true);
  if(ulStatus & (UART_INT_RX | UART_INT_RT ))
  {
    fromHwFIFO(nPort);
#if(BLOCKING_DRV==1)
    xSemaphoreGiveFromISR(uarts[nPort].charAvailabilitySignal, &xHigherPriorityTaskWoken);
#endif
  }
  /*
  if(ulStatus & UART_INT_TX)
  {
	  if(uarts[nPort].outHead!=uarts[nPort].outTail)
	  {
 	    if (!uarts[nPort].writingToBuf) toHwFIFO(nPort);
 	    else uarts[nPort].intWhileWriting=1;
	  }
	  UARTIntClear(uartsBases[nPort], UART_INT_TX);
  }
  */
  UARTIntClear(uartsBases[nPort], ulStatus);

  //TODO: va aqui fijo?
  CHAT_recibir();
}

/**
 * @brief  Numero de huecos libres en el buffer del hardware de entrada.
 *
 * @return    Numero de elementos
 *
*/
static int nHuecosIn(int nPort)
{
	return (uarts[nPort].inTail+BUFF_SIZE-uarts[nPort].inHead-1) % BUFF_SIZE;
}

/**
 * @brief  Numero de elementos en el buffer del hardware de entrada.
 *
 * @return    Numero de elementos
 *
*/
static int nElementosIn(int nPort)
{
	return (uarts[nPort].inHead+BUFF_SIZE-uarts[nPort].inTail) % BUFF_SIZE;
}

/**
 * @brief  Numero de huecos libres en el buffer del hardware de salida.
 *
 * @return    Numero de huecos libres
 *
*/
static int nHuecosOut(int nPort)
{
	return (uarts[nPort].outTail+BUFF_SIZE-uarts[nPort].outHead-1) % BUFF_SIZE;
}

/**
 * @brief  Numero de elementos en el buffer del hardware de salida.
 *
 * @return    Numero de elementos
 *
*/
static int nElementosOut(int nPort)
{
	return (uarts[nPort].outHead+BUFF_SIZE-uarts[nPort].outTail) % BUFF_SIZE;
}
/*********************************************************************
** 																	**
** EOF 																**
** 																	**
**********************************************************************/
