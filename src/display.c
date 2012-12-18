/*********************************************************************
** 																	**
** project : ChatLM3S8962	 										**
** filename : display.c												**
** version : 1 														**
** date : 2012-10-08	 											**
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
**Description : Modified version for the chat.						**
*********************************************************************/

/*********************************************************************
**																	**
** MODULES USED 													**
** 																	**
**********************************************************************/
#include "rit128x96x4.h"
#include "chat.h"
#include "display.h"
#include "displayGenerico.h"
#include "framebuffer.h"
#include "hw_types.h"
#include <stdlib.h>
/*********************************************************************
** 																	**
** DEFINITIONS AND MACROS 											**
** 																	**
**********************************************************************/
#define DISPLAY_C

/*********************************************************************
** 																	**
** EXPORTED VARIABLES 												**
** 																	**
*********************************************************************/
extern unsigned long g_ul_keypad_switches; /*Valor leído en los botones*/
extern tBoolean g_b_enviar; /*Si hay mensaje para enviar*/
extern unsigned char g_ucChangedData; /*Si ha cambiado la tecla que se está pulsando*/
extern unsigned char * g_frase; /*Lo escrito por el usuario de la stellaris*/

/*********************************************************************
** 																	**
** GLOBAL VARIABLES 												**
** 																	**
**********************************************************************/
unsigned char g_letra = 97; /*Letra que se muestra actualmente en el display*/
int g_i_tamano = 0; /*Cantidad de caracteres que tiene la frase del usuario*/
int g_i_numero_elemento = 0; /*Identificador del ultimo texto introducido en pantalla*/
int g_i_usuario = 0; /*Identificador del texto del usuario que se muestra en pantalla*/
int g_i_altura_conversacion = 15; /*Altura en la cual se encuentra la conversacion*/
int g_i_altura_usuario = 85; /*Altura en la cual se inserta el texto del usuario*/
tBoolean ultimo_usuario = true; /*Si el ultimo texto introducido en la conversacion es del usuario*/

const unsigned char g_puc_circ[60]  =  {
        0x00, 0x00, 0x44, 0x44, 0x00, 0x00,
        0x00, 0x4a, 0xaa, 0xaa, 0xa4, 0x00,
        0x04, 0xaa, 0xaa, 0xaa, 0xaa, 0x40,
        0x4a, 0xaa, 0xaa, 0xaa, 0xaa, 0xa4,
        0x4a, 0xaa, 0xaa, 0xaa, 0xaa, 0xa4,
        0x4a, 0xaa, 0xaa, 0xaa, 0xaa, 0xa4,
        0x4a, 0xaa, 0xaa, 0xaa, 0xaa, 0xa4,
        0x04, 0xaa, 0xaa, 0xaa, 0xaa, 0x40,
        0x00, 0x4a, 0xaa, 0xaa, 0xa4, 0x00,
        0x00, 0x00, 0x44, 0x44, 0x00, 0x00
    }; /*Dibujo de un círculo*/
const unsigned char g_puc_nada[60]  =  {
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    }; /*Dibujo vacío del tamaño del círculo*/

/*********************************************************************
** 																	**
** PROTOTYPES OF LOCAL FUNCTIONS 									**
** 																	**
*********************************************************************/
void CHAT_inicializacion_usuario();
unsigned char * CHAT_concatenar_usuario();
unsigned char * CHAT_concatenar_remoto();
void CHAT_escribir_usuario();
void CHAT_borrar_usuario();

/*********************************************************************
** 																	**
** LOCAL FUNCTIONS 													**
** 																	**
**********************************************************************/

/**
 * @brief  Inicializamos la pantalla.
 *
 * @return    -
 *
 * Primero se inicializa el buffer y luego se inicializa el texto del usuario.
 * Al final, se vuelca a la pantalla real.
*/
void CHAT_inicializacion_display(){
	g_frase = malloc(sizeof(unsigned char)*MAX_ELEMS_LINEA);
	char *str;

	FRAME_BUFFER_init();
	str="----------";
	FRAME_BUFFER_insert_text(str, 0, 75); //Escribimos en el buffer
	CHAT_inicializacion_usuario(); //Inicializamos el texto del usuario y lo volvamos a la pantalla
}

/**
 * @brief  Refrescamos la pantalla o enviamos los datos.
 *
 * @return      -
 *
 * Se refresca la pantalla dependiendo de la tecla pulsada o se envia el texto.
 * 		*Arriba: se muestra el siguiente caracter.
 * 		*Abajo: se muestra el anterior caracter.
 * 		*Derecha: aumentamos en uno el numero de caracteres.
 * 		*Izquierda: reducimos en uno el numero de caracteres.
 * 		*Select: enviar el texto.
*/
void CHAT_logica_teclas(){
	if(g_ucChangedData){
		switch(g_ul_keypad_switches){
			case KEY_UP:
				if(g_letra == 57 ){
					g_letra = 97;
				} else if(g_letra == 122){
					g_letra = 48;
				} else{
					g_letra++;
				}
				CHAT_escribir_usuario();
				break;
			case KEY_DOWN:
				if(g_letra == 97 ){
					g_letra = 57;
				} else if(g_letra == 48){
					g_letra = 122;
				} else{
					g_letra--;
				}
				CHAT_escribir_usuario();
				break;
			case KEY_LEFT:
				if(g_i_tamano > 0){
					g_frase[g_i_tamano] = 32;
					g_i_tamano--;
					g_letra = g_frase[g_i_tamano];
					CHAT_borrar_usuario();
				}
				break;
			case KEY_RIGHT:
				if(g_i_tamano < MAX_ELEMS_LINEA){
					g_i_tamano++;
				}
				g_letra = 97;
				CHAT_escribir_usuario();
				break;
			case KEY_SELECT:
				CHAT_refrescar_conversacion(TIPO_USUARIO, NULL);
				g_b_enviar = true;
				break;
			default:
				break;
		}
	}
}

/**
 * @brief  Reinicializamos la parte de la pantalla del usuario.
 *
 * @return    -
 *
 * Vaciamos lo escrito por el usuario y ponemos la letra inicial.
*/
void CHAT_reinicializacion_usuario(){
	int contador = 0; //Contador para recorrer el string de datos a escribir en pantalla

	g_frase = malloc(sizeof(unsigned char)*MAX_ELEMS_LINEA);

	g_letra = 97;
	g_frase[contador] = g_letra;
	contador++;
	for(contador; contador < (MAX_ELEMS_LINEA - 1); contador++){
		g_frase[contador] = 32;
	}
	g_frase[contador] = '\0';

	FRAME_BUFFER_delete_element(g_i_usuario);
	g_i_numero_elemento = FRAME_BUFFER_insert_text(g_frase, 0, g_i_altura_usuario);
	g_i_usuario = g_i_numero_elemento - 1;
	FRAME_BUFFER_write_to_display();
}

/**
 * @brief  Insertar la ultima frase de la conversacion
 *
 * @return    -
 *
 * Si se recibe un texto se muestra en pantalla poniendo como inicio PC-> y si
 * se envia se pone Yo->
*/
void CHAT_refrescar_conversacion(int tipo, unsigned char * mensaje){
	int contador = 0;
	int altura = 15;
	unsigned char * final;

	if(!tipo){
		ultimo_usuario = true;
		g_frase[g_i_tamano+1] = '\0';
		final = CHAT_concatenar_usuario();
	} else{
		ultimo_usuario = false;
		final = CHAT_concatenar_remoto(mensaje);
	}

	if(g_i_numero_elemento != MAX_ELEMS_PANTALLA){
		g_i_numero_elemento = FRAME_BUFFER_insert_text(final, 0, g_i_altura_conversacion);
		g_i_altura_conversacion += 10;
	} else{
		contador = 1;
		g_i_numero_elemento = FRAME_BUFFER_delete_element(contador);
		FRAME_BUFFER_write_to_display();
		for(contador; contador < g_i_numero_elemento; contador++){
			FRAME_BUFFER_actualiza_posicion_elemento(contador, 0, altura);
			altura += 10;
		}
		FRAME_BUFFER_actualiza_texto_elemento((g_i_numero_elemento - 1), final);
		if(ultimo_usuario){
			g_i_numero_elemento = FRAME_BUFFER_insert_text(final, 0, 100);
		} else{
			g_i_numero_elemento = FRAME_BUFFER_insert_text(g_frase, 0, g_i_altura_usuario);
		}
	}

	g_i_tamano = 0;
}

/**
 * @brief  Inicializamos la parte de la pantalla del usuario.
 *
 * @return    -
 *
 * Se escribe la letra inicial en pantalla.
*/
void CHAT_inicializacion_usuario(){
	g_frase[g_i_tamano] = g_letra;
	g_frase[g_i_tamano+1] = '\0';

	g_i_numero_elemento = FRAME_BUFFER_insert_text(g_frase, 0, g_i_altura_usuario);
	g_i_usuario = g_i_numero_elemento - 1;
	FRAME_BUFFER_write_to_display();
}

/**
 * @brief  Escritura en la parte del usuario.
 *
 * @return    -
 *
 * Se escribe en pantalla reaccionando al boton pulsado por el usuario.
*/
void CHAT_escribir_usuario(){
	g_frase[g_i_tamano] = g_letra;
	g_frase[g_i_tamano+1] = '\0';

	FRAME_BUFFER_actualiza_texto_elemento(g_i_usuario, g_frase);
	FRAME_BUFFER_write_to_display();
}

/**
 * @brief  Escritura de la parte del usuario.
 *
 * @return    -
 *
 * Se borra la ultima letra introducida por el usuario.
*/
void CHAT_borrar_usuario(){
	g_frase[g_i_tamano] = g_letra;
	g_frase[g_i_tamano+2] = '\0';

	FRAME_BUFFER_actualiza_texto_elemento(g_i_usuario, g_frase);
	FRAME_BUFFER_write_to_display();
}

/**
 * @brief  Concatenacion de lo escrito por el usuario y Yo->
 *
 * @return    -
 *
 * Se concatena lo que ha escrito el usuario y la parte de Yo->
*/
unsigned char * CHAT_concatenar_usuario(){
	int i = 4, j = 0;
	unsigned char * final = malloc(sizeof(unsigned char)*(MAX_ELEMS_LINEA+4));

	final[0] = 'Y';
	final[1] = 'o';
	final[2] = '-';
	final[3] = '>';
	while(j <= g_i_tamano){
		final[i] = g_frase[j];
		i++;
		j++;
	}
	final[i] = '\0';

	return final;
}

/**
 * @brief  Concatenacion de lo recibido por la UART y PC->
 *
 * @return    -
 *
 * Se concatena lo que ha se ha recibido y la parte de PC->
*/
unsigned char * CHAT_concatenar_remoto(unsigned char * mensaje){
	int i = 4, j = 0;
	unsigned char * final = malloc(sizeof(unsigned char)*(MAX_ELEMS_LINEA+4));

	final[0] = 'P';
	final[1] = 'C';
	final[2] = '-';
	final[3] = '>';
	while(mensaje[j] != '\0'){
		final[i] = mensaje[j];
		i++;
		j++;
	}
	final[i] = '\0';

	return final;
}
/*********************************************************************
** 																	**
** EOF 																**
** 																	**
**********************************************************************/
