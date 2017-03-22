/*
 * bestelUI.h
 *
 *  Created on: 4-dec.-2016
 *      Author: robbe
 */

#ifndef BESTELUI_H_
#define BESTELUI_H_

#include "stm32f7xx_hal.h"
#include "usb_device.h"
#include "stm32746g_discovery.h"
#include "stm32746g_discovery_lcd.h"
#include "stm32746g_discovery_sdram.h"
#include "stm32746g_discovery_ts.h"
#include "string.h"

typedef struct hoofdmenu_struct
{
	char cData[30];
	int16_t sId;
	int16_t sSubID;
	int16_t sStartX;
	int16_t sStartY;
	int16_t sBreedte;
	int16_t sHoogte;
	void (*functie)( void );
	struct hoofdmenu_struct *pxNext;
}menu_struct;

extern menu_struct *pxBestelUIHeadMenu[5];
extern menu_struct *pxBestelUICurrMenu[5];
extern int16_t sBestelUIHoofdMenuIndex;	//Id van het eerste hoofdmenuitem dat men moet laten zien
extern int16_t sBestelUIOnderMenuIndex; //Id van het eerste ondermenuitemen dat men moet laten zien
extern double xBestelUITotaalPrijs; //Totaale prijs van de bestelling
extern char cBestelUINaam[9];
extern char cBestelUIZichtbaarItem[20];
extern int16_t sBestelUIZichtbaarID;
extern int16_t sBestelUIZichtbaarOnderID;
extern int16_t sBestelUIAantalDetail;
extern double xBestelUIPrijsDetail;
extern struct tcp_pcb * xBestelUILocalConnectie;

/*
 * 0 -> hoofdmenu
 * 1 -> ondermenu
 * 2 -> sidemenu
 * 3 -> bestelopties
 * 4 -> winkelmandje
 */

/*---------------------------------------------------------------------------------------------*/

void vBestelUIPrintStartscherm( int16_t xduur );
void vBestelUIPrintWachtenVerbinding( void );
void vBestelUIPrintVerbonden( int16_t xduur );
void vBestelUIInitSideMenu( void );
void vBestelUISetDevicename( char name[8] );
void vBestelUIPrintHoofdmenu( void );
void vBestelUIPrintOndermenu( void );
void vBestelUIMenuDOWN( void );
void vBestelUIMenuUP( void );
void vBestelUIMenuTERUG( void );
void vBestelUIClearMenuSection( void );
void vBestelUIClearOndermenu( void );
void vBestelUIPrintTotaalPrijs( void );
void vBestelUISetDetailPrice( char *prijs );
void vBestelUIEnableBestelopties( void );
void vBestelUISetAantal( void );
void vBestelUIAantalMin( void );
void vBestelUIAantalPlus( void );
void vBestelUIBestellingToevoegen( void );
void vBestelUIBestel( void );
menu_struct* xBestelUIAddToList( char cData[], int16_t sId, int16_t sLijstID, int16_t sStartX, int16_t sStartY, int16_t sBreedte, int16_t sHoogte, void ( *functie )(void), int16_t sSubID );
menu_struct* xBestelUICreateList( char cData[], int16_t sId, int16_t sLijstID, int16_t sStartX, int16_t sStartY, int16_t sBreedte, int16_t sHoogte, void ( *functie )(void), int16_t sSubID );


#endif /* BESTELUI_H_ */
