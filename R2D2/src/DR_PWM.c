#include "../inc/DR_GPIO.h"
#include "../inc/DR_PWM.h"

volatile Buffer_t buffer_salidas;

void Init_PWM()
{
	*DIR_PCONP |= (1<<6); 		// PowerPWM (PCPWM1 == 6)

	//PCLKSEL0 &= ~(3<<12);	//PCLK_PWM1 ponemos los 2 bits en 1 para habilitarlo

	PWM->PWM1PR = PWMPRESCALE;

	PWM->PWM1MR0 = PWM_DIVISOR; 	//1000us  period duration 	//PWM_DIVISOR de todos
	PWM->PWMMR0R = 1;				// Reset PWM TC on PWM1MR0 match
	PWM->PWMMR5S = 0;

	// update values in MR0
	PWM->LER_ENA0 = 1;

	//enable counters and PWM Mode
	PWM->TCR_PWMENA = 1;
	PWM->TCR_CountENA = 1;

	//colocamos los pwm todos en single edge
	//PWM->PWMSEL1 = 0;
	PWM->PWMSEL2 = 0;
	PWM->PWMSEL3 = 0;
	PWM->PWMSEL4 = 0;
	//PWM->PWMSEL5 = 0;
	//PWM->PWMSEL6 = 0;

	//EncenderPWM(PWM1,OFF);
	EncenderPWM(PWM2,OFF);
	EncenderPWM(PWM3,OFF);
	EncenderPWM(PWM4,OFF);
	//EncenderPWM(PWM5,OFF);
	//EncenderPWM(PWM6,OFF);

	// enable PWM OUTPUT (desde el modulo de hardware de PWM)
	//PWM->PWMENA1 = 1;
	PWM->PWMENA2 = 1;
	PWM->PWMENA3 = 1;
	PWM->PWMENA4 = 1;
	//PWM->PWMENA5 = 1;
	//PWM->PWMENA6 = 1;

	//GPIO_Pinsel(GPIO_PWM1,PINSEL_FUNC1); // configuro como: PWM1
	GPIO_Pinsel(GPIO_PWM2,PINSEL_FUNC1); // configuro como: PWM2
	GPIO_Pinsel(GPIO_PWM3,PINSEL_FUNC1); // configuro como: PWM3
	GPIO_Pinsel(GPIO_PWM4,PINSEL_FUNC1); // configuro como: PWM4
	//GPIO_Pinsel(GPIO_PWM5,PINSEL_FUNC1);   // configuro como: PWM5
	//GPIO_Pinsel(GPIO_PWM6,PINSEL_FUNC1); // configuro como: PWM6


	//GPIO DIRECCION MOTOR PASO A PASO TORRE
	GPIO_Pinsel(DIR_TORRE,PINSEL_GPIO); //configuro como pulso digital de entrada o salida
	//GPIO_Mode(DIR,PINMODE_PULLDOWN); //configuro el pin como pull up
	GPIO_Dir(DIR_TORRE,OUTPUT); //configuro el pin como salida

	//GPIO DIRECCION MOTOR PASO A PASO BASE
	GPIO_Pinsel(DIR_BASE,PINSEL_GPIO); //configuro como pulso digital de entrada o salida
	//GPIO_Mode(DIR,PINMODE_PULLDOWN); //configuro el pin como pull up
	GPIO_Dir(DIR_BASE,OUTPUT); //configuro el pin como salida

	//GPIO ENABLE PWM
	GPIO_Pinsel(ENABLE_PWM,PINSEL_GPIO); //configuro como pulso digital de entrada o salida
	//GPIO_Mode(ENABLE_PWM,PINMODE_PULLUP); //configuro el pin como pull up
	GPIO_Dir(ENABLE_PWM,OUTPUT); //configuro el pin como salida
}

void EncenderPWM(uint8_t motor, int estado)
{
	int pulso;
	if(estado == ON)
		pulso = 500;
	else
		pulso = 0;

	switch(motor)
	{
		case PWM1:
			PWM->PWM1MR1 = pulso;	//Update MR1 with new value
			PWM->LER_ENA1 = 1;			//Load the MR1 new value at start of next cycle
			break;

		case PWM2:
			PWM->PWM1MR2 = pulso;
			PWM->LER_ENA2 = 1;
			break;

		case PWM3:
			PWM->PWM1MR3 = pulso;
			PWM->LER_ENA3 = 1;
			break;

		case PWM4:
			PWM->PWM1MR4 = pulso;
			PWM->LER_ENA4 = 1;
			break;

		case PWM5:
			PWM->PWM1MR5 = pulso;
			PWM->LER_ENA5 = 1;
			break;

		case PWM6:
			PWM->PWM1MR6 = pulso;
			PWM->LER_ENA6 = 1;
			break;

		default:
			break;
	}
}