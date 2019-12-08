/*******************************************************************************************************************************//**
 *
 * @file		DR_buttons.c
 * @brief		Inicializacion, seteo e implimentacion de los botones
 * @date		17 nov. 2019
 * @author
 *
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** INCLUDES
 **********************************************************************************************************************************/
#include "../inc/DR_buttons.h"
#include "../inc/DR_GPIO.h"

/***********************************************************************************************************************************
 *** DEFINES PRIVADOS AL MODULO
 **********************************************************************************************************************************/
#define BOUNCES 4
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
extern volatile button_state stop_button;
extern volatile button_state emergency_button;
extern volatile button_state knifes_tower_top;
extern volatile button_state base_init;
extern volatile button_state start_button;
extern volatile button_state base_end;
extern volatile button_state up_button;
extern volatile button_state down_button;
extern volatile button_state presence_button;

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
	\fn  init_buttons
	\brief Inicializacion de todos los pines que vamos a usar como botones
 	\author
 	\date
 	\param [in]
 	\param [out]
	\return
 */
void init_buttons() {
	GPIO_Pinsel ( CUBE_PRESENCE_BUTTON , PINSEL_GPIO );
	GPIO_Pinsel ( STOP_BUTTON , PINSEL_GPIO );
	GPIO_Pinsel ( EMERGENCY_BUTTON , PINSEL_GPIO );
	GPIO_Pinsel ( UP_BUTTON , PINSEL_GPIO );
	GPIO_Pinsel ( DOWN_BUTTON , PINSEL_GPIO );
	GPIO_Pinsel ( KNIFES_TOWER_TOP_BUTTON , PINSEL_GPIO );
	GPIO_Pinsel ( BASE_INIT_BUTTON , PINSEL_GPIO );
	GPIO_Pinsel ( RUN_BUTTON , PINSEL_GPIO);
	GPIO_Pinsel ( BASE_END_BUTTON , PINSEL_GPIO);

	GPIO_Dir ( CUBE_PRESENCE_BUTTON , INPUT );
	GPIO_Dir ( STOP_BUTTON , INPUT );
	GPIO_Dir ( UP_BUTTON , INPUT );
	GPIO_Dir ( DOWN_BUTTON , INPUT );
	GPIO_Dir ( EMERGENCY_BUTTON , INPUT );
	GPIO_Dir ( KNIFES_TOWER_TOP_BUTTON , INPUT );
	GPIO_Dir ( BASE_INIT_BUTTON , INPUT );
	GPIO_Dir ( RUN_BUTTON , INPUT );
	GPIO_Dir ( BASE_END_BUTTON , INPUT );

	GPIO_Mode ( CUBE_PRESENCE_BUTTON , PULLUP );
	GPIO_Mode ( STOP_BUTTON , PULLUP );
	GPIO_Mode ( UP_BUTTON , PULLUP );
	GPIO_Mode ( DOWN_BUTTON , PULLUP );
	GPIO_Mode ( EMERGENCY_BUTTON , PULLUP );
	GPIO_Mode ( KNIFES_TOWER_TOP_BUTTON , PULLUP );
	GPIO_Mode ( BASE_INIT_BUTTON , PULLUP );
	GPIO_Mode ( RUN_BUTTON , PULLUP );
	GPIO_Mode ( BASE_END_BUTTON , PULLUP );

}

/**
	\fn  button_state_hw
	\brief ******
 	\author
 	\date
 	\param [in]
 	\param [out]
	\return
 */
void button_state_hw( void )
{
	presence_button.state=!GPIO_Get(CUBE_PRESENCE_BUTTON);
	stop_button.state = !GPIO_Get(STOP_BUTTON);
	emergency_button.state = !GPIO_Get(EMERGENCY_BUTTON);
	knifes_tower_top.state = !GPIO_Get(KNIFES_TOWER_TOP_BUTTON);
	base_init.state = !GPIO_Get(BASE_INIT_BUTTON);
	start_button.state = !GPIO_Get(RUN_BUTTON);
	base_end.state = !GPIO_Get(BASE_END_BUTTON);
	up_button.state = !GPIO_Get(UP_BUTTON);
	down_button.state = !GPIO_Get(DOWN_BUTTON);

}

/**
	\fn  filter_bounce
	\brief  *******
 	\author
 	\date
 	\param [in]
 	\param [out]
	\return
 */
void filter_bounce(volatile button_state* button) {
	if (button->state && button->stable_states < BOUNCES) {
		button->stable_states++;
		button->current_state = OFF;
	}
	if (button->state && button->stable_states >= BOUNCES) {
		button->current_state = ON;
	}
	if (!button->state) {
		button->stable_states = 0;
		button->current_state = OFF;
	}
}

/**
	\fn  one_click
	\brief  funcion que limita a mandar un solo ON si es que se mantiene presionada la tecla
 	\author
 	\date
 	\param [in]
 	\param [out]
	\return
 */
void one_click(volatile button_state* button){
	if (button->state && button->stable_states != BOUNCES) {
		button->stable_states++;
		button->current_state = OFF;
	}
	if (button->state && button->stable_states == BOUNCES) {
		button->current_state = ON;
		button->stable_states++;
	}
	if (!button->state) {
		button->stable_states = 0;
		button->current_state = OFF;
	}
}

/**
	\fn  input_control
	\brief Controla si se activo algun boton
 	\author
 	\date
 	\param [in]
 	\param [out]
	\return
 */
void input_control(void) {
	button_state_hw();
	//one_click(&emergency_button);
	//one_click(&stop_button);
	//one_click(&start_button);
	filter_bounce(&stop_button);
	filter_bounce(&emergency_button);
	filter_bounce(&presence_button);
	filter_bounce(&knifes_tower_top);
	filter_bounce(&base_init);
	filter_bounce(&base_end);
	filter_bounce(&start_button);
	//filter_bounce(&down_button);
	//filter_bounce(&up_button);
	one_click(&up_button);
	one_click(&down_button);

}






