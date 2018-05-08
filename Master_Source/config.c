#include "config.h"

void GPIO_Initt()
{
	GPIO_InitTypeDef GPIO_Notify_Led;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	
	GPIO_Notify_Led.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Notify_Led.GPIO_Pin = Led_Pin;
	GPIO_Notify_Led.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&GPIO_Notify_Led);
}
/*********************************************** SPI1 Init *********************/
void SPI_Initt()
{
		// Initialization struct
		SPI_InitTypeDef SPI_InitStruct;
		GPIO_InitTypeDef GPIO_InitStruct;
	
	//RCC->CR = 0x00000001;
	
	// Step 1: Initialize SPI
	SPI_I2S_DeInit(SPIx);
	SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_128;
	SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStruct.SPI_Mode = SPI_Mode_Master;
	SPI_InitStruct.SPI_NSS = SPI_NSS_Soft | SPI_NSSInternalSoft_Set;
	SPI_Init(SPI1, &SPI_InitStruct); 
	SPI_Cmd(SPI1,ENABLE);
	
	// Step 2: Initialize GPIO
	RCC_APB2PeriphClockCmd(SPI_GPIO_RCC, ENABLE);
	// GPIO pins for MOSI, MISO, and SCK
	GPIO_InitStruct.GPIO_Pin = SPI_PIN_MOSI | SPI_PIN_MISO | SPI_PIN_SCK;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SPI_GPIO, &GPIO_InitStruct);
	// GPIO pin for SS
	GPIO_InitStruct.GPIO_Pin = SPI_PIN_SS;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SPI_GPIO, &GPIO_InitStruct);
	
	// Disable SPI slave device
	cbi(SPI_GPIO->BRR,SPI_PIN_SS);
}

char SPIx_Transfer(uint8_t data)
{
	// Write data to be transmitted to the SPI data register
	SPIx->DR = data; 
	// Wait until transmit complete
	while (!(SPIx->SR & (SPI_I2S_FLAG_TXE)));
	// Wait until receive complete
	while (!(SPIx->SR & (SPI_I2S_FLAG_RXNE)));
	// Wait until SPI is not busy anymore
	while (SPIx->SR & (SPI_I2S_FLAG_BSY));
	// Return received data from SPI data register
	return SPIx->DR;
}

void UART_Initt(uint16_t baud)
{
	RCC_APB1PeriphClockCmd(RCC_APB1ENR_USART2EN,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	USART_InitTypeDef UART2_Init;
	GPIO_InitTypeDef 	GPIO_UART_Init;
	
	/****************************** GPIO Init for UART*************************************************/
	GPIO_UART_Init.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_UART_Init.GPIO_Pin	 = UART2_TX | UART2_RX;
	GPIO_UART_Init.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_Init(GPIOA,&GPIO_UART_Init);
	
	/****************************** UART Initialization (Get from stm32f10x_usart.c)********************/
	UART2_Init.USART_BaudRate = baud;
  UART2_Init.USART_WordLength = USART_WordLength_8b;
  UART2_Init.USART_StopBits = USART_StopBits_1;
  UART2_Init.USART_Parity = USART_Parity_No ;
  UART2_Init.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  UART2_Init.USART_HardwareFlowControl = USART_HardwareFlowControl_None;  
	USART_Init(USART2,&UART2_Init);
	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);
	USART_Cmd(USART2,DISABLE);						// Enable when button interrupt UART press
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

void NVIC_Initt_UART()
{
	NVIC_InitTypeDef	NVIC_UART2_Init;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	NVIC_UART2_Init.NVIC_IRQChannel = USART2_IRQn;
	NVIC_UART2_Init.NVIC_IRQChannelPreemptionPriority =  0;
	NVIC_UART2_Init.NVIC_IRQChannelSubPriority = 1;
	NVIC_UART2_Init.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_UART2_Init);
}

/************************************************
	@brief Config External interrupt
*************************************************/

void Sel_Mode_UART()
{
	GPIO_InitTypeDef GPIO_Button_Init;
	EXTI_InitTypeDef Ext_Init;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	GPIO_Button_Init.GPIO_Mode = GPIO_Mode_IPD;								// IPD: Input Pull Down
	GPIO_Button_Init.GPIO_Pin = GPIO_Pin_8;
	GPIO_Button_Init.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_Button_Init);
	
	Ext_Init.EXTI_Line = EXTI_Line9;
	Ext_Init.EXTI_Mode = EXTI_Mode_Interrupt;
	Ext_Init.EXTI_Trigger = EXTI_Trigger_Falling;
	Ext_Init.EXTI_LineCmd = ENABLE;
	
}

void NVIC_Mode_Enable_UART()
{
	NVIC_InitTypeDef	NVIC_EXTI_Init;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	NVIC_EXTI_Init.NVIC_IRQChannel = EXTI9_5_IRQn;
	NVIC_EXTI_Init.NVIC_IRQChannelPreemptionPriority =  0;
	NVIC_EXTI_Init.NVIC_IRQChannelSubPriority = 2;
	NVIC_EXTI_Init.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_EXTI_Init);
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

