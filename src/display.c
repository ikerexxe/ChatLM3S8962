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

	FRAME_BUFFER_init();
	char *str;
	str="----------------";
	FRAME_BUFFER_insert_text(str, 0, 75); //Escribimos en el buffer
	FRAME_BUFFER_write_to_display(); //Volcamos el buffer en la pantalla
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









