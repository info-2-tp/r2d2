//
// Created by marcelo on 2/11/19.
//

#ifndef STATES_MACHINE_STATES_H
#define STATES_MACHINE_STATES_H

#define 	PREPARE	0
#define 	LOAD	1
#define 	STOP	2
#define 	MEASURING	3
#define 	OBI_WAN_COM	4
#define 	PREPARE_CUT	5
#define 	PREPARE_SNIFE	6
#define 	CUTTING	7
#define 	CUT_RETURNING	8
/**
*	\fn void state_machine()
*	\brief Resumen
*	\details Detalles
*	\author
*	\date 03-11-2019 01:34:46
*/
void state_machine();

void init_state();
void prepare_state();
void load_state();
void stop_state();
void measuring_state();
void obi_wan_com_state();
void prepare_cut_state();
void prepare_knife_state();
void cutting_state();
void cut_returning_state();


#endif //STATES_MACHINE_STATES_H
