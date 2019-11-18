#include "../inc/DR_outputs.h"
#include "../inc/DR_tipos.h"
#include "../inc/DR_GPIO.h"

#define ENABLE_PWM	2,7			//Exp0

void controlarSalidas()
{
	GPIO_Set(ENABLE_PWM, !buffer_salidas.PWM_ENABLE);

	EncenderPWM(PWM1, buffer_salidas.PWM_1);
	EncenderDireccionPWM(PWM1,buffer_salidas.PWM_DIR_1);

	EncenderPWM(PWM2, buffer_salidas.PWM_2);
	EncenderDireccionPWM(PWM2,buffer_salidas.PWM_DIR_2);

	EncenderPWM(PWM3, buffer_salidas.PWM_3);
	EncenderDireccionPWM(PWM3,buffer_salidas.PWM_DIR_3);

	EncenderPWM(PWM4, buffer_salidas.PWM_4);
	EncenderDireccionPWM(PWM4,buffer_salidas.PWM_DIR_4);

	EncenderPWM(PWM5, buffer_salidas.PWM_5);
	EncenderDireccionPWM(PWM5,buffer_salidas.PWM_DIR_5);

	EncenderPWM(PWM6, buffer_salidas.PWM_6);
	EncenderDireccionPWM(PWM6,buffer_salidas.PWM_DIR_6);


}
