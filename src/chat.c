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
unsigned char g_note = 0; /*Numero de nota*/
extern unsigned long g_ul_keypad_switches; /*Valor le�do en los botones*/
unsigned long g_ul_system_clock; /*Frecuencia del clock*/
unsigned long g_us_note_frecuency = FRECUENCIA_SILENCIO; /*Frecuencia de las notas*/
tBoolean g_b_enviar = false; //TODO: comentar a partir de aqui
unsigned long g_ul_keypad_switches = 0x1f; /*Valor le�do en los botones*/
unsigned char g_ucChangedData = 0x00; //si se ha cambiado la tecla que se esta pulsando
unsigned char * g_frase;

/*********************************************************************
** 																	**
** LOCAL FUNCTIONS 													**
** 																	**
**********************************************************************/
int main(void){
	/*
	 * Inicializamos los componentes y perif�ricos necesarios
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
 * @brief  Funci�n para inicializaci�n de los perif�ricos.
 *
 * @return    -
 *
 * Se inicializan uno a uno el clock, el PWM, la pantalla, los botones
 * y el led.
*/
//TODO: falta la explicacion con todos los perifericos
void CHAT_inicializacion(){
	CHAT_inicializacion_clock();
	CHAT_inicializacion_keypad();
	CHAT_inicializacion_display();
	//CHAT_inicializacion_pwm();
	CHAT_inicializacion_comunicacion();
}

/**
 * @brief  Funci�n para elegir el paso a seguir en cada momento.
 *
 * @return    -
 *
 * Primero se lee la tecla pulsada, luego se elige la nota seg�n la tecla
 * que se haya pulsado y luego, se reproduce la nota y se dibuja en la
 * pantalla.
*/
//TODO: falta la explicarla bien
void CHAT_logica(){
	CHAT_leer_keypad();
	CHAT_logica_teclas();

	if(g_b_enviar){
		CHAT_enviar(g_frase);
		CHAT_reinicializacion_usuario(TIPO_USUARIO, NULL);
		g_b_enviar = false;
	}

	//CHAT_recibir();
}
/*********************************************************************
** 																	**
** EOF 																**
** 																	**
**********************************************************************/
