/*********************************************************************
** 																	**
** project : ChatLM3S8962	 										**
** filename : uart.c 												**
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
#define COMUNICACION_C
/*********************************************************************
**																	**
** MODULES USED 													**
** 																	**
**********************************************************************/
#include "framebuffer.h"
#include "uartDrv.h"
#include <string.h>

/*********************************************************************
** 																	**
** GLOBAL VARIABLES 												**
** 																	**
**********************************************************************/
int puerto = 0;
int y = 15;

/*********************************************************************
** 																	**
** EXPORTED VARIABLES 												**
** 																	**
*********************************************************************/


/*********************************************************************
** 																	**
** LOCAL FUNCTIONS 													**
** 																	**
**********************************************************************/
//TODO: faltan las explicaciones
void CHAT_inicializacion_comunicacion(){
	openUART(puerto);
}

void CHAT_ciere_comunicacion(){
	closeUART(puerto);
}

void CHAT_enviar(unsigned char * datos){
	int tamano;

	tamano=strlen(datos);

	sendUART(puerto, datos, &tamano);
}

void CHAT_recibir(){
	unsigned char * datos;

	//TODO: a revisar el parametro de entrada
	datos = getSwFIFO(puerto);
	//TODO: a revisar x e y
	FRAME_BUFFER_insert_text(datos,0,y);
	y+=10;
	FRAME_BUFFER_write_to_display();
}
/*********************************************************************
** 																	**
** EOF 																**
** 																	**
**********************************************************************/
