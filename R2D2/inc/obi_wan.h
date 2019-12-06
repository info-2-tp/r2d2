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
#define MESSAGE_TYPE_NEW_REQUEST 1
#define MESSAGE_TYPE_NEW_ROUTINE 2
#define MESSAGE_TYPE_ACK 3
#define MESSAGE_TYPE_PING 4
#define MESSAGE_TYPE_PONG 5

typedef struct {
    unsigned short type;
    unsigned short size;
} message_header_t;

void init_obi_wan_com();
void obi_wan_listen();
uint8_t are_new_routine();
uint8_t obi_wan_timeout();
uint8_t obi_wan_restore();
void load_routine(message_header_t *header, routine_t* routines);
void send_routine_request_to_obi_wan(uint16_t cube_size);
void send_ack_to_obi_wan();

#endif /* OBI_WAN_H_ */
