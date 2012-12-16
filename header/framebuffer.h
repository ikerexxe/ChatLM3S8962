/*****************************************************************************
**											  								**
** project :  ChatLM3S8962					 							  	**
** filename : framebuffer.h						  					    	**
** version : 1 									  							**
** date :  	2012-10-08		 		 									    **
** 											  								**
******************************************************************************
** 											  								**
**                      											  		**
**  								  					                    **
** 											  								**
******************************************************************************
**										        							**
**VERSION HISTORY:									  						**
**----------------									  						**
**Version : 2										  						**
**Date : 2012-12-05									 					    **
**Revised by : iker pedrosa                    	  							**
**Description : Modified version for the chat.				  				**
*****************************************************************************/
/*****************************************************************************
** 																			**
** DEFINITIONS AND MACROS 													**
** 																			**
******************************************************************************/
#ifndef CONSOLE_H
#define CONSOLE_H

#define MAX_ROWS 11
#define MAX_LINES 12
#define MAX_ELEMS_PANTALLA 8
/*****************************************************************************
** 																			**
** PROTOTYPES OF GLOBAL FUNCTIONS											**
** 																			**
******************************************************************************/
void FRAME_BUFFER_init();
int FRAME_BUFFER_insert_image(char *puc, int x, int y, int w, int h);
int FRAME_BUFFER_insert_text(char *texto, int x, int y);
void FRAME_BUFFER_actualiza_texto_elemento(int id, char *texto);
int FRAME_BUFFER_delete_element(int id);
void FRAME_BUFFER_actualiza_posicion_elemento(int id, int x, int y);
void FRAME_BUFFER_write_to_display(void);
/*****************************************************************************
**	 																		**
** TYPEDEFS AND STRUCTURES 													**
** 																			**
*****************************************************************************/
#ifndef CONSOLE_C
#define PUB_CONSOLE extern
#else
#define PUB_CONSOLE
#endif
/*Creamos la estructura del buffer*/
struct Tipo_DISPLAY_ELEMENT{
	int id;
	int x;
	int y;
	int xOld;
	int yOld;
	int width;
	int height;
	char* picture_or_text;
	int what; /*0=IMAGE, 1=TEXT*/
};
/*Creamos un buffer: DISPLAY_ELEMENT*/
typedef struct Tipo_DISPLAY_ELEMENT DISPLAY_ELEMENT;

/*****************************************************************************
** 																			**
** EOF 																		**
** 																			**
******************************************************************************/
#endif
