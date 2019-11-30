/*
 * PR_UART0.h
 *
 *  Created on: Oct 31, 2019
 *      Author: augusto
 */

#ifndef PR_UART0_H_
#define PR_UART0_H_

#include "DR_tipos.h"

int16_t UART0_PopRX( void );
void UART0_PushTX( uint8_t dato );
void send(void * buffer, int size);
void receive(void * buffer, int size);
uint8_t receive_ready();

#endif /* PR_UART0_H_ */
