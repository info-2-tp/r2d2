/*******************************************************************************************************************************//**
 *
 * @file		DR_UART.c
 * @brief		Descripcion del modulo
 * @date		12 de oct. de 2017
 * @author		Ing. Marcelo Trujillo
 *
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** INCLUDES
 **********************************************************************************************************************************/
#include "../inc/DR_UART0.h"
#include "../inc/DR_gpio.h"
#include "../inc/DR_PLL.h"

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
volatile uint8_t UART0_BufferRx[MAX_BUFF_RX];
volatile uint8_t UART0_BufferTx[MAX_BUFF_TX];
volatile uint8_t UART0_inRX , UART0_outRX , UART0_inTX, UART0_outTX;
volatile int16_t UART0_flagTx;

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
	\fn  void UART0_IRQHandler (void)
	\brief Interrupcion de UART0
 	\author Ing. Marcelo Trujillo
 	\date 31 de ago. de 2016
 	\param void
	\return void
*/
void UART0_IRQHandler (void)
{
	unsigned char iir,aux;
	int dato;
	do{
		iir= UART0IIR;
		aux=(iir>>1)& 0x07;


		if(aux & 0x01){//estoy transmitiendo un byte

			dato = UART0_PopTX();
			setDataToBuffer(dato);
		}

		else if(aux & 0x02){//estoy recibiendo un byte
			dato = UART0RBR;
			UART0_PushRX(dato);
		}
		else{
			UART0_flagTx = 0;
		}

	}while(!(iir & 0x01 ));

}

/**
	\fn  	void UART0_Inicializacion ( uint32_t baudios )
	\brief 	Inicializacion de UART0
 	\author Ing. Marcelo Trujillo
 	\date 	31 de ago. de 2016
	\return void
*/
void UART0_Inicializacion ( void )
{
	//energizo
	PCONP|=0x01<<3;

	//selecciono el clock
	DIR_PCLK_UART0 &= ~(0x03<<6);

	//seteo la cantidad de bytes para la transmicion y recepcion
	UART0LCR= 0x00000083;
	UART0DLM= 0;
	UART0DLL= 0xA3;

	//Selecciono los pines y el modo
	GPIO_Pinsel(RX0, RXD);//pongo el pin en modo receptor
	GPIO_Pinsel(TX0, TXD);//pongo el pin en modo transmisor

	UART0LCR= 0x03;
	//interrupciones
	UART0IER= 0x03;//activo la interrupcion desde el modulo uart0
	ISER0|=(1<<5); //activo la interrupcion desde el micro

}
/**
	\fn  void PushRX( uint8_t dato )
	\brief pone un Byte en el buffer de recepcion
 	\author Ing. Marcelo Trujillo
 	\date 5 de oct. de 2017
 	\param [in] uint8_t dato Dato a guardar
	\return void
*/
void UART0_PushRX( uint8_t dato )
{
	UART0_BufferRx[UART0_inRX]= dato;
	UART0_inRX++;
	UART0_inRX%=MAX_BUFF_RX;
}

/**
	\fn int16_t PopTX( void )
	\brief saca un Byte en el buffer de transmicion
 	\author Ing. Marcelo Trujillo
 	\date 5 de oct. de 2017
 	\param void
	\return int16_t valor del dato o -1 por ERROR
*/
int16_t UART0_PopTX( void )
{
	int dato = -1;
		if(UART0_inTX != UART0_outTX){
			dato = (unsigned int) UART0_BufferTx[UART0_outTX];
			UART0_outTX++;
			UART0_outTX %= MAX_BUFF_TX;
		}
	return dato;
}



void setDataToBuffer(short dato){
	//dato = UART0_PopTX();
				if(dato >= 0){
					UART0THR = (unsigned char)dato;
				}
				else{
					UART0_flagTx =0;
				}
}
