/*********************************************************************
** 																	**
** project : ChatLM3S8962	 										**
** filename : displayRIT.c											**
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
#define DISPLAY_RIT_C
/*********************************************************************
**																	**
** MODULES USED 													**
** 																	**
**********************************************************************/
#include "displayGenerico.h"
#include "rit128x96x4.h"
#include "string.h"
/*********************************************************************
** 																	**
** LOCAL FUNCTIONS 													**
** 																	**
**********************************************************************/
/**
 * @brief  Función para escribir en pantalla.
 *
 * @return        -
 *
*/
void DISPLAY_GENERICO_dibuja_string(const char *pcStr, unsigned long ulX,
        unsigned long ulY, unsigned char ucLevel){
	RIT128x96x4StringDraw(pcStr,ulX,ulY,ucLevel);
}
/**
 * @brief  Función para dibujar en pantalla.
 *
 * @return        -
 *
 *
*/
void DISPLAY_GENERICO_dibuja_imagen(const unsigned char *pucImage, unsigned long ulX,
        unsigned long ulY, unsigned long ulWidth,
        unsigned long ulHeight){
	RIT128x96x4ImageDraw(pucImage, ulX,ulY,ulWidth,ulHeight);
}
/**
 * @brief  Función para inicializar pantalla.
 *
 * @return        -
 *
 *
*/
void DISPLAY_GENERICO_init(){
	RIT128x96x4Init(1000000);
}
/*********************************************************************
** 																	**
** EOF 																**
** 																	**
**********************************************************************/




