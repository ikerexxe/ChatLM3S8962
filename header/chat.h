/*****************************************************************************
**											  								**
** project :  ChatLM3S8962					 							  	**
** filename : chat.h	  						  					    	**
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
/*
 * Definición de las frecuencias
 */
#define FRECUENCIA_SILENCIO 40000
#define FRECUENCIA_DO 262
#define FRECUENCIA_RE 294
#define FRECUENCIA_MI 330
#define FRECUENCIA_FA 349
#define FRECUENCIA_SOL 392
/*
 * Definición de las notas
 */
#define DO 1
#define RE 2
#define MI 3
#define FA 4
#define SOL 5
/*
 * Definición de las teclas
 */
#define KEY_UP 0x1e     //11110
#define KEY_DOWN 0x1d   //11101
#define KEY_LEFT 0x1b   //11011
#define KEY_RIGHT 0x17  //10111
#define KEY_SELECT 15 //01111
/*
 * Definición del numero maximo de caracteres por linea
 */
#define MAX_ELEMS_LINEA 10
/*
 * Definición del tipo de usuario que escribe en la conversacion
 */
#define TIPO_USUARIO 0 //El usuario de la stellaris
#define TIPO_REMOTO 1 //El usuario del ordenador

#define NULL 0
/*****************************************************************************
** 																			**
** EOF 																		**
** 																			**
******************************************************************************/

