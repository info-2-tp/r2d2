/*******************************************************************************************************************************//**
 *
 * @file	events.c
 * @brief	Funcion con todos los eventos de control
 * @date	2/11/19.
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
	\fn base_back
	\brief Control del boton de base_on_end
 	\author
 	\date
 	\param [in]
 	\param [out]
	\return Devuelve el estado del boton de base_on_end
*/
char base_back() {
    return base_on_end();
}

/**
	\fn  base_middle
	\brief
 	\author
 	\date
 	\param [in]
 	\param [out]
	\return
*/
char base_middle() {
    return 0;
}

/**
	\fn  base_front
	\brief Control del boton de base_front
 	\author
 	\date
 	\param [in]
 	\param [out]
	\return Devuelve el estado del boton de base_front
*/
char base_front() {
    return base_on_init();
}

/**
	\fn   know_stop_button
	\brief Control del boton de stop
 	\author
 	\date
 	\param [in]
 	\param [out]
	\return Devuelve el estado del boton de stop
*/
char know_stop_button() {
    return stop_button_active();
}

/**
	\fn  know_emergency_button
	\brief Control del boton de emergencia
 	\author
 	\date
 	\param [in]
 	\param [out]
	\return Devuelve el estado del boton de emergencia
*/
char know_emergency_button(){
	return emergency_button_active();
}
/**
	\fn  know_up_button
	\brief Control del boton de arriba
 	\author
 	\date
 	\param [in]
 	\param [out]
	\return Devuelve el estado del boton arriba
*/
char know_up_button(){
	return up_button_active();
}
/**
	\fn  know_down_button
	\brief Control del boton abajo
 	\author
 	\date
 	\param [in]
 	\param [out]
	\return Devuelve el estado del boton abajo
*/
char know_down_button(){
	return down_button_active();
}

/**
	\fn  knifes_top
	\brief control del boton de TOWER_TOP
 	\author
 	\date
 	\param [in]
 	\param [out]
	\return Estado del boton TOWER_TOP
*/
char knifes_top() {
    return knifes_tower_on_top();
}

/**
	\fn  run_button
	\brief control del boton de RUN
 	\author
 	\date
 	\param [in]
 	\param [out]
	\return	devuelve el estado del boton RUN
*/
char know_run_button() {
    return run_button_active();
}

/**
	\fn  knifes_are_ready
	\brief
 	\author
 	\date
 	\param [in]
 	\param [out]
	\return
*/
char knifes_are_ready() {
    return 0;
}

/**
	\fn  knifes_is_there
	\brief
 	\author
 	\date
 	\param [in]
 	\param [out]
	\return
*/
char knifes_is_there(unsigned short position) {
    return 0;
}
