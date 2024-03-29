/*******************************************************************************************************************************//**
 *
 * @file	states.c
 * @brief	Maquina de estado mediante vector de ptr a funcion
 * @date	2/11/19.
 * @author	Marcelo Passut
 *
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** INCLUDES
 **********************************************************************************************************************************/
#include "../inc/actions.h"
#include "../inc/events.h"
#include "../inc/states.h"
#include "../inc/PR_Relay.h"
#include "../inc/PR_PWM.h"
#include "../inc/PR_timer.h"
#include "../inc/PR_HC_SR04.h"
#include "../inc/PR_UART0.h"
#include "../inc/PR_LCD.h"
#include "../inc/PR_Towercontrol.h"
#include "../inc/obi_wan.h"
#include <stdio.h>

/***********************************************************************************************************************************
 *** DEFINES PRIVADOS AL MODULO
 **********************************************************************************************************************************/
#define 	BASE_DISTANCE 490
#define 	KNIFE_DISTANCE 422

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
int current_state;
cuts_t cuts;
char current_cut = 0;
int cube_size = 0;
unsigned char has_data=0;
int32_t  measure_size;
message_header_t header;
routine_t routine[50];
uint8_t cube_sizes[50];
uint8_t blink=0;
int measuring_cube_size = 0;

uint16_t samples[1000];
uint16_t samples_size = 0;
int32_t measuring_timer = -1;

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
	\fn  LCD_Display
	\brief llamado a la funcion PrintLCD para mostrar algo en el LCD
 	\author
 	\date
 	\param [in]	string : texto a mostrar en el Display LCD
 	\param [in]	line : linea del display donde queremos mostrar el texto
 	\param [in]	pos : posicion donde queremos que inicie el texto, sirve para desplazarlo
 	\param [out]
	\return
 */
void LCD_Display(const char *string, unsigned char line ,unsigned char pos) {
	PrintLCD(string,line,pos);
}




void red_blink_off(void){
	set_color(SHUTDOWN);
	if(blink)startTimer(1000,red_blink_on, MILLISECONDS);
}

void red_blink_on(void){
	set_color(ROJO);
	if(blink)startTimer(1000,red_blink_off, MILLISECONDS);
}





/**

	\fn  reset_obiwan_data
	\brief Reinicia los flags de la comunicacion con obiwan
 	\author
 	\date
 	\param [in]
 	\param [out]
	\return
 */
void reset_obiwan_data(){
	has_data=0;
	measure_size=0;
}

/**
	\fn  measuring
	\brief Se encarga de medir el cubo en milimetros
 	\author
 	\date
 	\param [in]
 	\param [out]
	\return
 */
void measuring() {
	distance_t distance = getDistance(CM)*10;
	PrintLCD_With_Number("Distancia: %d", RENGLON_1, 0, distance);
	measure_size = BASE_DISTANCE - distance;
	if (measure_size > 0) {
		samples[samples_size++] = measure_size;
	}
	if (current_state == MEASURING)	measuring_timer = startTimer(100, measuring, MILLISECONDS);
}

int calculate_median_size() {
	int max = -1;
	int maxIndex = -1;
	for (uint16_t i = 0; i < samples_size; i++) {
		cube_sizes[samples[i]/10]++;
	}

	for (uint8_t i = 0; i < 50; i++) {
		if (cube_sizes[i] > max) {
			max = cube_sizes[i];
			maxIndex = i;
		}
	}
	return maxIndex*10;

}

/**
	\fn  *state_functions[]
	\brief llama a los cambios de estado
 	\author
 	\date
 	\param [in]
 	\param [out]
	\return
 */
void (*state_functions[])() = {prepare_state, load_state, stop_state, measuring_state, obi_wan_com_state,
		prepare_cut_state, prepare_knife_state, cutting_state, cut_returning_state,calibration_state};

/*char msj_state[]={"PREPARE","LOAD","STOP","MEASURING","OBI-WAN COM","PRE-CUT","PRE-KNIFE","CUTTING","RETURN CUT","CALIBRATION"};*/
/**
	\fn  stop_state
	\brief Con la maquina detenida, si apretamos el boton de run vuelve a inicializar la maquina
 	\author
 	\date
 	\param [in]
 	\param [out]
	\return
 */
void stop_state() {
	if ( know_run_button()) {
		init_machine();
	}

}

/*void emergency_state(){

	if ( know_run_button() && (base_front() && knifes_top())){
	    	current_state = LOAD;
	    }

    if (base_front()) base_stop();

    if (knifes_top()) knife_tower_stop();

     if (base_front() && knifes_top()) {
        	stop_all();
        }
}*/

/**
	\fn  prepare_state
	\brief Preparacion inicial de la maquina
 	\author
 	\date
 	\param [in]
 	\param [out]
	\return
 */
void prepare_state() {

	if (know_stop_button()) {
		stop_all();
		current_state = STOP;
		set_color(SHUTDOWN);
		set_color(ROJO);
		PrintLCD("STOP", RENGLON_2, 5);
		return;
	}

	if(know_emergency_button()){
		stop_all();
		init_machine();
		blink=1;
		red_blink_off();
		PrintLCD("EMERGENCY STOP", RENGLON_1, 1);
	}

	if (base_front()) base_stop();

	if (knifes_top()) knife_tower_stop();

	if ( base_front() && !knifes_top()) move_knife_tower_up();

	if (base_front() && knifes_top()) {
		stop_all();
		PrintLCD("Cargar cubos", RENGLON_1, 0);
		current_state = LOAD;
		PrintLCD("LOAD", RENGLON_2, 5);
		set_color(SHUTDOWN);
		set_color(VERDE);
	}
}

/**
	\fn  load_state
	\brief Espera la carga de un cubo y que se oprima un boton para comensar la medicion
 	\author
 	\date
 	\param [in]
 	\param [out]
	\return
 */
void load_state() {
	if (know_stop_button()) {
		stop_all();
		current_state = STOP;
		set_color(SHUTDOWN);
		set_color(ROJO);
		PrintLCD("STOP", RENGLON_2, 5);
		return;
	}

	if(know_emergency_button()){
		stop_all();
		init_machine();
		blink=1;
		red_blink_off();
		PrintLCD("EMERGENCY STOP", RENGLON_1, 1);
	}

	if ( know_run_button() && detect_cube()) {
		blink=0;
		measuring_timer = startTimer(100, measuring, MILLISECONDS);
		turnOnPWM(ON);
		move_base_back();
		for( uint8_t i = 0; i < 50; i++) {
			cube_sizes[i] = 0;
		}
		current_state = MEASURING;
		PrintLCD("MEASURING", RENGLON_2, 3);
	}
}

/**
	\fn  measuring_state
	\brief Realiza movimiento de la plataforma para medir la altura del cubo
 	\author
 	\date
 	\param [in]
 	\param [out]
	\return
 */
void measuring_state() {
	if(know_emergency_button()){
		stop_all();
		init_machine();
		blink=1;
		red_blink_off();
		PrintLCD("EMERGENCY STOP", RENGLON_1, 1);
	}

	if (know_stop_button()) {
		stop_all();
		current_state = STOP;
		set_color(SHUTDOWN);
		set_color(ROJO);
		PrintLCD("STOP", RENGLON_2, 5);
		return;
	}

	if(!detect_cube()){
		stop_all();
		current_state = STOP;
		PrintLCD("ERROR NOT CUBE",RENGLON_1,1);
		PrintLCD("STOP",RENGLON_2,5);
		set_color(SHUTDOWN);
		set_color(ROJO);
	}



	if(base_back()){
		killTimer(measuring_timer);
		measuring_timer = -1;
		measuring_cube_size = calculate_median_size();
		samples_size = 0;
		base_stop();
		if (measuring_cube_size <= 0) {
			measuring_cube_size = 0;
			init_machine();
			current_state = PREPARE;
			PrintLCD("PREPARE", RENGLON_2, 4);
			PrintLCD("No hay cubo", RENGLON_1, 0);
		}
		if (measuring_cube_size > 0 ) {
			move_base_front();
			cube_size = measuring_cube_size;
			send_routine_request_to_obi_wan(cube_size);
			measuring_cube_size = 0;
			receive((void*)&header,sizeof(header));
			current_state = OBI_WAN_COM;
			PrintLCD_With_Number("Cubo de %dmm", RENGLON_1, 0, cube_size);
			PrintLCD("OBI WAN COM", RENGLON_2, 2);
			set_color(SHUTDOWN);
			set_color(VERDE);
		}
	}
}

/**
	\fn  obi_wan_com_state
	\brief Comunicacion de R2D2 por UART con obi wan, y control de el dato transmitido
 	\author
 	\date
 	\param [in]
 	\param [out]
	\return
 */
void obi_wan_com_state() {
	if (know_stop_button()) {
		stop_all();
		current_state = STOP;
		set_color(SHUTDOWN);
		set_color(ROJO);
		PrintLCD("STOP", RENGLON_2, 5);
		return;
	}

	if(!detect_cube()){
		stop_all();
		current_state = STOP;
		PrintLCD("ERROR NOT CUBE",RENGLON_1,1);
		PrintLCD("STOP",RENGLON_2,5);
		set_color(SHUTDOWN);
		set_color(ROJO);
	}

	if(know_emergency_button()){
		stop_all();
		init_machine();
		blink=1;
		red_blink_off();
		PrintLCD("EMERGENCY STOP", RENGLON_1, 1);
	}

	if (base_front()) {
		stop_all();
	}

	if (are_new_routine()) {
    	has_data = READY;
    	load_routine(&header, &routine);
    	cuts = calculate_cuts(routine,header.size/sizeof(routine_t));
    }

	if (has_data && (header.size/sizeof(routine_t)) == 0){
		init_machine();
		PrintLCD("No hay trabajos", RENGLON_1, 0);
		current_state = PREPARE;
		PrintLCD("PREPARE", RENGLON_2, 4);
		reset_obiwan_data();
		return;
	}

	if (has_data && (header.size/sizeof(routine_t)) > 0 && base_front()) {
		turnOnPWM(ON);
		Tower_Control(KNIFE_DISTANCE - cube_size,DOWN);
		reset_obiwan_data();
		current_state = PREPARE_CUT;
		PrintLCD("PREPARE CUT", RENGLON_2, 2);
		set_color(SHUTDOWN);
		set_color(AZUL);
		return;
	}

	if(id_timer<0){
		id_timer = startTimer(OBI_WAN_TTL,obiwan_ttl,SECONDS);
	}

	if(obiwan_timeout){
		PrintLCD("Sin comunicacion", RENGLON_1, 0);
		init_machine();
		current_state = PREPARE;
		PrintLCD("PREPARE", RENGLON_2, 4);
		reset_obiwan_data();
		return;
	}
}

/**
	\fn  prepare_cut_state
	\brief Controla que la cuchilla llego a la posicion de corte
 	\author
 	\date
 	\param [in]
 	\param [out]
	\return
 */
void prepare_cut_state() {
	if (know_stop_button()) {
		stop_all();
		current_state = STOP;
		set_color(SHUTDOWN);
		set_color(ROJO);
		PrintLCD("STOP", RENGLON_2, 5);
		return;
	}

	if(!detect_cube()){
		stop_all();
		current_state = STOP;
		PrintLCD("ERROR NOT CUBE",RENGLON_1,1);
		PrintLCD("STOP",RENGLON_2,5);
		set_color(SHUTDOWN);
		set_color(ROJO);
	}

	if(know_emergency_button()){
		stop_all();
		init_machine();
		blink=1;
		red_blink_off();
		PrintLCD("EMERGENCY STOP", RENGLON_1, 1);
	}

	if (TowerPosition == (KNIFE_DISTANCE - cube_size)) {
		move_base_back();
		startTimer(2500, base_stop, MILLISECONDS);
		current_state = CALIBRATION;
		PrintLCD("CALIBRATE", RENGLON_2, 4);
		set_color(SHUTDOWN);
		set_color(AZUL);

	}
}

void calibration_state(){
	if(!detect_cube()){
		stop_all();
		current_state = STOP;
		PrintLCD("ERROR NOT CUBE",RENGLON_1,1);
		PrintLCD("STOP",RENGLON_2,5);
		set_color(SHUTDOWN);
		set_color(ROJO);
	}

	if (know_stop_button()) {
		stop_all();
		current_state = STOP;
		set_color(SHUTDOWN);
		set_color(ROJO);
		PrintLCD("STOP", RENGLON_2, 5);
		return;
	}
	if(know_emergency_button()){
		stop_all();
		init_machine();
		blink=1;
		red_blink_off();
		PrintLCD("EMERGENCY STOP", RENGLON_1, 1);
	}
	if(know_up_button()){
		Tower_Control(1, UP);
	}

	if(know_down_button()){
		Tower_Control(1, DOWN);
	}

	if(know_run_button()){
		Tower_Control(cuts.positions[current_cut],DOWN);
		next_cut();
		current_state = PREPARE_SNIFE;
		PrintLCD("PREPARE SNIFE", RENGLON_2, 1);
		set_color(SHUTDOWN);
		set_color(AZUL);
	}
}
/**
	\fn  prepare_knife_state
	\brief Cuando la cuchilla esta en la posicion de corte empieza el movimiento de corte
 	\author
 	\date
 	\param [in]
 	\param [out]
	\return
 */
void prepare_knife_state() {
	if (know_stop_button()) {
		stop_all();
		current_state = STOP;
		set_color(SHUTDOWN);
		set_color(ROJO);
		PrintLCD("STOP", RENGLON_2, 5);
		return;
	}

	if(!detect_cube()){
		stop_all();
		current_state = STOP;
		PrintLCD("ERROR NOT CUBE",RENGLON_1,1);
		PrintLCD("STOP",RENGLON_2,5);
		set_color(SHUTDOWN);
		set_color(ROJO);
	}

	if(know_emergency_button()){
		stop_all();
		init_machine();
		blink=1;
		red_blink_off();
		PrintLCD("EMERGENCY STOP", RENGLON_1, 1);
	}

	if (TowerPosition == TowerPositionOld) {
		knife_tower_stop();
		knifes_run();
		move_base_back();
		PrintLCD_With_Number("Cortando %dmm", RENGLON_1, 0, cuts.positions[current_cut - 1]);
		current_state = CUTTING;
		PrintLCD("CUTTING", RENGLON_2, 4);
		set_color(SHUTDOWN);
		set_color(AZUL);
	}
}

/**
	\fn  cutting_state
	\brief Analiza que se cumplan todas las condiciones y hace retornar la plataforma al inicio
 	\author
 	\date
 	\param [in]
 	\param [out]
	\return
 */
void cutting_state() {
	if (know_stop_button()) {
		stop_all();
		current_state = STOP;
		set_color(SHUTDOWN);
		set_color(ROJO);
		PrintLCD("STOP", RENGLON_2, 5);
		return;
	}

	if(!detect_cube()){
		stop_all();
		current_state = STOP;
		PrintLCD("ERROR NOT CUBE",RENGLON_1,1);
		PrintLCD("STOP",RENGLON_2,5);
		set_color(SHUTDOWN);
		set_color(ROJO);
	}

	if(know_emergency_button()){
		stop_all();
		init_machine();
		blink=1;
		red_blink_off();
		PrintLCD("EMERGENCY STOP", RENGLON_1, 1);
	}

	if (base_back()) {
		move_base_front();
		current_state = CUT_RETURNING;
		PrintLCD("CUT RETURNING", RENGLON_2, 1);
		set_color(SHUTDOWN);
		set_color(AZUL);
	}
}

/**
	\fn  cut_returning_state
	\brief Analiza que se cumplan todas las condiciones y empieza todo el movimiento para el siguiente corte
 	\author
 	\date
 	\param [in]
 	\param [out]
	\return
 */
void cut_returning_state() {
	if (know_stop_button()) {
		stop_all();
		current_state = STOP;
		set_color(SHUTDOWN);
		set_color(ROJO);
		PrintLCD("STOP", RENGLON_2, 5);
		return;
	}

	if(!detect_cube()){
		stop_all();
		current_state = STOP;
		PrintLCD("ERROR NOT CUBE",RENGLON_1,1);
		PrintLCD("STOP",RENGLON_2,5);
		set_color(SHUTDOWN);
		set_color(ROJO);
	}

	if(know_emergency_button()){
		stop_all();
		init_machine();
		blink=1;
		red_blink_off();
		PrintLCD("EMERGENCY STOP", RENGLON_1, 1);
	}

	if (base_front()) {
		//
		base_stop();
		if (current_cut == cuts.cuts) {
			send_ack_to_obi_wan();
			PrintLCD("I will Back >:(", RENGLON_1, 0);
			reset_tower();
			reset_cut();
			init_machine();
			current_state = PREPARE;
		} else {
			Tower_Control(cuts.positions[current_cut],DOWN);
			next_cut();
			current_state = PREPARE_SNIFE;
			PrintLCD("PREPARE SNIFE", RENGLON_2, 1);
			set_color(SHUTDOWN);
			set_color(AZUL);
		}
	}
}

/**
	\fn  state_machine
	\brief Funcion que llama al estado actual segun indique current_state
 	\author
 	\date
 	\param [in]
 	\param [out]
	\return
 */
void state_machine() {
    state_functions[current_state]();
    obi_wan_com();
}

/**
	\fn  init_machine
	\brief Acciona todos los movimientos iniciales para la preparacion de la maquina
 	\author
 	\date
 	\param [in]
 	\param [out]
	\return
 */
void init_machine() {
	turnOnPWM(ON);
	knifes_stop();
	move_base_front();
	current_state = PREPARE;
	PrintLCD("PREPARE", RENGLON_2, 4);
}

void reset_measuring() {
	samples_size = 0;
	cube_size = 0;
	measuring_cube_size = 0;
}

void resetAll() {
	reset_measuring();
	reset_obiwan_data();
	reset_cut();
	reset_tower();
}

void obi_wan_com() {
	obi_wan_listen();
	if (obi_wan_timeout()) {
		resetAll();
		if (current_state != STOP) {
			init_machine();
		}
		PrintLCD("No hay comunicacion", RENGLON_1, 0);
	}

	if (obi_wan_restore()) {
		PrintLCD("Obi Wan Listo!", RENGLON_1, 0);
	}
}


