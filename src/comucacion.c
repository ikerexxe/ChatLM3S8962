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
#include "chat.h"
#include <string.h>

/*********************************************************************
** 																	**
** GLOBAL VARIABLES 												**
** 																	**
**********************************************************************/
int puerto = 0;
int g_i_width = 0;
int g_i_height = 15;
unsigned char * datosRecibidos;
int contRecibidos = 0;

/*********************************************************************
** 																	**
** EXPORTED VARIABLES 												**
** 																	**
*********************************************************************/
extern int g_i_numero_elemento;

/*********************************************************************
** 																	**
** LOCAL FUNCTIONS 													**
** 																	**
**********************************************************************/
//TODO: faltan las explicaciones
void CHAT_inicializacion_comunicacion(){
	openUART(puerto);
	datosRecibidos = malloc(sizeof(unsigned char)*MAX_ELEMENTOS);
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
	int x = 0;
	int y = 0;
	unsigned char * temporal;
	int * recibido;
	int a;
	int actualizador = 0;

	(*recibido) = 0;

	//TODO: revisar la funcion recvUART pos si sirve mas que esta
	recvUART(puerto, temporal, recibido);
	if((*recibido)){
		while(temporal[contTemporal]!='\0'){
			//TODO: a revisar x e y
			if(temporal[contTemporal] == 36){
				datosRecibidos[contRecibidos] = '\0';
				if(g_i_height < 65){
					FRAME_BUFFER_insert_text(datosRecibidos, g_i_width, g_i_height);
					g_i_height += 10;
				} else{
					FRAME_BUFFER_delete_element(actualizador);
					actualizador = 1;
					y = 15;
					//TODO: puede que no sea asi sino -1
					while(actualizador < MAX_ELEMS ){
						if(actualizador!=g_i_numero_elemento){
							FRAME_BUFFER_actualiza_posicion_elemento(actualizador, x, y);
						}
						y += 10;
						actualizador++;
					}
				}
				FRAME_BUFFER_write_to_display();
				contTemporal = 0;
				contRecibidos = 0;
				temporal[contTemporal] = '\0';
			} else{
				datosRecibidos[contRecibidos] = temporal[contTemporal];
				contRecibidos++;
				contTemporal++;
			}
		}
	}
}
/*********************************************************************
** 																	**
** EOF 																**
** 																	**
**********************************************************************/
