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
** DEFINITIONS AND MACROS 											**
** 																	**
**********************************************************************/
#define MAX_RECIBIDOS 10

/*********************************************************************
** 																	**
** GLOBAL VARIABLES 												**
** 																	**
**********************************************************************/
int puerto = 0;
int y = 15;
unsigned char * datosRecibidos;
int contRecibidos = 0;

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
	datosRecibidos = malloc(sizeof(unsigned char)*MAX_RECIBIDOS);
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
	int contTemporal = 0;
	unsigned char * temporal;

	//TODO: a revisar el parametro de entrada
	temporal = getSwFIFO(puerto);

	while(temporal[contTemporal]!='\0'){
		//TODO: a revisar x e y
		if(temporal[contTemporal] == 36){
			datosRecibidos[contRecibidos] = '\0';
			FRAME_BUFFER_insert_text(datosRecibidos,0,y);
			y+=10;
			FRAME_BUFFER_write_to_display();
			contTemporal = 0;
			contRecibidos = 0;
		} else{
			datosRecibidos[contRecibidos] = temporal[contTemporal];
			contRecibidos++;
		}

		contTemporal++;
	}
}
/*********************************************************************
** 																	**
** EOF 																**
** 																	**
**********************************************************************/
