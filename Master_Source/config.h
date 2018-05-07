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

#define sbi(PORT,PIN)		PORT |= (1<<PIN)
#define cbi(PORT,PIN)		PORT &= ~(1<<PIN)

void Clock_reConfig();
void GPIO_Initt();
void Interrupt_Initt();
void I2C_Initt();
void SPI_Initt();
void TIM_Initt();
void UART_Initt();
void NVIC_Initt();
void UART_Send_String(char* data);

#endif





