#ifndef TIMER_DRIVER_REGS_V0_1_0_H_
#define TIMER_DRIVER_REGS_V0_1_0_H_

#include "DR_tipos.h"

// Registro de control de poder de perifericos
#define 	PCONP	(* ( ( __RW uint32_t  * ) 0x400FC0C4UL ))

// Seleccion de la velocidad de clock, los bit 2:3 son para el timer 0
// Son el el registro PCLKSEL0
#define		PCLKSEL0_P		( ( __RW uint32_t  * ) 0x400FC1A8UL )
#define		PCLKSEL_0		PCLKSEL0_P[0]

//Estructura para manejar el timer
typedef struct
{
	uint32_t	IR;
	uint32_t	TCR;
	uint32_t 	TC;
	uint32_t 	PR;
	uint32_t 	PC;
	uint32_t 	MCR;
	uint32_t	MR0;
	uint32_t	MR1;
	uint32_t	MR2;
	uint32_t	MR3;
	uint32_t	CCR;
	uint32_t	CR0;
	uint32_t	CR1;       // 0x40004030 para el T0
	uint32_t    RESERVE[2];// 0x40004034 para el T0
						   // 0x40004038 para el T0
	uint32_t    EMR;       // 0x4000403C para el T0

} timer_t;

//0x40004000UL : Direccion de inicio de los registros del Timer0
#define	DIR_TIMER_0	( ( timer_t  * ) 0x40004000UL )
//0x40004000UL : Direccion de inicio de los registros del Timer1
#define	DIR_TIMER_1	( ( timer_t  * ) 0x40008000UL )

#define TC_0_P ((uint32_t*) 0x40004008UL)
#define TC_0 TC_0_P[0]

#define TC_1_P ((uint32_t*) 0x40008008UL)
#define TC_1 TC_1_P[0]

#define	TIMER_0	DIR_TIMER_0[0]
#define	TIMER_1	DIR_TIMER_1[0]

// Timer 0
#define		T0_IR		TIMER_0.IR
#define     T0_IR_MR0   T0_IR & 0X01       // Bit 0 del registro IR corresponde al flag de la Int por Match 0
#define     T0_IR_MR1   (T0_IR & (0X01 <<1))>>1 // Bit 1 del registro IR corresponde al flag de la Int por Match 1
#define     T0_IR_MR2   (T0_IR & (0X01 <<2))>>2 // Bit 2 del registro IR corresponde al flag de la Int por Match 2
#define     T0_IR_MR3   (T0_IR & (0X01 <<3))>>3 // Bit 3 del registro IR corresponde al flag de la Int por Match 3
#define     T0_IR_CR0   (T0_IR & (0X01 <<1))>>4 // Bit 4 del registro IR corresponde al flag de la Int por CAPTURE 0
#define     T0_IR_CR1   (T0_IR & (0X01 <<1))>>5 // Bit 5 del registro IR corresponde al flag de la Int por CAPTURE 1

#define		T0_TCR		TIMER_0.TCR
#define		T0_PC		TIMER_0.PC
#define		T0_TC		TIMER_0.TC
#define		T0_PR		TIMER_0.PR
#define		T0_MCR		TIMER_0.MCR
#define		T0_MR0		TIMER_0.MR0
#define		T0_MR1		TIMER_0.MR1
#define		T0_MR2		TIMER_0.MR2
#define		T0_MR3		TIMER_0.MR3
#define		T0_CCR		TIMER_0.CCR
#define		T0_CR0		TIMER_0.CR0
#define		T0_CR1		TIMER_0.CR1
#define		T0_EMR		TIMER_0.EMR

// Timer 1
#define		T1_IR		TIMER_1.IR
#define     T1_IR_MR0   T1_IR & 0X01       // Bit 0 del registro IR corresponde al flag de la Int por Match 0
#define     T1_IR_MR1   (T1_IR & (0X01 <<1))>>1 // Bit 1 del registro IR corresponde al flag de la Int por Match 1
#define     T1_IR_MR2   (T1_IR & (0X01 <<2))>>2 // Bit 2 del registro IR corresponde al flag de la Int por Match 2
#define     T1_IR_MR3   (T1_IR & (0X01 <<3))>>3 // Bit 3 del registro IR corresponde al flag de la Int por Match 3
#define     T1_IR_CR0   (T1_IR & (0X01 <<1))>>4 // Bit 4 del registro IR corresponde al flag de la Int por CAPTURE 0
#define     T1_IR_CR1   (T1_IR & (0X01 <<1))>>5 // Bit 5 del registro IR corresponde al flag de la Int por CAPTURE 1

#define		T1_TCR		TIMER_1.TCR
#define		T1_PC		TIMER_1.PC
#define		T1_TC		TIMER_1.TC
#define		T1_PR		TIMER_1.PR
#define		T1_MCR		TIMER_1.MCR
#define		T1_MR0		TIMER_1.MR0
#define		T1_MR1		TIMER_1.MR1
#define		T1_MR2		TIMER_1.MR2
#define		T1_MR3		TIMER_1.MR3
#define		T1_CCR		TIMER_1.CCR
#define		T1_CR0		TIMER_1.CR0
#define		T1_CR1		TIMER_1.CR1
#define		T1_EMR		TIMER_1.EMR

//0xE000E100UL : Direccion de inicio de los registros de habilitación (set) de interrupciones en el NVIC:
#define		ISER		( ( uint32_t * ) 0xE000E100UL )
//0xE000E180UL : Direccion de inicio de los registros de deshabilitacion (clear) de interrupciones en el NVIC:
#define		ICER		( (  uint32_t * ) 0xE000E180UL )

#define     ISE_TIMER_0  ISER[0] |= (0x00000001)<<  1   //ISER0->bit1 pongo un 1 en el bit 1 para habilitar la INT TIMER0
#define 	ISE_TIMER_1  ISER[0] |= (0x00000001)<<  2   //ISER0->bit2 pongo un 1 en el bit 2 para habilitar la INT TIMER1

#define PRESCALE_FOR_1_US 24	//Prescaler puesto a 25 - 1 para que el clock sea de a 1 microsegundo

#endif //TIMER_DRIVER_REGS_V0_1_0_H_
