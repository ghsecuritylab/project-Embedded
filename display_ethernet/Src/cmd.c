/*
 * cmd.c
 *
 *  Created on: 30-nov.-2016
 *  Author: Robbe Goovaerts
 */

#include "cmd.h"

char *pvGeheugen[MAX_LEN_GEHEUGEN];
int xWriteLocation = 0;
int xBottomLine = 10;

/*
 * Deze functie initaliseert het display.
 */
void voidLcdInit( void )
{
	/* LCD Initialization */
	BSP_LCD_Init();
	BSP_LCD_LayerDefaultInit(0, LCD_FB_START_ADDRESS);
	BSP_LCD_LayerDefaultInit(1, LCD_FB_START_ADDRESS+480*272*4);
	BSP_LCD_SelectLayer(1);
	BSP_LCD_Clear(0x00000000);
	BSP_LCD_SelectLayer(0);
	BSP_LCD_Clear(0x00000000);
	/* Enable the LCD */
	BSP_LCD_DisplayOn();
}

/*------------------------------------------------------------------------*/

/*
 * Deze functie zet alles klaar om het display te kunnen gebruiken als cmd.
 */
void voidCmdInit( void )
{
	/* Select the LCD Background Layer  */
	BSP_LCD_SelectLayer(0);
	/* Clear the Background Layer */
	BSP_LCD_Clear(LCD_COLOR_BLACK);
	/* Some sign */
	BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
	BSP_LCD_SetBackColor(LCD_COLOR_BLACK);
	BSP_LCD_SetFont(&Font16);
}

/*------------------------------------------------------------------------*/

/*
 * Deze functie pusht één regel naar het scherm
 */

void voidCmdWrite( char *input, int len )
{
	//Geheugen alloceren
	char *buffer = (char*)calloc(len, sizeof(char));

	//Input string wegschrijven naar die geheugenplaats
	buffer = input;

	//Input wegschrijven naar geheugen array
	pvGeheugen[xWriteLocation] = buffer;
	xWriteLocation++;

	//Scherm herschrijven
	voidRefreshDisplay();
}

/*------------------------------------------------------------------------*/

/*
 * Deze functie schrijft het hele display terug naar het scherm
 */

void voidRefreshDisplay( void )
{
	int xTeller = xWriteLocation - 1;
	int xLijnTeller;

	//Het scherm leegmaken
	BSP_LCD_Clear(LCD_COLOR_BLACK);

	//Onderste schrijflijn berekenen
	if(xTeller<=xBottomLine)
	{
		xLijnTeller = xTeller;
	}
	else
	{
		xLijnTeller = xBottomLine;
	}

	//Beeld lijn per lijn wegschrijven. Van onderen te beginnen.
	while(xLijnTeller >= 0)
	{
		BSP_LCD_DisplayStringAtLine(xLijnTeller, (uint8_t*)pvGeheugen[xTeller]);
		xTeller--;
		xLijnTeller--;
	}
}

/*------------------------------------------------------------------------*/

/*
 * Deze functie stat het toetsenbord op en geeft de ingegeven string terug.
 */

char* pvcharCmdRead( UART_HandleTypeDef huart1 )
{
	HAL_UART_Transmit(&huart1, (uint8_t*)"Toetsenbord gestart\r\n", strlen("Toetsenbord gestart\r\n"), HAL_MAX_DELAY);
	int i, j, k;
	int hitChecker;
	TS_StateTypeDef xTsData;
	uint16_t buttonXLocation[40];
	uint16_t buttonYLocation[40];
	uint8_t buttonAantal = 40;
	uint8_t *buttonName[41] = {'1', '2', '3', '4', '5', '6', '7', '8', '9', '0', 'A', 'Z', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', 'Q', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', 'M', 'W', 'X', 'C', 'V', 'B', 'N', ',', ';', '.', ':'};

	//Toetsenbord printen
	BSP_LCD_SetTextColor(LCD_COLOR_WHITE);

	//Spaciebalk printen
	BSP_LCD_DrawRect(140, 242, 200, 25);

	//Overige toetsen printen
	j=0;
	for(i=0;i<buttonAantal;i++)
	{
		buttonXLocation[i] = 90 + 30 * j;
		j++;
		if(j==10)
		{
			j=0;
		}
	}
	j=0;
	k=0;
	for(i=0;i<buttonAantal;i++)
	{
		buttonYLocation[i] = 122 + 30 * j;
		k++;
		if(k==10)
		{
			k=0;
			j++;
		}
	}


	for(i=0;i<buttonAantal;i++)
	{
		BSP_LCD_DrawRect(buttonXLocation[i], buttonYLocation[i], 25, 25);
		BSP_LCD_DisplayStringAt(buttonXLocation[i]+9, buttonYLocation[i]+9, &buttonName[i], LEFT_MODE);
	}


	while(1)
	{
		BSP_TS_GetState(&xTsData);
		if(xTsData.touchDetected>0)
		{
			HAL_UART_Transmit(&huart1, &xTsData.touchX[0], strlen(1), HAL_MAX_DELAY);
			HAL_Delay(400);
		}
	}
}




