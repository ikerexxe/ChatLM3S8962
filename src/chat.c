/*********************************************************************
** 																	**
** project : Chat			 										**
** filename : main.c 												**
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
**Version : 2														**
**Date : 2012-12-05													**
**Revised by : iker pedrosa											**
**Description : Original version.									**
*********************************************************************/

/*********************************************************************
**																	**
** MODULES USED 													**
** 																	**
**********************************************************************/
#include "chat.h"
#include "framebuffer.h"
#include "clock.h"
#include "keypad.h"
#include "displayGenerico.h"
#include "display.h"
#include "sonido.h"
#include "comunicacion.h"
#include "hw_types.h"

/*********************************************************************
** 																	**
** PROTOTYPES OF LOCAL FUNCTIONS 									**
** 																	**
*********************************************************************/
void CHAT_inicializacion();
void CHAT_logica();

/*********************************************************************
** 																	**
** GLOBAL VARIABLES 												**
** 																	**
**********************************************************************/
unsigned long g_ul_system_clock; /*Frecuencia del clock*/
tBoolean g_b_enviar = false; /*Si hay mensaje para enviar*/
unsigned char * g_frase; /*Lo escrito por el usuario de la stellaris*/

/*********************************************************************
** 																	**
** LOCAL FUNCTIONS 													**
** 																	**
**********************************************************************/
int main(void){
	/*
	 * Inicializamos los componentes y periféricos necesarios
	 */
	CHAT_inicializacion();

	/*
	 * Superloop.
	 */
    while(1){
    	CHAT_logica();
    }
}

/**
 * @brief  Función para inicialización de los periféricos.
 *
 * @return    -
 *
 * Se inicializan uno a uno el clock, el keypad, la pantalla, el PWM y
 * la UART.
*/
void CHAT_inicializacion(){
	CHAT_inicializacion_clock();
	CHAT_inicializacion_keypad();
	CHAT_inicializacion_display();
	CHAT_inicializacion_pwm();
	CHAT_inicializacion_comunicacion();
}

/**
 * @brief  Función para elegir el paso a seguir en cada momento.
 *
 * @return    -
 *
 * Primero se lee la tecla pulsada, luego se actualiza la pantalla teniendo
 * en cuenta la tecla pulsa, si se pulsa el select se envía el mensaje y se
 * reinicializa la parte de la pantalla en la que escribe el usuario.
 * Si se ha reibido algo por la UART y está almacenado en el buffer de software
 * se muestra en pantalla.
*/
void CHAT_logica(){
	CHAT_leer_keypad();
	CHAT_logica_teclas();

	if(g_b_enviar){
		CHAT_enviar(g_frase);
		CHAT_reinicializacion_usuario(TIPO_USUARIO, NULL);
		g_b_enviar = false;
	}

	CHAT_recibir();
}
/*********************************************************************
** 																	**
** EOF 																**
** 																	**
**********************************************************************/
