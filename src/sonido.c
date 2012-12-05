/*********************************************************************
** 																	**
** project : ChatLM3S8962	 										**
** filename : sonido.c 												**
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
#define SONIDO_C
/*********************************************************************
**																	**
** MODULES USED 													**
** 																	**
**********************************************************************/
#include "hw_types.h"
#include "hw_pwm.h"
#include "hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/pwm.h"
#include "sonido.h"
/*********************************************************************
** 																	**
** EXPORTED VARIABLES 												**
** 																	**
*********************************************************************/
extern unsigned long g_ul_system_clock; /*Frecuencia del clock*/
extern unsigned long g_us_note_frecuency; /*Frecuencia de las notas*/
/*********************************************************************
** 																	**
** LOCAL FUNCTIONS 													**
** 																	**
**********************************************************************/
/**
 * @brief  Inicializacaión del PWM.
 *
 * @return     -
 *
 * Se inicializa el clock del PWM, y luego los pines correspondientes.
 * Se apaga el generador para configurar su contador, y se vuelve a encender.
 * Se especifica el volumen y se activa la salida del PWM.
*/
void CHAT_inicializacion_pwm(){

	//clock pwm
	SysCtlPWMClockSet(SYSCTL_PWMDIV_8);

	//Configuramos los pines 0 y 1 del puerto G como PWM
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOG);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM);
	GPIOPinTypePWM(GPIO_PORTG_BASE, GPIO_PIN_0);
	GPIOPinTypePWM(GPIO_PORTG_BASE, GPIO_PIN_1);

	// Apagamos el  PWM generator
	PWMOutputState(PWM_BASE,  PWM_OUT_1_BIT, false);
	PWMGenDisable(PWM_BASE, PWM_GEN_0);

	// Configuracion, contador up/down.
	PWMGenConfigure(PWM_BASE, PWM_GEN_0,
	PWM_GEN_MODE_UP_DOWN | PWM_GEN_MODE_SYNC);

	// Activamos pwm
	PWMGenEnable(PWM_BASE, PWM_GEN_0);

	//Especificamos la ganancia.Volumen.
	HWREG(PWM_BASE + PWM_GEN_0_OFFSET + PWM_O_X_CMPB) = 50;
	PWMSyncUpdate(PWM_BASE, PWM_GEN_0_BIT);

	//Activamos la salida del pwm
	PWMOutputState(PWM_BASE, PWM_OUT_1_BIT, true);
}
/**
 * @brief  Reproducir una nota con el PWM.
 *
 * @return     -
 *
 * Se activa el PWM con la frecuencia seleccionada.
*/
void MINI_PIANO_reproducir_nota(){
	PWMGenPeriodSet(PWM_BASE, PWM_GEN_0,(g_ul_system_clock /(g_us_note_frecuency * 8)));
	PWMSyncUpdate(PWM_BASE, PWM_GEN_0_BIT);
}
/*********************************************************************
** 																	**
** EOF 																**
** 																	**
**********************************************************************/







