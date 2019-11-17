#include "../inc/DR_outputs.h"
#include "../inc/DR_tipos.h"
#include "../inc/DR_GPIO.h"


void controlarSalidas()
{
	if((buffer_salidas.PWM_1))
	{
		EncenderPWM(PWM1,ON);

	}
	else if(!buffer_salidas.PWM_1)
	{
		EncenderPWM(PWM1,OFF);
	}
	if(buffer_salidas.PWM_2)
	{
		EncenderPWM(PWM2,ON);
	}
	else if(!buffer_salidas.PWM_2)
	{
		EncenderPWM(PWM2,OFF);
	}
	if(buffer_salidas.PWM_3)
	{
		EncenderPWM(PWM3,ON);
	}
	else if(!buffer_salidas.PWM_3)
	{
		EncenderPWM(PWM3,OFF);
	}
	if(buffer_salidas.PWM_4)
	{
		EncenderPWM(PWM4,ON);
	}
	else if(!buffer_salidas.PWM_4)
	{
		EncenderPWM(PWM4,OFF);
	}
	if(buffer_salidas.PWM_5)
	{
		EncenderPWM(PWM5,ON);
	}
	else if(!buffer_salidas.PWM_5)
	{
		EncenderPWM(PWM5,OFF);

	}
	if(buffer_salidas.PWM_6)
	{
		EncenderPWM(PWM6,ON);
	}
	else if(!buffer_salidas.PWM_6)
	{
		EncenderPWM(PWM6,OFF);
	}
}
