/*******************************************************************************************************************************//**
 *
 * @file		DR_GPIO.c
 * @brief
 * @date
 * @author		Cristian Usigas
 *
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** INCLUDES
 **********************************************************************************************************************************/
#include <DR_GPIO.h>
/***********************************************************************************************************************************
 *** DEFINES PRIVADOS AL MODULO
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** MACROS PRIVADAS AL MODULO
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** TIPOS DE DATOS PRIVADOS AL MODULO
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** TABLAS PRIVADAS AL MODULO
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** VARIABLES GLOBALES PUBLICAS
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** VARIABLES GLOBALES PRIVADAS AL MODULO
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** PROTOTIPO DE FUNCIONES PRIVADAS AL MODULO
 **********************************************************************************************************************************/

 /***********************************************************************************************************************************
 *** FUNCIONES PRIVADAS AL MODULO
 **********************************************************************************************************************************/

 /***********************************************************************************************************************************
 *** FUNCIONES GLOBALES AL MODULO
 **********************************************************************************************************************************/

/**
	\fn  GPIO_Pinsel
	\brief
 	\author
 	\date
 	\param [in] Puerto puerto a seleccionar
 	\param [in] Pin del micro que vamos a configurar
 	\param [in] Funcion: la funcion que queremos que cumpla ese pin
 	\param [out]
	\return
*/
void GPIO_Pinsel(uint32_t Puerto, uint32_t Pin, uint32_t Funcion){

	PINSEL[(Puerto*2) + (Pin/16)]&=~(3<<((Pin%16) * 2));

	if(Funcion!=0)
		PINSEL[(Puerto*2) + (Pin/16)]|=(Funcion<<((Pin%16) * 2));
}


/**
	\fn  GPIO_Mode
	\brief	Modificamos el modo en que el pin espera la señal que ingresara
 	\author
 	\date
 	\param [in] Puerto puerto a seleccionar
 	\param [in] Pin del micro que vamos a configurar
 	\param [in] Configurar: modo en que queremos que este el pin, sea PULL_UP, PULL_DOWN, NONE,etc
 	\param [out]
	\return
*/
void GPIO_Mode(uint32_t Puerto, uint32_t Pin,uint32_t Configurar){

	PINMODE[(Puerto*2) + (Pin/16)]&=~(3<<((Pin%16) * 2));

	if(Configurar!=0){
		PINMODE[(Puerto*2) + (Pin/16)]|=(Configurar<<((Pin%16) * 2));
	}
}

/**
	\fn  GPIO_Dir
	\brief Le indicamos al micro que ese pin sera de entrada o salida de bits
 	\author
 	\date
 	\param [in] Puerto puerto a seleccionar
 	\param [in] Pin del micro que vamos a configurar
 	\param [in] Dir: la direccion que va a tener el pin, sea entrada o salida
 	\param [out]
	\return
*/
void GPIO_Dir(uint32_t Puerto, uint32_t Pin,uint32_t Dir){	//0: Entrada; 1: Salida
	
	if(Dir)
		GPIOs[Puerto].FIODIR|=(1<<Pin);
	else
		GPIOs[Puerto].FIODIR&=~(1<<Pin);
}

/**
	\fn  GPIO_Set
	\brief Modificamos el estado de un pin para que este en LOW o HIGH
 	\author
 	\date
 	\param [in] Puerto puerto a seleccionar
 	\param [in] Pin del micro que vamos a modificar
 	\param [out]
	\return
*/
void GPIO_Set(uint32_t Puerto, uint32_t Pin, uint32_t Estado){	//escritura del GPIO
	
	if(1==Estado)	//Buena practica de programacion: en lugar de if(Estado==1)
		GPIOs[Puerto].FIOSET = (1<<Pin);
	else
		GPIOs[Puerto].FIOCLR = (1<<Pin);
}

/**
	\fn  GPIO_Get
	\brief Controlamos el estado del pin si esta en LOW o HIGH
 	\author
 	\date
 	\param [in] Puerto puerto a seleccionar
 	\param [in] Pin del micro que queremos controlar
 	\param [out]
	\return
*/
uint32_t GPIO_Get(uint32_t Puerto, uint32_t Pin){	//lectura del GPIO

	if(GPIOs[Puerto].FIOPIN & (1<<Pin))
		return 1;
	else
		return 0;
}





