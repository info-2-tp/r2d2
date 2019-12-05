/*
 * obi_wan.h
 *
 *  Created on: 5 dic. 2019
 *      Author: exo
 */

#ifndef OBI_WAN_H_
#define OBI_WAN_H_

#include "../inc/PR_UART0.h"

#define READY 1
#define NOT_READY 0

#define MESSAGE_TYPE_NEW_ROUTINE 2

typedef struct {
    unsigned short type;
    unsigned short size;
} message_header_t;

uint8_t are_new_routine();
void loadRoutine(message_header_t *header, routine_t* routines);

#endif /* OBI_WAN_H_ */
