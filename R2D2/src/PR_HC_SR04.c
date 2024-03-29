/*******************************************************************************************************************************//**
 *
 * @file	PR_HC_SR04.c
 * @brief	Funciones primitivas del sensor de ultrasonido
 * @date	26 oct. 2019
 * @author	Marcelo Passut
 *
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** INCLUDES
 **********************************************************************************************************************************/
#include "../inc/PR_timer.h"
#include "../inc/DR_HC_SR04.h"
#include "../inc/PR_HC_SR04.h"

/***********************************************************************************************************************************
 *** DEFINES PRIVADOS AL MODULO
 **********************************************************************************************************************************/
#define TRIGGER_TTL 10
#define MICROSECONDS_BY_CM 58
#define TTL_ECHO 174000
#define MAX 13
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
	\fn  transform_microseconds_to
	\brief Calcula el cambio de microsegundos a la base que se necesite
 	\author
 	\date
 	\param [in]
 	\param [out]
	\return
*/
distance_t transform_microseconds_to(uint32_t microseconds, distance_base base) {
	return microseconds*base/MICROSECONDS_BY_CM;
}

/**
	\fn  init_hc_sr04_sensor
	\brief Configuracion inicial del sensor ultrasonico
 	\author
 	\date
 	\param [in]
 	\param [out]
	\return
*/
void init_hc_sr04_sensor() {
	init_hc_sr04();
}

//distance_t vector[]={ 590,590,590,590,590,590,590,590,480,480,480,480, 280 };


/**
	\fn  getDistance
	\brief
 	\author
 	\date
 	\param [in] base :
 	\param [out]
	\return devuelve la distancia
*/
distance_t getDistance(distance_base base) {
	//static uint8_t index=0;
	//index++;
	//index = index%MAX;
	//return vector[index];

	uint16_t cron;
	uint32_t microseconds;
	trigger_turn_on();
	wait(TRIGGER_TTL);
	trigger_turn_off();
	cron = startCronometer();
	while(echo_off() && getCronometer(cron) < TTL_ECHO);
	cron = startCronometer();
	while(echo_on() && getCronometer(cron) < TTL_ECHO);
	microseconds = stopCronometer(cron);
	return transform_microseconds_to(microseconds, base);
}

