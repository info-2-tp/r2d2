#include "DR_tipos.h"
#include "DR_GPIO.h"
#include "DR_PLL.h"


#define PWM ( (__RW pwm1_t *) 0x40018000)
#define PWMPRESCALE (25-1)

#define PWM_DIVISOR	1000
#define PWMPRESCALE	(25-1)

#define PWM1 1
#define PWM2 2
#define PWM3 3
#define PWM4 4
#define PWM5 5
#define PWM6 6

#define GPIO_PWM1 2,0
#define GPIO_PWM2 2,1
#define GPIO_PWM3 2,2
#define GPIO_PWM4 2,3
#define GPIO_PWM5 2,4
#define GPIO_PWM6 2,5



#define ENABLE_PWM	2,7			//Exp0

//#define DIR_TORRE	1,29		//Exp1
//#define DIR_BASE	4,28		//Exp2
#define DIR_PWM1	1,18		//Exp2
#define DIR_PWM2	4,28		//Exp2	Base
#define DIR_PWM3	1,18		//Exp2
#define DIR_PWM4	1,29		//Exp1	Torre
#define DIR_PWM5	1,18		//Exp1
#define DIR_PWM6	1,18		//Exp2

typedef struct {
	union{
		uint32_t buffer_salidas2;
		struct{
			__RW uint32_t PWM_1:1;
			__RW uint32_t PWM_2:1;
			__RW uint32_t PWM_3:1;
			__RW uint32_t PWM_4:1;
			__RW uint32_t PWM_5:1;
			__RW uint32_t PWM_6:1;
			__RW uint32_t PWM_DIR_1:1;
			__RW uint32_t PWM_DIR_2:1;
			__RW uint32_t PWM_DIR_3:1;
			__RW uint32_t PWM_DIR_4:1;
			__RW uint32_t PWM_DIR_5:1;
			__RW uint32_t PWM_DIR_6:1;
			__RW uint32_t PWM_ENABLE:1;
			__RW uint32_t RESERVE:19;
		};
	};
}Buffer_t;

typedef struct{	//campos de bits
	__RW uint32_t PWM1IR;
	union{
		__RW uint32_t PWM1TCR;
		struct{
			__RW uint32_t TCR_CountENA:1;	//habilita o deshabilita el temporizador pwm y los contadores prescalares
			__RW uint32_t TCR_CountRST:1;	//reinicia el contador pwm
			__RW uint32_t RESERVED_5:1;
			__RW uint32_t TCR_PWMENA:1;		//hab o deshab el bloque pwm
			__RW uint32_t RESERVED_6:28;
		};
	};
	__RW uint32_t PWM1TC;
	__RW uint32_t PWM1PR;
	__RW uint32_t PWM1PC;
	union{
		__RW uint32_t PWM1MCR;
		struct{
			__RW uint32_t PWMMR0I:1;
			__RW uint32_t PWMMR0R:1;
			__RW uint32_t PWMMR0S:1;
			__RW uint32_t PWMMR1I:1;
			__RW uint32_t PWMMR1R:1;
			__RW uint32_t PWMMR1S:1;
			__RW uint32_t PWMMR2I:1;
			__RW uint32_t PWMMR2R:1;
			__RW uint32_t PWMMR2S:1;
			__RW uint32_t PWMMR3I:1;
			__RW uint32_t PWMMR3R:1;
			__RW uint32_t PWMMR3S:1;
			__RW uint32_t PWMMR4I:1;
			__RW uint32_t PWMMR4R:1;
			__RW uint32_t PWMMR4S:1;
			__RW uint32_t PWMMR5I:1;
			__RW uint32_t PWMMR5R:1;
			__RW uint32_t PWMMR5S:1;
			__RW uint32_t PWMMR6I:1;
			__RW uint32_t PWMMR6R:1;
			__RW uint32_t PWMMR6S:1;
			__RW uint32_t RESERVED_2:11;
		};
	};
	__RW uint32_t PWM1MR0;			//period or PWM_DIVISOR //tiempo del periodo(pulso alto+pulso bajo)
	__RW uint32_t PWM1MR1;			//1.1 dividendo// tiempo de pulso alto
	__RW uint32_t PWM1MR2;			//1.2 dividendo
	__RW uint32_t PWM1MR3;			//1.3 dividendo
	__RW uint32_t PWM1CCR;
	__RW uint32_t PWM1CR0;
	__RW uint32_t PWM1CR1;
	__RW uint32_t PWM1CR2;
	__RW uint32_t PWM1CR3;
	__RW uint32_t RESERVED_0;
	__RW uint32_t PWM1MR4;			//1.4 dividendo
	__RW uint32_t PWM1MR5;			//1.5 dividendo
	__RW uint32_t PWM1MR6;			//1.6 dividendo
	union{
		__RW uint32_t PWM1PCR;		// Select Single Edge PWM (= 0)
		struct{
			__RW uint32_t UNUSED_0:2;	//sin uso
			__RW uint32_t PWMSEL2:1;	//para seleccionar pwm simple o doble
			__RW uint32_t PWMSEL3:1;
			__RW uint32_t PWMSEL4:1;
			__RW uint32_t PWMSEL5:1;
			__RW uint32_t PWMSEL6:1;
			__RW uint32_t RESERVED_4:2;
			__RW uint32_t PWMENA1:1;	//habilitar o deshabilitar salida pwm
			__RW uint32_t PWMENA2:1;
			__RW uint32_t PWMENA3:1;
			__RW uint32_t PWMENA4:1;
			__RW uint32_t PWMENA5:1;
			__RW uint32_t PWMENA6:1;
			__RW uint32_t RESERVED_7:17;
		};
	};
	union{
		__RW uint32_t PWM1LER; 		//update values from MR0 to MR6
		struct{
			__RW uint32_t LER_ENA0:1;
			__RW uint32_t LER_ENA1:1;
			__RW uint32_t LER_ENA2:1;
			__RW uint32_t LER_ENA3:1;
			__RW uint32_t LER_ENA4:1;
			__RW uint32_t LER_ENA5:1;
			__RW uint32_t LER_ENA6:1;
			__RW uint32_t RESERVED_3:25;
		};
	};
	__RW uint32_t RESERVED_1[5];
	__RW uint32_t PWM1CTCR;
}pwm1_t;

extern volatile Buffer_t buffer_salidas;


void Init_PWM();
void EncenderPWM(uint8_t motor, int estado);
void EncenderDireccionPWM(uint8_t motor, uint8_t direciotn);

