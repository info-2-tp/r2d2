
#ifndef funciones_GPIO_h
#define funciones_GPIO_h

#include "DR_tipos.h"


//Estados de FIODIR:
#define		OUTPUT	1
#define		INPUT	0

#define		ALTO	1
#define		BAJO	0

//Estados de PINSEL:
#define		PINSEL_GPIO		0
#define		PINSEL_FUNC1	1
#define		PINSEL_FUNC2	2
#define		PINSEL_FUNC3	3

//Estados de PINMODE
#define		PULLUP 		0
#define		REPEAT 		1
#define		NONE 		2
#define		PULLDOWN 	3


//Registros generales:
//0x4002C000UL : Direccion de inicio de los registros PINSEL
#define		PINSEL		( ( uint32_t  * ) 0x4002C000UL )
//FTM para asignar solo utilizando PINSEL=...; Utilidad = Evita utilizar punteros
//0x2009C000UL : Direccion de inicio de los registros de GPIOs
#define		GPIOs		( ( gpio_t  * ) 0x2009C000UL )
//0x400FC0C4UL : Direccion de inicio del registro de habilitación de dispositivos:
#define 	DIR_PCONP	( ( uint32_t  * ) 0x400FC0C4UL )

//0x400FC1A8UL : Direccion de inicio de los registros de seleccion de los clks de los dispositivos:
//#define		PCLKSEL		( ( uint32_t  * ) 0x400FC1A8UL )

//0x4002C040UL : Direccion de inicio de los registros de modo de los pines del GPIO
#define		PINMODE		( ( uint32_t  * ) 0x4002C040UL )
//0xE000E100UL : Direccion de inicio de los registros de habilitación (set) de interrupciones en el NVIC:
#define		ISER		( ( uint32_t  * ) 0xE000E100UL )
//0xE000E180UL : Direccion de inicio de los registros de deshabilitacion (clear) de interrupciones en el NVIC:
#define		ICER		( ( uint32_t  * ) 0xE000E180UL )

//Estructura para manejar los GPIOs:
typedef struct
{
	uint32_t	FIODIR;
	uint32_t	RESERVED[3];//Espacio en blanco entre FIODIR y FIOMASK
	uint32_t 	FIOMASK;
	uint32_t 	FIOPIN;
	uint32_t 	FIOSET;
	uint32_t 	FIOCLR;

} gpio_t;

void GPIO_Pinsel(uint32_t Puerto, uint32_t Pin,uint32_t Configurar);	//tambien llamada PINSEL por el resto de la catedra
void GPIO_Mode(uint32_t Puerto, uint32_t Pin,uint32_t Configurar);		//pull up or pull down
void GPIO_Dir(uint32_t Puerto, uint32_t Pin,uint32_t Dir);				//0: INPUT; 1: OUTPUT
uint32_t GPIO_Get(uint32_t Puerto, uint32_t Pin);						//lectura del GPIO
void GPIO_Set (uint32_t Puerto, uint32_t Pin, uint32_t Estado);			//escritura del GPIO

#endif
