#include "../inc/DR_systick.h"
#include "../inc/DR_buttons.h"
#include <stdio.h>

void SysTickInit (void)
{

	systick->STRELOAD = (systick->STCALIB / 4) -1;
	systick->STCURR = 0;

	systick->ENABLE   = 1;
	systick->TICKINT  = 1;
	systick->CLKSOURCE= 1;
}

void SysTick_Handler ( void )
{
	printf("Systick!\n");
	input_control();
}
