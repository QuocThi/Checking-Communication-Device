#include "delay.h"

// For store tick counts in us

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