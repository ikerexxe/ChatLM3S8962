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
extern unsigned char g_note; /*Numero de nota*/
extern unsigned long g_ul_keypad_switches; /*Valor leído en los botones*/
extern tBoolean g_b_enviar;
extern unsigned char g_ucChangedData; //si se ha cambiado la tecla que se esta pulsando
extern unsigned char * g_frase;
/*********************************************************************
** 																	**
** GLOBAL VARIABLES 												**
** 																	**
**********************************************************************/
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

unsigned char g_letra = 97;
int g_i_tamano = 0;
int g_i_numero_elemento = 0;
int g_i_altura_usuario = 85;
int g_i_altura_conversacion = 15;
int minus = 2;
tBoolean ultimo_envio = true;
int g_i_usuario = 0;

unsigned char * concatenar_usuario();
unsigned char * concatenar_remoto();

/*********************************************************************
** 																	**
** LOCAL FUNCTIONS 													**
** 																	**
**********************************************************************/
/**
 * @brief  Inicializamos y escribimos notas en la pantalla.
 *
 * @return    -
 *
 * Primero se inicializa el buffer, y luego se escriben las notas en él.
 * Al final, se vuelca a la pantalla real.
*/
void CHAT_inicializacion_display(){
	g_frase = malloc(sizeof(unsigned char)*MAX_ELEMENTOS);
	char *str;

	FRAME_BUFFER_init();
	str="----------(Final)";
	FRAME_BUFFER_insert_text(str, 0, 75); //Escribimos en el buffer
	CHAT_inicializacion_usuario();
}

/**
 * @brief  Le damos valor a la nota.
 *
 * @return      -
 *
 * Se le asigna un valor a la variable g_note según la nota que
 * se haya seleccionado.
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
				if(g_i_tamano < MAX_ELEMENTOS){
					g_i_tamano++;
				}
				g_letra = 97;
				CHAT_escribir_usuario();
				break;
			case KEY_SELECT:
				CHAT_refrescar_conversacion();
				g_b_enviar = true;
				break;
			default:

				break;
		}
	}
}

void CHAT_inicializacion_usuario(){
	g_frase[g_i_tamano] = g_letra;
	g_frase[g_i_tamano+1] = '\0';

	g_i_numero_elemento = FRAME_BUFFER_insert_text(g_frase, 0, g_i_altura_usuario);
	g_i_usuario = g_i_numero_elemento - 1;
	FRAME_BUFFER_write_to_display();
}

void CHAT_reinicializacion_usuario(){
	int contador = 0;

	g_frase = malloc(sizeof(unsigned char)*MAX_ELEMENTOS);

	g_letra = 97;
	g_frase[contador] = g_letra;
	contador++;
	for(contador; contador < (MAX_ELEMENTOS - 1); contador++){
		g_frase[contador] = 32;
	}
	g_frase[contador] = '\0';

	FRAME_BUFFER_delete_element(g_i_usuario);
	g_i_numero_elemento = FRAME_BUFFER_insert_text(g_frase, 0, g_i_altura_usuario);
	g_i_usuario = g_i_numero_elemento - 1;
	FRAME_BUFFER_write_to_display();
}

void CHAT_escribir_usuario(){
	g_frase[g_i_tamano] = g_letra;
	g_frase[g_i_tamano+1] = '\0';

	FRAME_BUFFER_actualiza_texto_elemento(g_i_usuario, g_frase);
	FRAME_BUFFER_write_to_display();
}

void CHAT_borrar_usuario(){
	g_frase[g_i_tamano] = g_letra;
	g_frase[g_i_tamano+2] = '\0';

	FRAME_BUFFER_actualiza_texto_elemento(g_i_usuario, g_frase);
	FRAME_BUFFER_write_to_display();
}

void CHAT_refrescar_conversacion(int tipo, unsigned char * mensaje){
	int contador = 0;
	int altura = 15;
	unsigned char * final;

	if(!tipo){
		ultimo_envio = true;
		g_frase[g_i_tamano+1] = '\0';
		final = concatenar_usuario();
	} else{
		ultimo_envio = false;
		final = concatenar_remoto(mensaje);
	}

	if(g_i_numero_elemento != MAX_ELEMS){
		g_i_numero_elemento = FRAME_BUFFER_insert_text(final, 0, g_i_altura_conversacion);
		g_i_altura_conversacion += 10;
	} else{
		contador = 1;
		g_i_numero_elemento = FRAME_BUFFER_delete_element(contador);
		for(contador; contador < g_i_numero_elemento; contador++){
			FRAME_BUFFER_actualiza_posicion_elemento(contador, 0, altura);
			altura += 10;
		}
		FRAME_BUFFER_actualiza_texto_elemento((g_i_numero_elemento - 1), final);
		if(ultimo_envio){
			g_i_numero_elemento = FRAME_BUFFER_insert_text(final, 0, 100);
		} else{
			g_i_numero_elemento = FRAME_BUFFER_insert_text(g_frase, 0, g_i_altura_usuario);
		}
	}

	FRAME_BUFFER_write_to_display();
	g_i_tamano = 0;
}

unsigned char * concatenar_usuario(){
	int i = 4, j = 0;
	unsigned char * final = malloc(sizeof(unsigned char)*(MAX_ELEMENTOS+4));

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

unsigned char * concatenar_remoto(unsigned char * mensaje){
	int i = 4, j = 0;
	unsigned char * final = malloc(sizeof(unsigned char)*(MAX_ELEMENTOS+4));

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









