/*
 * bestelUI.c
 *
 *  Created on: 4-dec.-2016
 *  Author: Robbe Goovaerts
 */

#include "bestelUI.h"

menu_struct *pxBestelUIHeadMenu[5];
menu_struct *pxBestelUICurrMenu[5];
int16_t sBestelUIHoofdMenuIndex = 0;
int16_t sBestelUIOnderMenuIndex = 0;
double xBestelUITotaalPrijs = 0;
char cBestelUINaam[9];
char cBestelUIZichtbaarItem[20];
int16_t sBestelUIZichtbaarID = 0;
int16_t sBestelUIZichtbaarOnderID = 0;
int16_t sBestelUIAantalDetail = 0;
double xBestelUIPrijsDetail = 0;
struct tcp_pcb * xBestelUILocalConnectie;

/*---------------------------------------------------------------------------------------------*/

/*
 * @brief 	Deze functie creeërt 4 gelinkte lijsten.
 * @parm	cData: 		String die in het dataveld van de gelinke lijst moeten komen
 * @parm	sId:		Id van het lijst element
 * @parm	sLijstID:	Id van de lijst waarin het element moet komen
 * @parm	sStartX:	X plaats waar de knop moet starten
 * @parm	sStarty:	Y plaats waar de knop moet starten
 * @parm	sBreedte:	Breedte van de knop
 * @parm	sHoogte:	Hoogte van de knop
 * @parm	functie:	Functie die uitgevoerd moet worden bij het drukken op de knop
 * @parm	sSubID:		Id van het element in de originele lijst
 */
menu_struct* xBestelUICreateList( char cData[], int16_t sId, int16_t sLijstID, int16_t sStartX, int16_t sStartY, int16_t sBreedte, int16_t sHoogte, void ( *functie )( void ), int16_t sSubID )
{

menu_struct *pxPtr = ( menu_struct* )malloc( sizeof( menu_struct ) );

	if( pxPtr == NULL )
	{
		BSP_LCD_Clear( LCD_COLOR_RED );
		BSP_LCD_SetBackColor( LCD_COLOR_RED );
		BSP_LCD_SetFont( &Font24 );
		BSP_LCD_DisplayStringAt( 0, 5, (uint8_t*)"Niet genoeg geheugen!", CENTER_MODE );
		while( 1 )
		{}
	}

	strcpy( pxPtr->cData, cData );
	pxPtr->sId = sId;
	pxPtr->sBreedte = sBreedte;
	pxPtr->sHoogte = sHoogte;
	pxPtr->sStartX = sStartX;
	pxPtr->sStartY = sStartY;
	pxPtr->pxNext = NULL;
	pxPtr->functie = functie;
	pxPtr->sSubID = sSubID;
	pxBestelUIHeadMenu[sLijstID] = pxBestelUICurrMenu[sLijstID] = pxPtr;
	return pxPtr;
}

/*---------------------------------------------------------------------------------------------*/

/*
 * @brief 	Deze functie voegt elementen toe aan 4 gelinkte lijsten.
 * 			Als de lijst nog niet bestaat zal de createList functie opgeroepen worden.
 * @parm	cData: 		String die in het dataveld van de gelinke lijst moeten komen
 * @parm	sId:		Id van het lijst element
 * @parm	sLijstID:	Id van de lijst waarin het element moet komen
 * @parm	sStartX:	X plaats waar de knop moet starten
 * @parm	sStarty:	Y plaats waar de knop moet starten
 * @parm	sBreedte:	Breedte van de knop
 * @parm	sHoogte:	Hoogte van de knop
 * @parm	functie:	Functie die uitgevoerd moet worden bij het drukken op de knop
 * @parm	sSubID:		Id van het element in de originele lijst
 */
menu_struct* xBestelUIAddToList( char cData[], int16_t sId, int16_t sLijstID, int16_t sStartX, int16_t sStartY, int16_t sBreedte, int16_t sHoogte, void ( *functie )( void ), int16_t sSubID )
{

menu_struct *pxPtr = ( menu_struct* )malloc( sizeof( menu_struct ) );

	if( pxBestelUIHeadMenu[sLijstID] == NULL )
	{
		return( xBestelUICreateList( cData, sId, sLijstID, sStartX, sStartY, sBreedte, sHoogte, functie, sSubID ) );
	}

	if( pxPtr == NULL )
	{
		BSP_LCD_Clear( LCD_COLOR_RED );
		BSP_LCD_SetBackColor( LCD_COLOR_RED );
		BSP_LCD_SetFont( &Font24 );
		BSP_LCD_DisplayStringAt( 0, 5, ( uint8_t* )"Niet genoeg geheugen!", CENTER_MODE );
		while(1)
		{}
	}

	strcpy( pxPtr->cData, cData );
	pxPtr->sId = sId;
	pxPtr->sBreedte = sBreedte;
	pxPtr->sHoogte = sHoogte;
	pxPtr->sStartX = sStartX;
	pxPtr->sStartY = sStartY;
	pxPtr->functie = functie;
	pxPtr->sSubID = sSubID;
	pxPtr->pxNext = NULL;

	pxBestelUICurrMenu[sLijstID]->pxNext = pxPtr;
	pxBestelUICurrMenu[sLijstID] = pxPtr;

	return pxPtr;
}

/*---------------------------------------------------------------------------------------------*/

/*
 * @brief 	Deze functie slaat de naam van het device op.
 * 			Deze naam zal zichtbaar gemaakt worden in de rechter bovenhoek van het scherm
 * 			alsook worden doorgestuurd bij elke bestelling.
 * @parm	xName: 		Naam van het toestel
 */
void vBestelUISetDevicename( char cName[8] )
{

int16_t sTeller = 0;

	for( sTeller = 0 ; sTeller < 8 ; sTeller++ )
	{
		cBestelUINaam[sTeller] = cName[sTeller];
	}
	cBestelUINaam[sTeller] = '\0';
}

/*---------------------------------------------------------------------------------------------*/

/*
 * @brief 	Deze functie print het startscherm af.
 * @parm	sDuur: 		Aantal ms dat het startscherm moet blijven staan
 */
void vBestelUIPrintStartscherm( int16_t sDuur )
{
	BSP_LCD_Clear( 0xff5495ff );
	BSP_LCD_SetTextColor( 0xffffffff );
	BSP_LCD_SetBackColor( 0xff5495ff );
	BSP_LCD_SetFont( &Font24 );
	BSP_LCD_DisplayStringAt( 0, 15, ( uint8_t* )"Bestelsysteem V1.0", CENTER_MODE );
	BSP_LCD_SetFont( &Font20 );
	BSP_LCD_DisplayStringAt( 0, 60, ( uint8_t* )"Embedded systems", CENTER_MODE );
	BSP_LCD_DisplayStringAt( 0, 100, ( uint8_t* )"Robbe Goovaerts", CENTER_MODE );
	HAL_Delay( sDuur );
}

/*---------------------------------------------------------------------------------------------*/

/*
 * @brief 	Het wachtscherm weergeven.
 */
void vBestelUIPrintWachtenVerbinding( void )
{
	BSP_LCD_Clear( 0xff5495ff );
	BSP_LCD_SetTextColor( 0xffffffff );
	BSP_LCD_SetBackColor( 0xff5495ff );
	BSP_LCD_SetFont( &Font24 );
	BSP_LCD_DisplayStringAt( 0, 15, ( uint8_t* )"Wachten op verbinding...", CENTER_MODE );
}

/*---------------------------------------------------------------------------------------------*/

/*
 * @brief 	Het verbondenscherm weergeven.
 */
void vBestelUIPrintVerbonden( int16_t xduur)
{
	BSP_LCD_Clear( 0xff00ff00 );
	BSP_LCD_SetTextColor( 0xffffffff );
	BSP_LCD_SetBackColor( 0xff00ff00 );
	BSP_LCD_SetFont( &Font24 );
	BSP_LCD_DisplayStringAt( 0, 15, ( uint8_t* )"Verbonden!", CENTER_MODE );
	HAL_Delay( xduur );
}

/*---------------------------------------------------------------------------------------------*/

/*
 * @brief 	Het menu langs de rechter kant initailiseren en weergeven.
 * 			Dit menu zal blijven staan.
 */
void vBestelUIInitSideMenu( void )
{

menu_struct *pxLocalCurr;

	BSP_LCD_Clear( 0xff5495ff );
	BSP_LCD_SetTextColor( LCD_COLOR_WHITE );
	BSP_LCD_SetBackColor( 0xff5495ff );
	BSP_LCD_SetFont( &Font16 );
	BSP_LCD_DrawLine( 320, 0, 320, 272 );
	BSP_LCD_DisplayStringAt( 0 , 5, ( uint8_t* )cBestelUINaam, RIGHT_MODE );

	//Knoppen instellen
	xBestelUIAddToList( "UP", 0, 2, 325, 32, 45, 35, &vBestelUIMenuUP, 0 );
	xBestelUIAddToList( "DOWN", 1, 2, 325, 72, 45, 35, &vBestelUIMenuDOWN, 0 );
	xBestelUIAddToList( "TERUG", 2, 2, 390, 32, 60, 35, &vBestelUIMenuTERUG, 0 );
	xBestelUIAddToList( "BESTEL", 3, 2, 325, 180, 100, 35, &vBestelUIBestel, 0 );

	//Knoppen tekenen
	pxLocalCurr = pxBestelUIHeadMenu[2];

	//Menu afdrukken
	while( pxLocalCurr != NULL )
	{
		BSP_LCD_SetBackColor( 0xff54ccff );
		BSP_LCD_SetTextColor( 0xff54ccff );
		BSP_LCD_FillRect( pxLocalCurr->sStartX+1, pxLocalCurr->sStartY+1, pxLocalCurr->sBreedte-1, pxLocalCurr->sHoogte-1 );
		BSP_LCD_SetTextColor( LCD_COLOR_WHITE );
		BSP_LCD_DrawRect( pxLocalCurr->sStartX, pxLocalCurr->sStartY, pxLocalCurr->sBreedte, pxLocalCurr->sHoogte );
		BSP_LCD_DisplayStringAt( pxLocalCurr->sStartX+1, pxLocalCurr->sStartY+8, ( uint8_t* )pxLocalCurr->cData, LEFT_MODE );
		pxLocalCurr = pxLocalCurr->pxNext;
	}

	vBestelUIPrintTotaalPrijs();
}

/*---------------------------------------------------------------------------------------------*/

/*
 * @brief 	Deze functie roept een functie op die de sectie waar het hoofdmenu moet komen leeg maakt.
 * 			Ook wordt het hoofdmenu afgedruk rekening houdend met het scrollen.
 */
void vBestelUIPrintHoofdmenu( void )
{

menu_struct *pxLocalCurr = pxBestelUIHeadMenu[0];

	vBestelUIClearMenuSection();
	int16_t sPrintCounter = 0;
	BSP_LCD_SetFont( &Font16 );
	BSP_LCD_SetBackColor( 0xff5495ff );
	BSP_LCD_SetTextColor( LCD_COLOR_WHITE );
	strcpy( cBestelUIZichtbaarItem, "Hoofdmenu" );

	//Tittel adrukken
	BSP_LCD_DisplayStringAt( 20, 12, ( uint8_t* )cBestelUIZichtbaarItem, LEFT_MODE );

	//Items adrukken
	BSP_LCD_SetBackColor( 0xff54ccff );
	while( ( pxLocalCurr!=NULL ) & ( pxLocalCurr->sId < sBestelUIHoofdMenuIndex ))
	{
		pxLocalCurr = pxLocalCurr->pxNext;
	}
	while( ( pxLocalCurr!=NULL ) & ( sPrintCounter<6 ) )
	{
		BSP_LCD_SetTextColor( 0xff54ccff );
		BSP_LCD_FillRect( 5+1, 32 + 40 * sPrintCounter+1, 310-1, 35-1 );
		BSP_LCD_SetTextColor( LCD_COLOR_WHITE );
		BSP_LCD_DrawRect( 5, 32 + 40 * sPrintCounter, 310, 35 );
		BSP_LCD_DisplayStringAt( 20, 42 + 40*sPrintCounter, (uint8_t*)pxLocalCurr->cData, LEFT_MODE );

		//Locatie van knop wegschrijven naar struct voor TS functies
		pxLocalCurr->sStartX = 5;
		pxLocalCurr->sStartY = 32 + 40 * sPrintCounter;
		pxLocalCurr->sBreedte = 310;
		pxLocalCurr->sHoogte = 35;

		pxLocalCurr = pxLocalCurr->pxNext;
		sPrintCounter++;
	}
}

/*---------------------------------------------------------------------------------------------*/

/*
 * @brief 	Deze functie roept een functie op die de sectie waar het ondermenu moet komen leeg maakt.
 * 			Ook wordt het ondermenu afgedruk rekening houdend met het scrollen.
 */
void vBestelUIPrintOndermenu( void )
{

menu_struct *pxLocalCurr = pxBestelUIHeadMenu[1];
int16_t sPrintCounter = 0;

	vBestelUIClearMenuSection();
	BSP_LCD_SetFont( &Font16 );
	BSP_LCD_SetBackColor( 0xff5495ff );
	BSP_LCD_SetTextColor( LCD_COLOR_WHITE );
	strcpy( cBestelUIZichtbaarItem, "Ondermenu" );

	//Tittel adrukken
	BSP_LCD_DisplayStringAt( 20, 12, (uint8_t*)cBestelUIZichtbaarItem, LEFT_MODE );


	//Items adrukken
	BSP_LCD_SetBackColor( 0xff54ccff );
	while( ( pxLocalCurr!=NULL ) & ( pxLocalCurr->sId < sBestelUIOnderMenuIndex ) )
	{
		pxLocalCurr = pxLocalCurr->pxNext;
	}
	while( ( pxLocalCurr != NULL ) & ( sPrintCounter < 6 ) )
	{
		BSP_LCD_SetTextColor( 0xff54ccff );
		BSP_LCD_FillRect( 5+1, 32 + 40 * sPrintCounter+1, 310-1, 35-1 );
		BSP_LCD_SetTextColor( LCD_COLOR_WHITE );
		BSP_LCD_DrawRect( 5, 32 + 40*sPrintCounter, 310, 35 );
		BSP_LCD_DisplayStringAt( 20, 42 + 40*sPrintCounter, ( uint8_t* )pxLocalCurr->cData, LEFT_MODE );

		//Locatie van knop wegschrijven naar struct voor TS functies
		pxLocalCurr->sStartX = 5;
		pxLocalCurr->sStartY = 32 + 40 * sPrintCounter;
		pxLocalCurr->sBreedte = 310;
		pxLocalCurr->sHoogte = 35;

		pxLocalCurr = pxLocalCurr->pxNext;
		sPrintCounter++;
	}
}

/*---------------------------------------------------------------------------------------------*/

/*
 * @brief 	Deze functie scrolt het ondermenu of het hoofdmenu afhankelijk van wat op
 * 			dat moment zichtbaar is naar beneden.
 */
void vBestelUIMenuDOWN( void )
{
	if( strcmp( "Hoofdmenu", cBestelUIZichtbaarItem ) )
	{
		sBestelUIOnderMenuIndex++;
		vBestelUIPrintOndermenu();
	}
	else
	{
		sBestelUIHoofdMenuIndex++;
		vBestelUIPrintHoofdmenu();
	}
}

/*---------------------------------------------------------------------------------------------*/

/*
 * @brief 	Deze functie scrolt het ondermenu of het hoofdmenu afhankelijk van wat op
 * 			dat moment zichtbaar is naar boven.
 */
void vBestelUIMenuUP( void )
{
	if( strcmp( "Hoofdmenu", cBestelUIZichtbaarItem ) )
	{
		sBestelUIOnderMenuIndex--;
		if( sBestelUIOnderMenuIndex < 0 )
		{
			sBestelUIOnderMenuIndex = 0;
		}
		vBestelUIPrintOndermenu();
	}
	else
	{
		sBestelUIHoofdMenuIndex--;
		if( sBestelUIHoofdMenuIndex < 0 )
		{
			sBestelUIHoofdMenuIndex = 0;
		}
		vBestelUIPrintHoofdmenu();
	}
}

/*---------------------------------------------------------------------------------------------*/

/*
 * @brief 	Deze functie zorgt ervoor dat je terug kan gaan naar het hoofdmenu.
 */
void vBestelUIMenuTERUG( void )
{
	if( strcmp( "Hoofdmenu", cBestelUIZichtbaarItem ) )
	{
		vBestelUIClearOndermenu();
		vBestelUIPrintHoofdmenu();
	}
}

/*---------------------------------------------------------------------------------------------*/

/*
 * @brief 	Deze functie zorgt ervoor dat het menu en ondermenu niet meer zichtbaar zijn.
 * 			Ook de knoppen worden uitgeschakeld.
 */
void vBestelUIClearMenuSection( void )
{

menu_struct *pxLocalCurr = pxBestelUIHeadMenu[0];

	BSP_LCD_SetTextColor( 0xff5495ff );
	BSP_LCD_FillRect( 0, 0, 319, 272 );

	while( pxLocalCurr != NULL )
	{
		pxLocalCurr->sBreedte = 0;
		pxLocalCurr->sHoogte = 0;
		pxLocalCurr->sStartX = 0;
		pxLocalCurr->sStartY = 0;
		pxLocalCurr = pxLocalCurr->pxNext;
	}

	pxLocalCurr = pxBestelUIHeadMenu[1];
	while( pxLocalCurr != NULL )
	{
		pxLocalCurr->sBreedte = 0;
		pxLocalCurr->sHoogte = 0;
		pxLocalCurr->sStartX = 0;
		pxLocalCurr->sStartY = 0;
		pxLocalCurr = pxLocalCurr->pxNext;
	}
}

/*---------------------------------------------------------------------------------------------*/

/*
 * @brief 	Deze functie maakt het ondermenu leeg. En geeft de geheugenplaatsen terug vrij.
 */
void vBestelUIClearOndermenu( void )
{

menu_struct *pxLocalCurr = pxBestelUICurrMenu[1];
menu_struct *pxNextBuffer;

	while( pxLocalCurr != NULL )
	{
		pxNextBuffer = pxLocalCurr->pxNext;
		free( pxLocalCurr );
		pxLocalCurr = pxNextBuffer;
	}
	pxBestelUICurrMenu[1] = NULL;
	pxBestelUIHeadMenu[1] = NULL;
}

/*---------------------------------------------------------------------------------------------*/

/*
 * @brief 	Deze functie print de totaalprijs.
 */
void vBestelUIPrintTotaalPrijs( void )
{

char cPrijsBuffer[32];

	BSP_LCD_SetBackColor( 0xff5495ff );
	BSP_LCD_SetTextColor( LCD_COLOR_WHITE );

	sprintf( cPrijsBuffer, "%4d,%02d EURO", ( int )xBestelUITotaalPrijs, ( ( int )( xBestelUITotaalPrijs * 100 ) ) % 100 );

	BSP_LCD_DisplayStringAt( 330, 140, ( uint8_t* )"TOTAAL:", LEFT_MODE );
	BSP_LCD_DisplayStringAt( 330, 157, ( uint8_t* )cPrijsBuffer, LEFT_MODE );
}

/*---------------------------------------------------------------------------------------------*/

/*
 * @brief 	Deze functie bepaalt de prijs van één product. Ook zal deze worden afgedrukt.
 * @parm	pxPrijs:	String waar de prijs in zit.
 */
void vBestelUISetDetailPrice( char* pcPrijs )
{

char cPrijsBuffer[64];
char cPrijsIntBuffer[64];
int16_t sTellerA=0, sTellerB=0, sPrijsBuffer = 0;

	while( pcPrijs[sTellerA] != '.' )
	{
		cPrijsIntBuffer[sTellerA] = pcPrijs[sTellerA];
		sTellerA++;
	}
	cPrijsIntBuffer[sTellerA] = '\0';

	xBestelUIPrijsDetail = atoi( cPrijsIntBuffer );

	sTellerA++;
	while( pcPrijs[sTellerA] != '\0' )
	{
		cPrijsIntBuffer[sTellerB] = pcPrijs[sTellerA];
		sTellerB++;
		sTellerA++;
	}
	cPrijsIntBuffer[sTellerB] = '\0';
	sPrijsBuffer = atoi( cPrijsIntBuffer );
	xBestelUIPrijsDetail += sPrijsBuffer/100.0;

 	BSP_LCD_SetBackColor( 0xff5495ff );
	BSP_LCD_SetTextColor( LCD_COLOR_WHITE );

	strcpy( cPrijsBuffer, pcPrijs );
	strcat( cPrijsBuffer, " EURO" );

	BSP_LCD_DisplayStringAt( 20, 140, ( uint8_t* )cBestelUIZichtbaarItem, LEFT_MODE );
	BSP_LCD_DisplayStringAt( 20, 160, ( uint8_t* )cPrijsBuffer, LEFT_MODE );
}

/*---------------------------------------------------------------------------------------------*/

/*
 * @brief 	Deze functie bekijk wat er allemaal al op het scherm zichtbaar is. Wat niet zichtbaar is
 * 			zal hier zichtbaar gemaakt worden.
 */
void vBestelUIEnableBestelopties( void )
{

menu_struct *pxLocalCurr = pxBestelUIHeadMenu[3];
int16_t sFoundPlus = 0, sFoundMin = 0, sFoundBestellen = 0;

	while( pxLocalCurr != NULL )
	{
		if( !strcmp( pxLocalCurr->cData, "+" ) )
		{
			sFoundPlus = 1;
		}
		if( !strcmp( pxLocalCurr->cData, "-" ) )
		{
			sFoundMin = 1;
		}
		if( !strcmp( pxLocalCurr->cData, "BESTELLEN") )
		{
			sFoundBestellen = 1;
		}
		pxLocalCurr = pxLocalCurr->pxNext;
	}

	if( sFoundPlus == 0 )
	{
		xBestelUIAddToList( "+", 99, 3, 130, 180, 30, 30, &vBestelUIAantalPlus, 0 );
	}
	if(sFoundMin == 0)
	{
		xBestelUIAddToList( "-", 99, 3, 170, 180, 30, 30, &vBestelUIAantalMin, 0 );
	}
	if( sFoundBestellen == 0 )
	{
		xBestelUIAddToList( "BESTELLEN", 99, 3, 130, 220, 120, 30, &vBestelUIBestellingToevoegen, 0 );
	}


	sBestelUIAantalDetail = 0;

	pxLocalCurr = pxBestelUIHeadMenu[3];

	BSP_LCD_SetBackColor( 0xff54ccff );
	while( ( pxLocalCurr != NULL ) )
	{
		BSP_LCD_SetTextColor( 0xff54ccff );
		BSP_LCD_FillRect( pxLocalCurr->sStartX+1, pxLocalCurr->sStartY+1, pxLocalCurr->sBreedte-1, pxLocalCurr->sHoogte-1 );
		BSP_LCD_SetTextColor( LCD_COLOR_WHITE );
		BSP_LCD_DrawRect( pxLocalCurr->sStartX, pxLocalCurr->sStartY, pxLocalCurr->sBreedte, pxLocalCurr->sHoogte );
		BSP_LCD_DisplayStringAt( pxLocalCurr->sStartX + 6, pxLocalCurr->sStartY+8, (uint8_t*)pxLocalCurr->cData, LEFT_MODE );

		pxLocalCurr = pxLocalCurr->pxNext;
	}
}

/*---------------------------------------------------------------------------------------------*/

/*
 * @brief 	Deze functie print het aantal te bestellen items naar het scherm.
 */
void vBestelUISetAantal( void )
{

char cAantal[16];

	BSP_LCD_SetBackColor( 0xff5495ff );
	BSP_LCD_SetTextColor( 0xff5495ff );
	sprintf( cAantal, "%d", sBestelUIAantalDetail );

	BSP_LCD_FillRect( 19, 185, 40, 15 );
	BSP_LCD_SetTextColor( LCD_COLOR_WHITE );
	BSP_LCD_DisplayStringAt( 20, 186, (uint8_t*)cAantal, LEFT_MODE );
}

/*---------------------------------------------------------------------------------------------*/

/*
 * @brief 	Deze functie vermindert het aantal detailitems met 1 en zorgt ervoor dat je niet minder
 * 			dan 0 items kan bestellen.
 */
void vBestelUIAantalMin( void )
{
	sBestelUIAantalDetail--;
	if( sBestelUIAantalDetail < 0 )
	{
		sBestelUIAantalDetail = 0;
	}
	vBestelUISetAantal();
}

/*---------------------------------------------------------------------------------------------*/

/*
 * @brief 	Deze functie vermeerdert het aantal met 1.
 */
void vBestelUIAantalPlus( void )
{
	sBestelUIAantalDetail++;
	vBestelUISetAantal();
}

/*---------------------------------------------------------------------------------------------*/

/*
 * @brief 	Deze functie voegt het bestelde item toe aan het winkelmandje.
 * 			Er word rekening gehouden of dat item reeds in het winkelmandje zit.
 */
void vBestelUIBestellingToevoegen( void )
{

menu_struct *pxLocalCurr = pxBestelUIHeadMenu[4];
int16_t sIfound = 0;

	//TotaalprijsBerekenen
	xBestelUITotaalPrijs += xBestelUIPrijsDetail * sBestelUIAantalDetail;

	//Item toevoegen aan winkelmandje
	while(pxLocalCurr!=NULL)
	{
		if((pxLocalCurr->sId==sBestelUIZichtbaarID)&(pxLocalCurr->sSubID==sBestelUIZichtbaarOnderID))
		{
			pxLocalCurr->sHoogte = pxLocalCurr->sHoogte + sBestelUIAantalDetail;
			sIfound = 1;
		}
		pxLocalCurr = pxLocalCurr->pxNext;
	}

	if(sIfound==0)
	{
		xBestelUIAddToList(cBestelUIZichtbaarItem, sBestelUIZichtbaarID, 4, 0, 0, 0, sBestelUIAantalDetail, NULL, sBestelUIZichtbaarOnderID);
	}

	vBestelUIPrintTotaalPrijs();
}

/*---------------------------------------------------------------------------------------------*/

/*
 * @brief 	Deze functie stuurt de bestelling door naar de server.
 * 			Ook worden de lokale gegevens gereset.
 */
void vBestelUIBestel( void )
{

int16_t sGetalBuffer = 0;
char cBestelBuffer[500];
char cIntBuffer[56];
char cIntBuffer2[56];
menu_struct *pxLocalCurr = pxBestelUIHeadMenu[4];
menu_struct *pxLocalCurrBuffer;

	strcpy( cBestelBuffer, "BESTEL, " );

	//Totaalprijs omzetten naar string
	sGetalBuffer = ( int )xBestelUITotaalPrijs;
	sprintf( cIntBuffer, "%d", sGetalBuffer );
	strcat( cIntBuffer, "." );
	xBestelUITotaalPrijs = xBestelUITotaalPrijs - sGetalBuffer;
	xBestelUITotaalPrijs = xBestelUITotaalPrijs * 100;
	sprintf(cIntBuffer2, "%d", ( int )xBestelUITotaalPrijs);
	strcat( cIntBuffer, cIntBuffer2 );

	//Tafelnaam toevoegen

	strcat( cBestelBuffer, cBestelUINaam );
	strcat( cBestelBuffer, ", " );

	strcat( cBestelBuffer, cIntBuffer );
	strcat( cBestelBuffer, ", " );

	while( pxLocalCurr != NULL )
	{
		sprintf( cIntBuffer, "%d", pxLocalCurr->sHoogte );
		strcat( cBestelBuffer, cIntBuffer );
		strcat( cBestelBuffer, " X " );
		sprintf( cIntBuffer, "%d", pxLocalCurr->sId );
		strcat( cBestelBuffer, cIntBuffer );
		strcat( cBestelBuffer, "_" );
		sprintf( cIntBuffer, "%d", pxLocalCurr->sSubID );
		strcat( cBestelBuffer, cIntBuffer );
		strcat( cBestelBuffer, "_" );
		strcat( cBestelBuffer, pxLocalCurr->cData );
		strcat( cBestelBuffer, ", " );
		pxLocalCurr = pxLocalCurr->pxNext;
	}
	strcat( cBestelBuffer, "END" );
	tcp_write( xBestelUILocalConnectie, cBestelBuffer, strlen(cBestelBuffer), 1 );

	//Alles resetten voor een nieuwe bestelling

	pxLocalCurr = pxBestelUIHeadMenu[4];
	while( pxLocalCurr != NULL )
	{
		pxLocalCurrBuffer = pxLocalCurr;
		free( pxLocalCurr );
		pxLocalCurr = pxLocalCurrBuffer->pxNext;
	}
	pxBestelUICurrMenu[4] = NULL;
	pxBestelUIHeadMenu[4] = NULL;
	xBestelUITotaalPrijs = 0.0;
	vBestelUIPrintTotaalPrijs();
}
