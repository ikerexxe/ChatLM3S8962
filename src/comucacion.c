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
#include "hw_types.h"
#include "framebuffer.h"
#include "uartDrv.h"
#include "chat.h"
#include "sonido.h"
#include "display.h"
#include <stdlib.h>
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
tBoolean contRecibidos = false;

/*********************************************************************
** 																	**
** EXPORTED VARIABLES 												**
** 																	**
*********************************************************************/
extern int g_i_numero_elemento;

tBoolean contains_end(unsigned char * data);

/*********************************************************************
** 																	**
** LOCAL FUNCTIONS 													**
** 																	**
**********************************************************************/
//TODO: faltan las explicaciones
void CHAT_inicializacion_comunicacion(){
	openUART(puerto);
	datosRecibidos = malloc(sizeof(unsigned char)*MAX_ELEMS_LINEA);
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
	unsigned char * temporal = malloc(sizeof(unsigned char)*MAX_ELEMS_LINEA);
	int tamano = 0;
	tBoolean final = false;

	recvUART(puerto, temporal, &tamano);
	if(tamano > 0){
		final = contains_end(temporal);
		if(!contRecibidos){
			strcpy(datosRecibidos, temporal);
			contRecibidos = true;
		} else{
			strcat(datosRecibidos, temporal);
		}
		if(final){
			CHAT_reproducir_aviso();
			CHAT_refrescar_conversacion(TIPO_REMOTO, datosRecibidos);
			FRAME_BUFFER_write_to_display();
			datosRecibidos = malloc(sizeof(unsigned char)*MAX_ELEMS_LINEA);
			contRecibidos = false;
			CHAT_reproducir_silencio();
		}
	}
	/*
	while(temporal[contTemporal]!='\0'){
		if(temporal[contTemporal] == 36){
			datosRecibidos[contRecibidos] = '\0';
			CHAT_refrescar_conversacion(TIPO_REMOTO, datosRecibidos);
			FRAME_BUFFER_write_to_display();
			datosRecibidos = malloc(sizeof(unsigned char)*MAX_ELEMENTOS);
			contTemporal = 0;
			contRecibidos = 0;
			temporal[contTemporal] = '\0';
		} else{
			datosRecibidos[contRecibidos] = temporal[contTemporal];
			contRecibidos++;
			contTemporal++;
		}
	}
	*/
}

tBoolean contains_end(unsigned char * data){
	int contador = 0;
	tBoolean resultado = false;

	while(contador < MAX_ELEMS_PANTALLA){
		if(data[contador] == 36){
			data[contador] = '\0';
			resultado = true;
			contador = MAX_ELEMS_PANTALLA;
		} else if(!((data[contador] >= 48 && data[contador] <= 57) || (data[contador] >= 97 && data[contador] <= 122))){
			data[contador] = '\0';
			contador = MAX_ELEMS_PANTALLA;
		}
		contador++;
	}

	return resultado;
}
/*********************************************************************
** 																	**
** EOF 																**
** 																	**
**********************************************************************/
