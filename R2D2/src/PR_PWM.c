#include "../inc/DR_outputs.h"
#include "../inc/PR_PWM.h"


void moveteMotorPWM(int motor,int estado,int direccion)
{
	switch(motor) {
	case PWM1:
		buffer_salidas.PWM_1 = estado;
		buffer_salidas.PWM_DIR_1 = direccion;
		break;
	case PWM2:
			buffer_salidas.PWM_2 = estado;
			buffer_salidas.PWM_DIR_2 = direccion;
			break;
	case PWM3:
			buffer_salidas.PWM_3 = estado;
			buffer_salidas.PWM_DIR_3 = direccion;
			break;
	case PWM4:
			buffer_salidas.PWM_4 = estado;
			buffer_salidas.PWM_DIR_4 = direccion;
			break;
	case PWM5:
			buffer_salidas.PWM_5 = estado;
			buffer_salidas.PWM_DIR_5 = direccion;
			break;
	case PWM6:
			buffer_salidas.PWM_6 = estado;
			buffer_salidas.PWM_DIR_6 = direccion;
			break;
	}
}

void turnOnPWM(uint8_t on) {
	moveteMotorPWM(TOWER_MOTOR, OFF, IZQ);
	moveteMotorPWM(BASE_MOTOR, OFF, IZQ);
	moveteMotorPWM(KNIFES_MOTOR, OFF, IZQ);
	buffer_salidas.PWM_ENABLE = on;
}
