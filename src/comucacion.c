/*********************************************************************
** 																	**
** project : ChatLM3S8962	 										**
** filename : comunicacion.c 										**
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
int g_i_puerto = 0; /*Puerto que se usa para la comunicacion*/
unsigned char * datosRecibidos; /*Datos recibidos por la UART*/
tBoolean recibido = false; /*Si datosRecibidos contiene datos*/

/*********************************************************************
** 																	**
** PROTOTYPES OF LOCAL FUNCTIONS 									**
** 																	**
*********************************************************************/
tBoolean contains_end(unsigned char * data);

/*********************************************************************
** 																	**
** LOCAL FUNCTIONS 													**
** 																	**
**********************************************************************/

/**
 * @brief  Función para inicializar la comunicación.
 *
 * @return    -
 *
 * Se abre el puerto de comunicaciones de la UART.
*/
void CHAT_inicializacion_comunicacion(){
	openUART(g_i_puerto);
	datosRecibidos = malloc(sizeof(unsigned char)*MAX_ELEMS_LINEA);
}

/**
 * @brief  Función para finalizar la comunicación.
 *
 * @return    -
 *
 * Se cierra el puerto de comunicaciones de la UART.
*/
void CHAT_ciere_comunicacion(){
	closeUART(g_i_puerto);
}

/**
 * @brief  Función para enviar los datos.
 *
 * @return    -
 *
 * Se envian los datos usando la UART.
*/
void CHAT_enviar(unsigned char * datos){
	int tamano; //Tamaño del texto a enviar

	tamano=strlen(datos);

	sendUART(g_i_puerto, datos, &tamano);
}

/**
 * @brief  Función para la recepcion de datos.
 *
 * @return    -
 *
 * Se tratan los datos recibidos usando la UART.
*/
void CHAT_recibir(){
	unsigned char * temporal = malloc(sizeof(unsigned char)*MAX_ELEMS_LINEA); //String temporal donde se guarda lo recibido
	int tamano = 0; //Tamaño del texto recibido
	tBoolean final = false; //Si se ha recibido el token final

	recvUART(g_i_puerto, temporal, &tamano);
	if(tamano > 0){
		final = contains_end(temporal);
		if(!recibido){
			strcpy(datosRecibidos, temporal);
			recibido = true;
		} else{
			strcat(datosRecibidos, temporal);
		}
		if(final){
			//TODO: quitar los reproducir de comentarios
			//CHAT_reproducir_aviso();
			CHAT_refrescar_conversacion(TIPO_REMOTO, datosRecibidos);
			FRAME_BUFFER_write_to_display();
			datosRecibidos = malloc(sizeof(unsigned char)*MAX_ELEMS_LINEA);
			recibido = false;
			//CHAT_reproducir_silencio();
		}
	} else{
		free(temporal);
	}
}

/**
 * @brief  Función que mira si se ha terminado de transmitir.
 *
 * @return    -
 *
 * Se mira si se ha recibido el caracter de fin de conversacion.
*/
tBoolean contains_end(unsigned char * data){
	int contador = 0; //Contador para recorrer el string
	tBoolean resultado = false; //Si se ha recibido el token final

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
