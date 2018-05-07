#include "config.h"

void GPIO_Initt()
{
	GPIO_InitTypeDef GPIO_Notify_Led;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	
	GPIO_Notify_Led.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Notify_Led.GPIO_Pin = GPIO_Pin_13;
	GPIO_Notify_Led.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&GPIO_Notify_Led);
}

void UART_Initt()
{
	RCC_APB1PeriphClockCmd(RCC_APB1ENR_USART2EN,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	USART_InitTypeDef UART2_Init;
	GPIO_InitTypeDef 	GPIO_UART_Init;
	
	/****************************** GPIO Init for UART*************************************************/
	GPIO_UART_Init.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_UART_Init.GPIO_Pin	 = GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_UART_Init.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_Init(GPIOA,&GPIO_UART_Init);
	
	/****************************** UART Initialization (Get from stm32f10x_usart.c)********************/
	UART2_Init.USART_BaudRate = 115200;
  UART2_Init.USART_WordLength = USART_WordLength_8b;
  UART2_Init.USART_StopBits = USART_StopBits_1;
  UART2_Init.USART_Parity = USART_Parity_No ;
  UART2_Init.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  UART2_Init.USART_HardwareFlowControl = USART_HardwareFlowControl_None;  
	USART_Init(USART2,&UART2_Init);
	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);
	USART_Cmd(USART2,ENABLE);
}

void UART_Send_String(char *data)
{
	int i=0;
	while(data[i] != '\0')
	{
			USART2->DR = (data[i] & (uint16_t)0x01FF);
			//USART_SendData(USART2,data[i]);
		while(USART_GetITStatus(USART2,USART_IT_TC) != RESET){;}
			i++;
			/****************************** Delay for transmit *************************************/
			DelayMs(2);										
	}
}

void NVIC_Initt()
{
	NVIC_InitTypeDef	NVIC_UART2_Init;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	NVIC_UART2_Init.NVIC_IRQChannel = USART2_IRQn;
	NVIC_UART2_Init.NVIC_IRQChannelPreemptionPriority =  0;
	NVIC_UART2_Init.NVIC_IRQChannelSubPriority = 10;
	NVIC_UART2_Init.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_UART2_Init);
}

/************************************************
	@brief Handling UART 2 receive interrupt
*************************************************/
void USART2_IRQHandler(void)
{
	uint8_t receivedChar;  
   if (USART_GetITStatus(USART2,USART_IT_RXNE) != RESET)           
   {          
        receivedChar = USART_ReceiveData(USART2);	 
    }       
	  USART_ClearITPendingBit(USART2,USART_IT_RXNE);
}

/************************************************
	@brief Re-config the system clock as PLL clock derived from HSI clock source (8MHz).
*************************************************/
void Clock_reConfig()
{
	RCC_HSICmd(ENABLE);
	while(RCC_GetITStatus(RCC_IT_HSIRDY) == RESET);
	
	RCC_SYSCLKConfig(RCC_SYSCLKSource_HSI);
	RCC_HCLKConfig(RCC_SYSCLK_Div1);
	RCC_PCLK1Config(RCC_HCLK_Div1);
	RCC_PCLK2Config(RCC_HCLK_Div1);
	
	RCC_PLLCmd(DISABLE);
	RCC_PLLConfig(RCC_PLLSource_HSI_Div2,RCC_PLLMul_2);
	RCC_PLLCmd(ENABLE);
	while(RCC_GetITStatus(RCC_IT_PLLRDY) == RESET);
	RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
	RCC_HCLKConfig(RCC_SYSCLK_Div1);
	RCC_PCLK1Config(RCC_HCLK_Div2);
	RCC_PCLK2Config(RCC_HCLK_Div1);
	SystemCoreClockUpdate();
}

