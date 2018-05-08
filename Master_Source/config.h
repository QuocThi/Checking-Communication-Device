#ifndef config_H
#define config_H
#include  "stm32f10x.h"
#include 	"stm32f10x_exti.h"
#include 	"stm32f10x_gpio.h"
#include 	"stm32f10x_i2c.h"
#include 	"stm32f10x_spi.h"
#include 	"stm32f10x_rcc.h"
#include 	"stm32f10x_tim.h"
#include 	"stm32f10x_usart.h"
#include 	"misc.h"
#include  "delay.h"

//volatile char Mode;

#define sbi(PORT,PIN)		PORT |= (1<<PIN)
#define cbi(PORT,PIN)		PORT &= ~(1<<PIN)

#define SPIx_RCC				RCC_APB2Periph_SPI1
#define SPIx						SPI1
#define UART2_TX		GPIO_Pin_2
#define UART2_RX		GPIO_Pin_3
#define Led_Pin			GPIO_Pin_13
#define SPI_GPIO_RCC		RCC_APB2Periph_GPIOA
#define SPI_GPIO				GPIOA
#define SPI_PIN_MOSI		GPIO_Pin_7
#define SPI_PIN_MISO		GPIO_Pin_6
#define SPI_PIN_SCK			GPIO_Pin_5
#define SPI_PIN_SS			GPIO_Pin_4
#define Led_Test				GPIO_Pin_7

void Clock_reConfig();
void GPIO_Initt();
void Interrupt_Initt();
void I2C_Initt();
void SPI_Initt();
void TIM_Initt();
void UART_Initt(uint16_t baud);
void NVIC_Initt();
void UART_Send_String(char* data);
void External_Interrupt_Init();


#endif





