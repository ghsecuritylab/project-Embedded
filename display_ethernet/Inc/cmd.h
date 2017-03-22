/*
 * cmd.h
 *
 *  Created on: 30-nov.-2016
 *  Author: Robbe Goovaerts
 */

#ifndef CMD_H_
#define CMD_H_

#include "stm32f7xx_hal.h"
#include "usb_device.h"
#include "stm32746g_discovery.h"
#include "stm32746g_discovery_lcd.h"
#include "stm32746g_discovery_sdram.h"
#include "stm32746g_discovery_ts.h"

#define MAX_LEN_GEHEUGEN 50



void voidLcdInit( void );
void voidCmdInit( void );
void voidCmdWrite( char *input, int len );
void voidRefreshDisplay( void );
char* pvcharCmdRead( UART_HandleTypeDef huart1 );


#endif /* CMD_H_ */
