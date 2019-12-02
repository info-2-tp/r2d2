/*******************************************************************************************************************************//**
 *
 * @file		${file_name}
 * @brief		Descripcion del modulo
 * @date		${date}
 * @author		Julian Mastroiacovo
 *
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** INCLUDES
 **********************************************************************************************************************************/
#include "PR_LCD.h"
#include "DR_LCD.h"

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
unsigned char numberVector[5];

// 0 para exito -1 para error
int8_t PushLCD (uint8_t dato , uint8_t control ){
	if( cantidadRestanteLCD >= MAX_BUFFER_LCD )
		return -1;

	Buffer_LCD[ inputIndexLCD ] = ( dato >> 4 ) & 0x0f;
	if( control == LCD_CONTROL )
		Buffer_LCD[ inputIndexLCD ] |= 0x80;

	inputIndexLCD++;

	Buffer_LCD[ inputIndexLCD ] = dato & 0x0f;
	if( control == LCD_CONTROL )
		Buffer_LCD[ inputIndexLCD ] |= 0x80;

	cantidadRestanteLCD += 2;

	inputIndexLCD++;
	inputIndexLCD %= MAX_BUFFER_LCD;

	return 0;
}

void PrintLCDNumber(uint32_t number) {
	unsigned char size = 0;
	unsigned char numberVector[5];

	if (number < 10) {
		PushLCD( '0' + number , LCD_DATA );
		return;
	}

	while(number != 0) {
		numberVector[size] = number%10 + '0';
		number = number/10;
		size++;
	}

	while(size) {
		size--;
		PushLCD( numberVector[size] , LCD_DATA );
	}
}

void PrintLCD_With_Number(uint8_t * message, uint8_t renglon, uint8_t posicion, uint32_t number) {
	PushLCD(CLEAR_DISPLAY, LCD_CONTROL);
	PushLCD( renglon + posicion + 0x80 , LCD_CONTROL );

	uint8_t state = 0;
	for( int i = 0 ; message[ i ] != '\0' ; i++ ){
		if (message[i] != '%' && message[i] != 'd') {
			state = 0;
			PushLCD( message[ i ] , LCD_DATA );
		}
		if (message[i] == 'd' && state != 1) {
			state = 0;
			PushLCD( message[ i ] , LCD_DATA );
		}
		if (message[i] == '%') {
			state = 1;
		}
		if (message[i] == 'd' && state == 1) {
			PrintLCDNumber(number);
			state = 0;
		}
	}
}

void PrintLCD(uint8_t * mensaje , uint8_t renglon , uint8_t posicion ){
	int i = 0;

	PushLCD(CLEAR_DISPLAY, LCD_CONTROL);
	PushLCD( renglon + posicion + 0x80 , LCD_CONTROL );

	for( i = 0 ; mensaje[ i ] != '\0' ; i++ ){
		PushLCD( mensaje[ i ] , LCD_DATA );
	}
}


/**
	\fn  Nombre de la Funcion
	\brief Descripcion
 	\author Julian Mastroiacovo
 	\date ${date}
 	\param [in] parametros de entrada
 	\param [out] parametros de salida
	\return tipo y descripcion de retorno
*/
