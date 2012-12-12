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
extern int g_i_numero_elemento;
int g_i_altura_texto = 85;
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
	CHAT_escribir_usuario();
}

void CHAT_reinicializacion_display(){
	int contador = 0;
	unsigned char * temporal = malloc(sizeof(unsigned char)*MAX_ELEMENTOS);

	g_frase[0] = 'a';
	g_frase[1] = '\0';

	temporal[contador] = g_frase[0];
	contador++;
	for(contador; contador < (MAX_ELEMENTOS - 1); contador++){
		temporal[contador] = 32;
	}
	temporal[contador] = '\0';

	FRAME_BUFFER_delete_element(g_i_numero_elemento);
	g_i_numero_elemento = FRAME_BUFFER_insert_text(temporal,0,g_i_altura_texto);
	FRAME_BUFFER_write_to_display();
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
				g_frase[g_i_tamano] = 32;
				if(g_i_tamano > 0){
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
				g_i_tamano = 0;
				g_b_enviar = true;
				break;
			default:

				break;
		}
	}
}

void CHAT_escribir_usuario(){
	g_frase[g_i_tamano] = g_letra;
	g_frase[g_i_tamano+1] = '\0';

	if(g_i_tamano!=0){
		FRAME_BUFFER_delete_element(g_i_numero_elemento);
	}

	g_i_numero_elemento = FRAME_BUFFER_insert_text(g_frase,0,g_i_altura_texto);
	FRAME_BUFFER_write_to_display();
}

void CHAT_borrar_usuario(){
	g_frase[g_i_tamano] = g_letra;
	g_frase[g_i_tamano+2] = '\0';

	if(g_i_tamano!=0){
		FRAME_BUFFER_delete_element(g_i_numero_elemento);
	}

	g_i_numero_elemento = FRAME_BUFFER_insert_text(g_frase,0,g_i_altura_texto);
	FRAME_BUFFER_write_to_display();
}
/**
 * @brief  Función para dibujar la nota en la pantalla.
 *
 * @return    -
 *
 * Se mira la nota que se haya elejido y se dibuja el círculo que le
 * corresponde. Al final, se vuelca en la pantalla.
*/
void MINI_PIANO_dibujar_en_pantalla(){

	switch( g_note){
		case DO:MINI_PIANO_dibujar_circulo( g_note, 10 , 30);
			break;
		case RE:MINI_PIANO_dibujar_circulo( g_note , 10 , 30);
			break;
		case MI:MINI_PIANO_dibujar_circulo( g_note , 10 , 30);
			break;
		case FA:MINI_PIANO_dibujar_circulo( g_note , 10 , 30);
			break;
		case SOL:MINI_PIANO_dibujar_circulo( g_note , 10 , 30);
			break;
		default:MINI_PIANO_dibujar_circulo( g_note , 10 , 30);
			break;

			}
	FRAME_BUFFER_write_to_display(); //Volcamos el buffer en la pantalla
}
/**
 * @brief  Función para dibujar los círculos.
 *
 * @return    -
 * En el caso de que se haya pulsado algún botón, se dibuja un circulo en la nota
 * correspondiente y si no se borra de la pantalla.
 *
*/
void MINI_PIANO_dibujar_circulo(unsigned char state , int x , int y ){
	int i = 0;
	if (state == 6) {
		FRAME_BUFFER_actualiza_posicion_elemento(2,-5, -5);
		FRAME_BUFFER_actualiza_posicion_elemento(3,-5, -5);
	}
	else{
		for (i = 0; i < 5 ; i++){	/*Definimos 5 estados, uno para cada nota*/
			if (  state == i+1 ){

					FRAME_BUFFER_insert_image(g_puc_circ , x+(20*i), y , 12, 10);
					FRAME_BUFFER_actualiza_posicion_elemento(2,x+(20*i), y);

			}
		}
	}
}
/*********************************************************************
** 																	**
** EOF 																**
** 																	**
**********************************************************************/









