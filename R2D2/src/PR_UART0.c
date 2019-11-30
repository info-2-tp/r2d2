/*
 * PR_UART0.c
 *
 *  Created on: Oct 31, 2019
 *      Author: augusto
 */

#include "../inc/DR_UART0.h"
#include "../inc/PR_UART0.h"
#include <stdio.h>

uint8_t loading;
uint8_t* buffer_loading;
uint32_t buffer_size;
uint32_t buffer_loaded = 0;
/**
	\fn  int16_t PopRX( void )
	\brief saca un Byte en el buffer de recepcion
 	\author Ing. Marcelo Trujillo
 	\date 5 de oct. de 2017
 	\param void
	\return int16_t valor del dato o -1 por ERROR
*/
int16_t UART0_PopRX( void )
{
	int dato = -1;
	if (UART0_inRX != UART0_outRX){
		dato = (unsigned int) UART0_BufferRx[UART0_outRX];
		UART0_outRX++;
		UART0_outRX %= MAX_BUFF_RX;
	}

	return dato;
}

/**
	\fn  void PushTX( uint8_t dato )
	\brief pone un Byte en el buffer de transmicion
 	\author Ing. Marcelo Trujillo
 	\date 5 de oct. de 2017
 	\param [in] uint8_t dato Dato a guardar
	\return void
*/
void UART0_PushTX( uint8_t dato )
{
	UART0_BufferTx[UART0_inTX] = dato;
	UART0_inTX++;
	UART0_inTX %= MAX_BUFF_TX;

	if(UART0_flagTx == 0){
		UART0_flagTx = 1;
		setDataToBuffer(dato);
		UART0_outTX++;
		UART0_outTX %= MAX_BUFF_TX;
	}



}


//Super primitivas

void send(void * buffer, int size){
	char* aux = (char *)buffer;
	for(int i = 0; i<size;i++){
		UART0_PushTX((unsigned char)aux[i]);
	}
}

uint8_t receive_ready() {
	if (buffer_loaded == buffer_size) return 1;

	int dato = UART0_PopRX();
	if (dato == -1) return 0;
	buffer_loading[buffer_loaded] = (uint8_t)dato;
	buffer_loaded++;
	return buffer_loaded == buffer_size;
}

void receive(void * buffer, int size){
	buffer_loading = (uint8_t *)buffer;
	buffer_size = size;
	buffer_loaded=0;
}