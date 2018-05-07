#include "config.h"

char Data = 'A';

int main()
{
	GPIO_Initt();
	DelayInit();
	GPIO_ResetBits(GPIOC,GPIO_Pin_13);
	DelayMs(1000);
	GPIO_SetBits(GPIOC,GPIO_Pin_13);
	DelayMs(1000);
	UART_Initt();
	NVIC_Initt();
	USART_SendData(USART2,Data);
	while(1)
	{
	}
	return 0;
}