/*
 * obi_wan.c
 *
 *  Created on: 5 dic. 2019
 *      Author: exo
 */

#include "../inc/obi_wan.h"
#include "../inc/PR_timer.h"


#define STATE_WAITING_HEADER 0
#define STATE_WAITING_ROUTINE 1

#define HEADER_SIZE sizeof(message_header_t)
/**************************** Uso interno de la libreria *****************************/
typedef union {
	message_header_t header;
	uint8_t bytes[4];
} message_header_u;

uint8_t obi_wan_com_state = STATE_WAITING_HEADER;
uint8_t header_bytes_loaded = 0;

message_header_u header;

/***************************** State *******************************************/
uint8_t timeout = 0;
uint8_t restore = 0;

uint16_t obi_wan_ttl_timer;
uint16_t obi_wan_timeout_timer;

uint8_t obi_wan_restore() {
	uint8_t tmp = restore;
	restore = 0;
	return tmp;
}

void timer_obi_wan_ttl() {
	send_ping_to_obi_wan();
	obi_wan_ttl_timer = startTimer(1, timer_obi_wan_ttl, SECONDS);
}

void timer_obi_wan_timeout() {
	obi_wan_timeout_timer = startTimer(5, timer_obi_wan_timeout, SECONDS);
	timeout = 1;
}

void init_obi_wan_com() {
	obi_wan_ttl_timer = startTimer(1, timer_obi_wan_ttl, SECONDS);
	obi_wan_timeout_timer = startTimer(5, timer_obi_wan_timeout, SECONDS);
}

uint8_t obi_wan_timeout() {
	return timeout;
}

/**************************** Info de New Routine *****************************/
typedef struct {
    unsigned int cant;
    unsigned int height;
} routine_t;

typedef union {
	routine_t routines[50];
	uint8_t bytes[50*8];
} message_routines_u;

message_header_t new_routine_header;
uint8_t new_routine_header_ready = NOT_READY;

uint16_t routine_bytes_loaded = 0;
routine_t new_routines[50];
message_routines_u routines_u;
uint8_t routine_ready = NOT_READY;

/**************************** Funciones internas *****************************/

receive_header() {
	if (header_bytes_loaded == HEADER_SIZE) return READY;

	int dato = UART0_PopRX();
	if (dato == -1) return 0;
	header.bytes[header_bytes_loaded] = (uint8_t)dato;
	header_bytes_loaded++;
	return header_bytes_loaded == HEADER_SIZE;
}

receive_routine(message_header_t header) {
	if (routine_bytes_loaded == header.size) return READY;

	int dato = UART0_PopRX();
	if (dato == -1) return 0;

	routines_u.bytes[routine_bytes_loaded] = (uint8_t)dato;
	routine_bytes_loaded++;
	return routine_bytes_loaded == header.size;
}

void listen_header() {
	if (receive_header()) {
		if (header.header.type == MESSAGE_TYPE_NEW_ROUTINE) {
			new_routine_header = header.header;
			new_routine_header_ready = READY;
			obi_wan_com_state = STATE_WAITING_ROUTINE;
		}

		if (header.header.type == MESSAGE_TYPE_PONG) {
			obi_wan_com_state = STATE_WAITING_HEADER;
			killTimer(obi_wan_timeout_timer);
			obi_wan_timeout_timer = startTimer(5, timer_obi_wan_timeout, SECONDS);
			if (timeout) {
				timeout = 0;
				restore = 1;
			}
		}
	}
}

void listen_routine() {
	if (receive_routine(new_routine_header.size)) {
		new_routines = routines_u.routines;
		routine_ready = READY;
		obi_wan_com_state = STATE_WAITING_HEADER;
	}
}


/********************************** Funciones de publicas ***********************/
uint8_t are_new_routine() {
	return routine_ready;
}

void load_routine(message_header_t *header, routine_t** routines) {
	*header = new_routine_header;
	*routines = new_routines;
	new_routine_header_ready = NOT_READY;
	routine_ready = NOT_READY;
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

void send_routine_request_to_obi_wan(uint16_t cube_size) {
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

vpod send_ping_to_obi_wan() {
	message_header_t header;

	header.type = MESSAGE_TYPE_PING;
	header.size = 0;

	send((void*)&header,sizeof(header));
}

void obi_wan_listen() {
	if (obi_wan_com_state == STATE_WAITING_HEADER) {
		listen_header();
	}
	if (obi_wan_com_state == STATE_WAITING_ROUTINE) {
		listen_routine();
	}
}
