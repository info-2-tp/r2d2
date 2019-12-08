/*******************************************************************************************************************************//**
 *
 * @file	PR_buttons.c
 * @brief	Control de los botones y fines de carrera
 * @date	17 nov. 2019
 * @author	Marcelo Passut
 *
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** INCLUDES
 **********************************************************************************************************************************/
#include "../inc/PR_buttons.h"
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
volatile button_state stop_button;
volatile button_state knifes_tower_top;
volatile button_state base_init;
volatile button_state start_button;
volatile button_state base_end;
volatile button_state emergency_button;
volatile button_state up_button;
volatile button_state down_button;
volatile button_state presence_button;

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
	\fn  stop_button_active
	\brief devuelve el estado del boton stop_button1
 	\author
 	\date
 	\param [in]
 	\param [out]
	\return
*/
uint8_t stop_button_active() {
	return stop_button.current_state;
}

/**
	\fn  emergency_button_active
	\brief devuelve el estado del boton emergency_button
 	\author
 	\date
 	\param [in]
 	\param [out]
	\return
*/
uint8_t emergency_button_active() {
	return emergency_button.current_state;
}

/**
	\fn  up_button_active
	\brief devuelve el estado del boton up_button
 	\author
 	\date
 	\param [in]
 	\param [out]
	\return
*/
uint8_t up_button_active() {
	return up_button.current_state;
}

/**
	\fn  down_button_active
	\brief devuelve el estado del boton down_button
 	\author
 	\date
 	\param [in]
 	\param [out]
	\return
*/
uint8_t down_button_active() {
	return down_button.current_state;
}



/**
	\fn  knifes_tower_on_top
	\brief devuelve el estado de knifes_tower_top
 	\author
 	\date
 	\param [in]
 	\param [out]
	\return
*/
uint8_t knifes_tower_on_top() {
	return knifes_tower_top.current_state;
}

/**
	\fn  base_on_init
	\brief devuelve el estado de base_init
 	\author
 	\date
 	\param [in]
 	\param [out]
	\return
*/
uint8_t base_on_init() {
	return base_init.current_state;
}

/**
	\fn  run_button_active
	\brief devuelve el estado de start_button
 	\author
 	\date
 	\param [in]
 	\param [out]
	\return
*/
uint8_t run_button_active(){
	return start_button.current_state;
}

/**
	\fn  base_on_end
	\brief Devuelve el estado de el boton de base_end
 	\author
 	\date
 	\param [in]
 	\param [out]
	\return
*/
uint8_t base_on_end() {
	return base_end.current_state;
}

/**
	\fn  up_button_active
	\brief devuelve el estado del boton up_button
 	\author
 	\date
 	\param [in]
 	\param [out]
	\return
*/
uint8_t detect_cube() {
	return presence_button.current_state;
}

