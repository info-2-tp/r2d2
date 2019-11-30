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
#include "DR_tipos.h"
#include "DR_LCD.h"
#include "DR_GPIO.h"

/***********************************************************************************************************************************
 *** DEFINES PRIVADOS AL MODULO
 **********************************************************************************************************************************/
#define		LCD_D4		0,5	 //pines asociados a los datos del LCD
#define		LCD_D5		0,10 
#define		LCD_D6		2,4	 
#define		LCD_D7		2,5	 

#define		LCD_RS		2,6  // pin de RS
#define		LCD_E		0,4  // pin del ENABLE
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
volatile uint8_t demoraLCD; 
volatile uint8_t Buffer_LCD[ MAX_BUFFER_LCD ]; //buffer
volatile uint8_t inputIndexLCD; 
volatile uint8_t outputIndexLCD; 
volatile uint8_t cantidadRestanteLCD; 
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

void InitLCD ( void ){

	int i=0;

	//seteo los pines en gpio
	GPIO_Pinsel(LCD_D4,PINSEL_GPIO);
	GPIO_Pinsel(LCD_D5,PINSEL_GPIO);
	GPIO_Pinsel(LCD_D6,PINSEL_GPIO);
	GPIO_Pinsel(LCD_D7,PINSEL_GPIO);

	GPIO_Pinsel(LCD_RS,PINSEL_GPIO);
	GPIO_Pinsel(LCD_E,PINSEL_GPIO);

	//seteo los gpio en salida
	GPIO_Dir(LCD_D4,OUTPUT);
	GPIO_Dir(LCD_D5,OUTPUT);
	GPIO_Dir(LCD_D6,OUTPUT);
	GPIO_Dir(LCD_D7,OUTPUT);

	GPIO_Dir(LCD_RS,OUTPUT);
	GPIO_Dir(LCD_E,OUTPUT);

	//pongo en off el pin enable
	GPIO_Set(LCD_E,OFF);

	//espero 15ms como indica la inicializacion del LCD (con un extra de tiempo por las dudas)
	demoraLCD = 25;
	while(demoraLCD)

	//primeros tres comandos requeridos por las instrucciones
	for( i = 0 ; i < 3 ; i++ ){
		GPIO_Set ( LCD_D4 , ON );
		GPIO_Set ( LCD_D5 , ON );
		GPIO_Set ( LCD_D6 , OFF );
		GPIO_Set ( LCD_D7 , OFF );
		GPIO_Set ( LCD_RS , OFF );
		GPIO_Set ( LCD_E , ON );
		GPIO_Set ( LCD_E , OFF );

		demoraLCD = 5;
		while( demoraLCD );
	}

	GPIO_Set ( LCD_D4 , OFF );
	GPIO_Set ( LCD_D5 , ON );
	GPIO_Set ( LCD_D6 , OFF );
	GPIO_Set ( LCD_D7 , OFF );
	GPIO_Set ( LCD_RS , OFF );
	GPIO_Set ( LCD_E , ON );
	GPIO_Set ( LCD_E , OFF );

	demoraLCD = 2;
	while( demoraLCD );

	//ultima configuracion
	PushLCD( SET_FUNCTION_4BITS , LCD_CONTROL );	//N = 1; 2 lineas; 5x7 puntos
	PushLCD( CURSOR_OFF , LCD_CONTROL );	//Cursor off
	PushLCD( CLEAR_DISPLAY , LCD_CONTROL );	//Clear display
	PushLCD( CURSOR_INCREMENT , LCD_CONTROL );	//incrementa puntero
	PushLCD( ACTIVATE , LCD_CONTROL );	//Activo LCD

	//pierdo tiempo
	demoraLCD = 2;
	while(demoraLCD);
	demoraLCD = 2;
	while(demoraLCD);
	demoraLCD = 2;
	while(demoraLCD);
	demoraLCD = 2;
	while(demoraLCD);
	demoraLCD = 2;
	while(demoraLCD);
}

//devuelve -1 por error
uint16_t popBufferLCD ( void ){
	int16_t dato;

	if( cantidadRestanteLCD == 0 )
		return -1;

	dato = Buffer_LCD[ outputIndexLCD ];

	cantidadRestanteLCD--;

	outputIndexLCD++;
	outputIndexLCD %= MAX_BUFFER_LCD;

	return dato;
}
void sendLCD ( void ){
	int16_t data;

	if( ( data = popBufferLCD( ) ) == -1 )
		return;

	GPIO_Set ( LCD_D4 , ( ( (uint8_t) data ) >> 0 ) & 0x01 );
	GPIO_Set ( LCD_D5 , ( ( (uint8_t) data ) >> 1 ) & 0x01 );
	GPIO_Set ( LCD_D6 , ( ( (uint8_t) data ) >> 2 ) & 0x01 );
	GPIO_Set ( LCD_D7 , ( ( (uint8_t) data ) >> 3 ) & 0x01 );

	if( ( (uint8_t) data ) & 0x80 )
		GPIO_Set( LCD_RS , 0 );
	else
		GPIO_Set( LCD_RS , 1 );

	GPIO_Set ( LCD_E , 1 );
	GPIO_Set ( LCD_E , 0 );
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
