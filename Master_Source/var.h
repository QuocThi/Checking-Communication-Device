#ifndef __VAR_H
#define __VAR_H

#ifdef __cplusplus
 extern "C" {
#endif
/*-------------------------------------------------------------------------------*/
#define RX_SIZE	20
#define TX_SIZE 8
extern uint8_t rxBuf[RX_SIZE];
extern uint8_t txBuf[TX_SIZE];
extern uint8_t rxFlag;
	 extern uint8_t Ticks_1s;
/*-------------------------------------------------------------------------------*/	 
#ifdef __cplusplus
}
#endif

#endif /* __MISC_H */

