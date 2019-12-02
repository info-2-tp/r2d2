//
// Created by marcelo on
//
/*******************************************************************************************************************************//**
 *
 * @file		actions.c
 * @brief		Ejecucion de todas las acciones de la maquina de estado
 * @date		2/11/19.
 * @author		Marcelo Passut
 *
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** INCLUDES
 **********************************************************************************************************************************/
#include <stdio.h>
#include "../inc/actions.h"
#include "../inc/DR_tipos.h"
#include "../inc/PR_PWM.h"
#include "../inc/PR_UART0.h"
#include "../inc/PR_Relay.h"

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
extern int cube_size;
extern unsigned char has_data;
extern char current_cut;
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
	\fn  knifes_stop
	\brief Detiene el motor de la sierra
 	\author
 	\date
 	\param [in]
 	\param [out]
	\return
*/
void knifes_stop() {
	relay_control(MOTOR_SIERRA, OFF);
}
/**
	\fn  knifes_run
	\brief Activa el motor de la sierra
 	\author
 	\date
 	\param [in]
 	\param [out]
	\return
*/
void knifes_run() {
	relay_control(MOTOR_SIERRA, ON);
}

/**
	\fn  move_knife_tower_up
	\brief Mueve la sierra a la posicion superior de la plataforma
 	\author
 	\date
 	\param [in]
 	\param [out]
	\return
*/
void move_knife_tower_up() {
	moveteMotorPWM(TOWER_MOTOR, ON, UP);
}

/**
	\fn  knife_tower_stop
	\brief Detiene el motor que mueve la altura de la sierra
 	\author
 	\date
 	\param [in]
 	\param [out]
	\return
*/
void knife_tower_stop() {
	moveteMotorPWM(TOWER_MOTOR, OFF, UP);
}
/**
	\fn  move_base_back
	\brief Mueve el motor de la base hacia la parte trasera de la plataforma
 	\author
 	\date
 	\param [in]
 	\param [out]
	\return
*/
void move_base_back() {
	moveteMotorPWM(BASE_MOTOR, ON, BACK);
}
/**
	\fn  move_base_front
	\brief Mueve el motor de la base hacia el frente de la plataforma
 	\author
 	\date
 	\param [in]
 	\param [out]
	\return
*/
void move_base_front() {
	moveteMotorPWM(BASE_MOTOR, ON, FRONT);
}
/**
	\fn  base_stop
	\brief Detiene el motor de la base
 	\author
 	\date
 	\param [in]
 	\param [out]
	\return
*/

void base_stop() {
	moveteMotorPWM(BASE_MOTOR, OFF, IZQ);
}
/**
	\fn  send_info_to_obi_wan
	\brief Envia la medicion del cubo por UART a la app para calcular el trabajo
 	\author Julian Mastroiacovo
 	\date ${date}
 	\param [in] cube_size tamaño del cubo a cortar
 	\param [out]
	\return
*/

void send_info_to_obi_wan(unsigned short cube_size) {
    message_header_t header;
    routine_source_t source;

    header.type = MESSAGE_TYPE_NEW_REQUEST;
    header.size = sizeof(routine_source_t);

    source.block_count = DEFAULT_CUBES;
    source.block_height = cube_size;

    send((void*)&header,sizeof(header));
    send((void*)&source,sizeof(source));
}
/**
	\fn  send_ack_to_obi_wan
	\brief Descripcion *****
 	\author
 	\date
 	\param [in]
 	\param [out]
	\return
*/

void send_ack_to_obi_wan() {
    message_header_t header;

    header.type = MESSAGE_TYPE_ACK;
    header.size = 0;

    send((void*)&header,sizeof(header));
}
/**
	\fn  stop_all
	\brief Detiene todos los motores de la maquina
 	\author
 	\date
 	\param [in]
 	\param [out]
	\return
*/

void stop_all() {
    turnOnPWM(OFF);
};

/**
	\fn  calculate_cuts
	\brief Recibe los cortes a traves de la UART1 y los carga al vector cuts
 	\author
 	\date
 	\param [in] vec vector donde se guardan los cortes a realizar
 	\param [in] size cantidad de cortes
 	\param [out] parametros de salida
	\return cuts_t cuts
*/
cuts_t calculate_cuts( const routine_t* vec,unsigned char size) {
	cuts_t cuts;
	cuts.cuts=0;
    for(int i=0; i < size;i++ ){

    	for(int j=0; j < vec[i].cant;j++ ){
    		cuts.positions[cuts.cuts+j]= vec[i].height;
    	}
    	cuts.cuts += vec[i].cant;
    }

    return cuts;
}
/**
	\fn  next_cut
	\brief Incrementa el contador de cortes
 	\author
 	\date
 	\param [in]
 	\param [out] current_cut contador al que aumentamos su valor
	\return
*/

void next_cut() {
    current_cut++;
}
/**
	\fn  reset_cut
	\brief Reinicia el contador de cortes
 	\author
 	\date
 	\param [in]
 	\param [out] current_cut es contador que reiniciamos
	\return
*/

void reset_cut() {
    current_cut = 0;
}








