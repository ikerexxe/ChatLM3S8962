#include "uartDrv.h"

typedef struct s_UARTCls
{
	//for reception
   int inHead, inTail;
   unsigned char inBuf[BUFF_SIZE];
   //for transmission
   int outHead, outTail;
   unsigned char outBuf[BUFF_SIZE];

}UARTClass;

unsigned long uartsBases[]={UART0_BASE,UART1_BASE};
unsigned long uartsInts[]={INT_UART0,INT_UART1};
/*Creamos los objetos de comunicaciones: UARTClass*/
UARTClass uarts[3];

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

int openUART(int nPort){
	initUartPins(nPort);
	UARTDisable(uartsBases[nPort]);
	UARTConfigSetExpClk(uartsBases[nPort], SysCtlClockGet(), BAUD_RATE, DATA_FRAME);
	UARTEnable(uartsBases[nPort]);
	uarts[nPort].inHead = uarts[nPort].inTail = uarts[nPort].inHead = uarts[nPort].inTail;
	UARTFIFOLevelSet(uartsBases[nPort], UART_FIFO_TX1_8, UART_FIFO_RX1_8);
	IntEnable(uartsInts[nPort]);
	UARTIntEnable(uartsBases[nPort], UART_INT_TX | UART_INT_RX | UART_INT_RT);

	return 0;
}

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

void closeUART(int nPort){
	UARTDisable(uartsBases[nPort]);
	if(nPort==0){
		IntDisable(INT_UART0);
	} else{
		IntDisable(INT_UART1);
	}
}

int recvUART(int nPort, unsigned char *p, int *pSize)
{
	int i,j,n;

	n=nElementosIn(nPort);
	if(n>*pSize){
		n=*pSize;
	}
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

int sendUART(int nPort, unsigned char *p, int *pSize)
{
	int i,j,n,retry;

	n=nHuecosOut(nPort);
	if(n>*pSize){
		n=*pSize;
	}
	*pSize=n;
	j=uarts[nPort].outHead;
	for(i=0;i<n;i++)
	{
		uarts[nPort].outBuf[j++]=p[i];
		if(j==BUFF_SIZE) j=0;
	}
	uarts[nPort].outHead=j;
	n=toHwFIFO(nPort);
	return 1;
}

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

	uarts[nPort].inBuf[uarts[nPort].inHead]=(unsigned char)i+48;
	uarts[nPort].inHead++;
}

unsigned char * getSwFIFO(nPort){
	int i = 0;
	unsigned char * data = (unsigned char *) malloc(sizeof(unsigned char)*(uarts[nPort].inHead-uarts[nPort].inTail));

	while(uarts[nPort].inHead!=uarts[nPort].inTail){
		data[i] = uarts[nPort].inBuf[uarts[nPort].inTail];
		uarts[nPort].inTail++;
		if(uarts[nPort].inTail==BUFF_SIZE) uarts[nPort].inTail=1;
		i++;
	}

	data[i] = "\0";

	return data;
}

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

void  __attribute__((interrupt)) UART0IntHandler(void)
{
	UARTIntHandlerLogic(0);
	CHAT_recibir();
}

void  __attribute__((interrupt)) UART1IntHandler(void)
{
	UARTIntHandlerLogic(1);
}

void UARTIntHandlerLogic(int nPort)
{
	unsigned long ulStatus;

	ulStatus = UARTIntStatus(uartsBases[nPort], true);
	if(ulStatus & (UART_INT_RX | UART_INT_RT ))
	{
		fromHwFIFO(nPort);
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
}

static int nHuecosIn(int nPort)
{
	return (uarts[nPort].inTail+BUFF_SIZE-uarts[nPort].inHead-1) % BUFF_SIZE;
}

static int nElementosIn(int nPort)
{
	return (uarts[nPort].inHead+BUFF_SIZE-uarts[nPort].inTail) % BUFF_SIZE;
}

static int nHuecosOut(int nPort)
{
	return (uarts[nPort].outTail+BUFF_SIZE-uarts[nPort].outHead-1) % BUFF_SIZE;
}

static int nElementosOut(int nPort)
{
	return (uarts[nPort].outHead+BUFF_SIZE-uarts[nPort].outTail) % BUFF_SIZE;
}
