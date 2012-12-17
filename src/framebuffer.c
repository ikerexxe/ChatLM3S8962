/*********************************************************************
** 																	**
** project : ChatLM3S8962	 										**
** filename : framebuffer.c											**
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
#define CONSOLE_C
/*********************************************************************
**																	**
** MODULES USED 													**
** 																	**
**********************************************************************/
#include "framebuffer.h"
#include <stdio.h>
#include <string.h>
#include "rit128x96x4.h"
#include "displayGenerico.h"
/*********************************************************************
** 																	**
** DEFINITIONS AND MACROS 											**
** 																	**
**********************************************************************/
#define CHAR_WIDTH 6
#define LINE_HEIGHT 8
/*********************************************************************
** 																	**
** GLOBAL VARIABLES 												**
** 																	**
**********************************************************************/
DISPLAY_ELEMENT g_display_elements[MAX_ELEMS_PANTALLA];
int g_display_element_kop = 0; /*Num. de elementos que hay en el buffer*/
extern const unsigned char g_puc_nada[60];/*Dibujo vacío del tamaño del círculo*/
/*********************************************************************
** 																	**
** LOCAL FUNCTIONS 													**
** 																	**
**********************************************************************/
/**
 * @brief  Inicialización de la pantalla y el buffer.
 *
 * @return        -
 *
 * Se inicializa la pantalla y se crea el buffer para un número de elementos definido en MAX_ELEMS
*/
void FRAME_BUFFER_init(){
	int i = 0;
	//Inicializamos la pantalla
	DISPLAY_GENERICO_init();
	//Creamos el buffer
	for (i=0; i<MAX_ELEMS_PANTALLA; i++){
		g_display_elements[i].id = i;
		g_display_elements[i].x = i;
		g_display_elements[i].y = i;
		g_display_elements[i].width = i;
		g_display_elements[i].height = i;
		g_display_elements[i].picture_or_text = i;
		g_display_elements[i].what = i;
	}
}
/**
 * @brief  Insertamos una imagen en el buffer
 *
 * @return  display_element_kop: numero de elementos que hay en el buffer
 *
 * Se crea un elemento del tipo what = 0 (imagen) y se especifica la imagen que debe tomar,
 * la posición en la que debe aparecer y las dimensiones que tiene. Se devuelve el numero de
 * elementos que hay en el buffer despues de haber añadido ésta imagen.
*/
int FRAME_BUFFER_insert_image(char *puc, int x, int y, int w, int h){
	if(g_display_element_kop < MAX_ELEMS_PANTALLA){
		g_display_elements[g_display_element_kop].id = g_display_element_kop;
		g_display_elements[g_display_element_kop].picture_or_text = puc;
		g_display_elements[g_display_element_kop].what = 0; //0 para imagen
		g_display_elements[g_display_element_kop].x = x;
		g_display_elements[g_display_element_kop].y = y;
		g_display_elements[g_display_element_kop].width = w;
		g_display_elements[g_display_element_kop].height = h;
		g_display_element_kop ++; //Aumentamos el número de elementos que hay en el buffer
	}
	return g_display_element_kop;
}
/**
 * @brief  Insertamos texto en el buffer
 *
 * @return  display_element_kop: numero de elementos que hay en el buffer
 *
 * Se crea un elemento del tipo what = 1 (texto) y se especifica el texto que debe tomar,
 * la posición en la que debe aparecer. Las dimensiones serán nulas. Se devuelve el numero de
 * elementos que hay en el buffer despues de haber añadido éste texto.
*/
int FRAME_BUFFER_insert_text(char *texto, int x, int y){
	if(g_display_element_kop < MAX_ELEMS_PANTALLA){
		g_display_elements[g_display_element_kop].id = g_display_element_kop;
		g_display_elements[g_display_element_kop].picture_or_text = texto;
		g_display_elements[g_display_element_kop].what = 1; //1 para texto
		g_display_elements[g_display_element_kop].x = x;
		g_display_elements[g_display_element_kop].y = y;
		g_display_elements[g_display_element_kop].width = 0;
		g_display_elements[g_display_element_kop].height = 0;
		g_display_element_kop ++; //Aumentamos numero de elementos que hay en el buffer
	}
	return g_display_element_kop;
}
/**
 * @brief  Cambiamos la posición de un elemento existente
 *
 * @return  -
 *
 * Se guardan las coordenadas anteriores en xOld e yOld, y se guardan las nuevas
 * coordenadas en su sitio.
*/
void FRAME_BUFFER_actualiza_posicion_elemento(int id, int x, int y){
	int x_actual; /*Posicion actual en el eje x*/
	int y_actual; /*Posicion actual en el eje y*/
	//Obtener posición actual
	x_actual = g_display_elements[id].x;
	y_actual = g_display_elements[id].y;
	//Borrar del FrameBuffer
	g_display_elements[id].xOld = x_actual;
	g_display_elements[id].yOld = y_actual;
	//Actualizar valor
	g_display_elements[id].x = x;
	g_display_elements[id].y = y;
}
/**
 * @brief  Cambiamos el texto de un elemento existente.
 *
 * @return  -
 *
 * Se actualiza el texto del elemento.
*/
void FRAME_BUFFER_actualiza_texto_elemento(int id, char *texto){
	g_display_elements[id].picture_or_text = texto;
}
/**
 * @brief  Borramos un elemento del buffer
 *
 * @return  display_element_kop: numero de elementos que hay en el buffer
 *
 * Sacamos el elemento de la pantalla, y desplazamos los elementos con una id mayor
 * a la posición anterior del buffer. Reduce una vez el numero de elementos del
 * buffer y lo devuelve.
*/
int FRAME_BUFFER_delete_element(int id){
	int x_actual; /*Posición del elemento en el eje x*/
	int y_actual; /*Posición del elemento en el eje y*/
	int i;
	//Obtener posición actual
	x_actual = g_display_elements[id].x;
	y_actual = g_display_elements[id].y;
	//Borrar del FrameBuffer
	g_display_elements[id].xOld = x_actual;
	g_display_elements[id].yOld = y_actual;
	//Actualizar valor
	g_display_elements[id].x = -5;
	g_display_elements[id].y = -5;

	for(i=id;i<MAX_ELEMS_PANTALLA;i++){
		g_display_elements[i].picture_or_text = g_display_elements[i + 1].picture_or_text;
		g_display_elements[i].what = g_display_elements[i + 1].what; //1 para texto
		g_display_elements[i].x = g_display_elements[i + 1].x;
		g_display_elements[i].y = g_display_elements[i + 1].y;
		g_display_elements[i].width = g_display_elements[i + 1].width;
		g_display_elements[i].height = g_display_elements[i + 1].height;

	}
	//Reducimos numero de elementos que hay en el buffer
	g_display_element_kop =  g_display_element_kop - 1;
	return g_display_element_kop;
}
/**
 * @brief  Volcar el buffer en la pantalla
 *
 * @return  -
 *
 * Se vuelcan todos los elementos existentes en el bufefr en la pantalla.
 * Se mira uno a uno si el elemento es un texto o un dibujo, y se usan las
 * funciones para cada uno de los casos para borrar lo innecesario y para
 * mostrar los elementos necesarios.
*/
void FRAME_BUFFER_write_to_display(void){
	int i=0;
	int size = 0; /*Tamaño del string*/
	int index =0; /*Indice de posición en el array str*/
	char str[30]; /*Array donde se va guardar el string*/
	for(i=0;i < g_display_element_kop; i++){ //Volcamos todos los elementos existentes en en buffer
		if(( g_display_elements[i].xOld != g_display_elements[i].x ) ||
		(g_display_elements[i].yOld != g_display_elements[i].y ))
		{
			if(g_display_elements[i].what==1){ //Es un texto
					size = strlen(g_display_elements[i].picture_or_text);
					for(index = 0; index < size; index ++){
						str[index]= ' ';
					}
					str[index]='\0';
					DISPLAY_GENERICO_dibuja_string(str, g_display_elements[i].xOld,g_display_elements[i].yOld, 15);
					DISPLAY_GENERICO_dibuja_string(g_display_elements[i].picture_or_text, g_display_elements[i].x,g_display_elements[i].y, 15);
				}else //Es una imagen
				{
					DISPLAY_GENERICO_dibuja_imagen(g_puc_nada, g_display_elements[i].xOld, g_display_elements[i].yOld, g_display_elements[i].width, g_display_elements[i].height);
					DISPLAY_GENERICO_dibuja_imagen(g_display_elements[i].picture_or_text, g_display_elements[i].x, g_display_elements[i].y, g_display_elements[i].width, g_display_elements[i].height);
				}
		}
	}
}
/*********************************************************************
** 																	**
** EOF 																**
** 																	**
**********************************************************************/




