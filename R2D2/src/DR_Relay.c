/*******************************************************************************************************************************//**
 *
 * @file	DR_Relay.c
 * @brief	Drivers para activacion de relays
 * @date
 * @author	Cristian Usigas
 *
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** INCLUDES
 **********************************************************************************************************************************/
#include "../inc/DR_Relay.h"
#include "../inc/DR_GPIO.h"
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
uint8_t buffer_relay[4];
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
	\fn  init_Relay
	\brief Configuracion inicial de pines del Relay0
 	\author Cristian Usigas
 	\date
 	\param [in]
 	\param [out]
	\return
 */
void init_Relay(void)
{
	GPIO_Pinsel(RELAY0, PINSEL_GPIO);
	GPIO_Dir( RELAY0 , OUTPUT);
	GPIO_Set(RELAY0,OFF);
}

/**
	\fn  active_relay
	\brief Controla el buffer_relay y actua dependiendo el valor de cada bit del buffer
 	\author Cristian Usigas
 	\date
 	\param [in]
 	\param [out]
	\return
 */
void active_relay(void)
{
	int i;
	for(i = 0; i < 4; i++)
	{
		if (buffer_relay[i])
		{
			switch(i)
			{
			case 0:
				GPIO_Set(RELAY0,ON);
				break;
			case 1:
				GPIO_Set(RELAY1,ON);
				break;
			case 2:
				GPIO_Set(RELAY2,ON);
				break;
			}
		}
		else if(!buffer_relay[i])
		{
			switch(i)
			{
			case 0:
				GPIO_Set(RELAY0,OFF);
				break;
			case 1:
				GPIO_Set(RELAY1,OFF);
				break;
			case 2:
				GPIO_Set(RELAY2,OFF);
				break;
			}
		}
	}
}
