#include "PR_Towercontrol.h"
#include "PR_PWM.h"
#include "PR_timer.h"
#include "../inc/DR_tipos.h"

volatile uint16_t LastControl;
volatile uint16_t TowerPositionOld = 0;
volatile uint16_t TowerPosition = 0 ;

void Tower_Control ( uint16_t milimeters ){
	uint16_t aux = 0;
	LastControl = milimeters;
	TowerPositionOld += LastControl;
	moveteMotorPWM(TOWER_MOTOR, ON, 0);
	aux = startTimer( milimeters*STEP, update_tower , MICROSECONDS);
}


void update_tower( void )
{
	moveteMotorPWM(TOWER_MOTOR, OFF, 0);
	TowerPosition += LastControl;
}
