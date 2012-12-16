/*****************************************************************************
**											  								**
** project :  ChatLM3S8962					 							  	**
** filename : displayGenerico.h					  					    	**
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
#ifndef _DISPLAY_GENERICO_H
#define _DISPLAY_GENERICO_H
/*****************************************************************************
** 																			**
** PROTOTYPES OF GLOBAL FUNCTIONS											**
** 																			**
******************************************************************************/
void DISPLAY_GENERICO_dibuja_string(const char *pcStr, unsigned long ulX,
        unsigned long ulY, unsigned char ucLevel);
void DISPLAY_GENERICO_dibuja_imagen(const unsigned char *pucImage, unsigned long ulX,
        unsigned long ulY, unsigned long ulWidth,
        unsigned long ulHeight);
void DISPLAY_GENERICO_init();
/*****************************************************************************
**	 																		**
** TYPEDEFS AND STRUCTURES 													**
** 																			**
*****************************************************************************/
#ifndef DISPLAY_RIT_C
#define PUB_DISPLAY_RIT extern
#else
#define PUB_DISPLAY_RIT
#endif
/*****************************************************************************
** 																			**
** EOF 																		**
** 																			**
******************************************************************************/
#endif
