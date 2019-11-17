/*
 * DR_Systick.h
 *
 *  Created on: 6 sep. 2019
 *      Author: gif30
 */


#ifndef DR_SYSTICK_H_
#define DR_SYSTICK_H_

#include "../inc/DR_tipos.h"

typedef struct{

	union {
			__RW uint32_t STCTRL;
		struct{
			__RW uint32_t ENABLE:1;			//enable del modulo
			__RW uint32_t TICKINT:1;		//habilitar interrupt (para hacer con pulling usar COUNTFLAG)
			__RW uint32_t CLKSOURCE:1;		//elegis entre clock de CPU (CCLK) o pin externo (STCLK)
			__RW uint32_t RESERVED_0:13;	//reservado
			__RW uint32_t COUNTFLAG:1;		//pone en 1 si llega al tiempo
			__RW uint32_t RESERVED_1:15;	//reservado
		};
	};

	__RW uint32_t STRELOAD;		//base de tiempo
	__RW uint32_t STCURR;		//memoria del tiempo
	__RW uint32_t STCALIB;		//base de tiempo 10ms fija

}systick_t;

#define Systick ((__RW systick_t *) 0xE000E010UL)



void SysTick_Init(void);
void SysTick_Handler(void); //Scheduler o maquina de timers



#endif /* DR_SYSTICK_H_ */
