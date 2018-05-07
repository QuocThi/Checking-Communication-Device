#include "delay.h"

// For store tick counts in us
static __IO uint32_t usTicks;

// SysTick_Handler function will be called every 1 us
void SysTick_Handler()
{
	if (usTicks != 0)
	{
		usTicks--;
	}
}

void DelayInit()
{
	SystemCoreClockUpdate();
	SysTick_Config(SystemCoreClock / 1000);
}

void DelayMs(uint32_t ms)
{
	usTicks = ms;
	while (usTicks !=0){};
}