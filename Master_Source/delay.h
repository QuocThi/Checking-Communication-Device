#ifndef __DELAY_H
#define __DELAY_H
 
#ifdef __cplusplus
extern "C" {
#endif
 
#include "config.h"
 static __IO uint32_t usTicks;
	
void DelayInit(void);
//void DelayUs(uint32_t us);
void DelayMs(uint32_t ms);
 
#ifdef __cplusplus
}
#endif
 
#endif