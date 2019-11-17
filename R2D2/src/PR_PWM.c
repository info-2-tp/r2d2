#include "../inc/DR_outputs.h"
#include "../inc/PR_PWM.h"


void moveteMotorPWM(int motor,int estado,int direccion)
{
	if (estado == 1)
	{
		GPIO_Set(2,7,0);//enable en  LOW
	}
	else
	{
		GPIO_Set(2,7,1);//enable en  LOW
	}

	if(!direccion)
	{
		GPIO_Set(4,28,0);//direccion del motor
	}
	else
	{
		GPIO_Set(4,28,1);//direccion del motor
	}

	buffer_salidas.PWM_2 = estado;
}
