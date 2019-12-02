#include "../inc/DR_Relay.h"
#include "../inc/DR_GPIO.h"

uint8_t buffer_relay[4];
void init_Relay(void)
{
	GPIO_Pinsel(RELAY0, PINSEL_GPIO);
	GPIO_Dir( RELAY0 , OUTPUT);
	GPIR_Set(RELAY0,OFF);
}

void active_relay(void)
{
	int i;
	for(i = 0; i < 4; i++)
	{
		if (buffer_relay[i])
		{
			switch(i)
			{
			case 0:
				GPIO_Set(2,0,1); //2 es el puerto del relay 0 que usamos para la base y 0 el pin del relay, lo encendemos
				break;
			case 1:
				GPIO_Set(0,23,1);
				break;
			case 2:
				GPIO_Set(0,21,1); //2 es el puerto del relay 0 que usamos para la sierra y 21 el pin del relay, lo encendemos
				break;
			}
		}
		else if(!buffer_relay[i])
		{
			switch(i)
			{
			case 0:
				GPIOs[2].FIOCLR = (1<<0);//2 es el puerto del relay 0 que usamos para la base y 0 el pin del relay, lo apagamos
				break;
			case 1:
				GPIOs[0].FIOCLR = (1<<23);//0 es el puerto del relay 1 que usamos para la torre y 23 el pin del relay, lo apagamos
				break;
			case 2:
				GPIOs[0].FIOCLR = (1<<21);//2 es el puerto del relay 0 que usamos para la sierra y 21 el pin del relay, lo apagamos
				break;
			}
		}
	}
}

