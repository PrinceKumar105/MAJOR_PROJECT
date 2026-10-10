/*
 * ST7789_GFX.c
 *
 *  Created on: Oct 8, 2026
 *      Author: KIIT0001
 */


#include "ST7789_STM32_Driver.h"
#include "ST7789_GFX.h"

/* imprecise small delay */
__STATIC_INLINE void DelayUs(volatile uint32_t us)
{
	us *= (SystemCoreClock / 1000000);
	while (us--);
}

void ST7789_DrawHollowCircle(uint16_t X, uint16_t Y, uint16_t radius, uint16_t color)
{
	int x = radius-1;
	int y = 0;
	int dx = 1;
	int dy = 1;
	int err = dx - (radius << 1);

	while (x >= y)
	{
		ST7789_DrawPixel(X + x, Y + y, color);
		ST7789_DrawPixel(X + y, Y + x, color);
		ST7789_DrawPixel(X - y, Y + x, color);
		ST7789_DrawPixel(X - x, Y + y, color);
		ST7789_DrawPixel(X - x, Y - y, color);
		ST7789_DrawPixel(X - y, Y - x, color);
		ST7789_DrawPixel(X + y, Y - x, color);
		ST7789_DrawPixel(X + x, Y - y, color);

		if (err <= 0)
		{
			y++;
			err += dy;
			dy += 2;
		}

		if (err > 0)
		{
			x--;
			dx += 2;
			err += (-radius << 1) + dx;
		}
	}
}

void ST7789_DrawFilledCircle(uint16_t X, uint16_t Y, uint16_t radius, uint16_t color)
{

	int x = radius;
	int y = 0;
	int xChange = 1 - (radius << 1);
	int yChange = 0;
	int radiusError = 0;

	while (x >= y)
	{
		for (int i = X - x; i <= X + x; i++)
		{
			ST7789_DrawPixel(i, Y + y,color);
			ST7789_DrawPixel(i, Y - y,color);
		}

		for (int i = X - y; i <= X + y; i++)
		{
			ST7789_DrawPixel(i, Y + x,color);
			ST7789_DrawPixel(i, Y - x,color);
		}

		y++;
		radiusError += yChange;
		yChange += 2;

		if (((radiusError << 1) + xChange) > 0)
		{
			x--;
			radiusError += xChange;
			xChange += 2;
		}
	}
}

void ST7789_DrawHollowRectangleCoord(uint16_t X0, uint16_t Y0, uint16_t X1, uint16_t Y1, uint16_t color)
{
	uint16_t xLen = 0;
	uint16_t yLen = 0;
	uint8_t negX = 0;
	uint8_t negY = 0;
	float negCalc = 0;

	negCalc = X1 - X0;
	if(negCalc < 0) negX = 1;
	negCalc = 0;

	negCalc = Y1 - Y0;
	if(negCalc < 0) negY = 1;

	//DRAW HORIZONTAL!
	if(!negX)
	{
		xLen = X1 - X0;
	}
	else
	{
		xLen = X0 - X1;
	}
	ST7789_DrawHLine(X0, Y0, xLen, color);
	ST7789_DrawHLine(X0, Y1, xLen, color);

	//DRAW VERTICAL!
	if(!negY)
	{
		yLen = Y1 - Y0;
	}
	else
	{
		yLen = Y0 - Y1;
	}

	ST7789_DrawVLine(X0, Y0, yLen, color);
	ST7789_DrawVLine(X1, Y0, yLen, color);

	if((xLen > 0)||(yLen > 0))
	{
		ST7789_DrawPixel(X1, Y1, color);
	}
}

void ST7789_DrawFilledRectangleCoord(uint16_t X0, uint16_t Y0, uint16_t X1, uint16_t Y1, uint16_t color)
{
	uint16_t xLen = 0;
	uint16_t yLen = 0;
	uint8_t negX = 0;
	uint8_t negY = 0;
	int32_t negCalc = 0;
	uint16_t X0True = 0;
	uint16_t Y0True = 0;

	negCalc = X1 - X0;
	if(negCalc < 0) negX = 1;
	negCalc = 0;

	negCalc = Y1 - Y0;
	if(negCalc < 0) negY = 1;

	if(!negX)
	{
		xLen = X1 - X0;
		X0True = X0;
	}
	else
	{
		xLen = X0 - X1;
		X0True = X1;
	}

	if(!negY)
	{
		yLen = Y1 - Y0;
		Y0True = Y0;
	}
	else
	{
		yLen = Y0 - Y1;
		Y0True = Y1;
	}

	ST7789_DrawRectangle(X0True, Y0True, xLen, yLen, color);
}

void ST7789_DrawChar(char ch, const uint8_t font[], uint16_t X, uint16_t Y, uint16_t color, uint16_t bgcolor)
{
	if ((ch < 31) || (ch > 127)) return;

	uint8_t fOffset, fWidth, fHeight, fBPL;
	uint8_t *tempChar;

	fOffset = font[0];
	fWidth = font[1];
	fHeight = font[2];
	fBPL = font[3];

	tempChar = (uint8_t*)&font[((ch - 0x20) * fOffset) + 4]; /* Current Character = Meta + (Character Index * Offset) */

	/* Clear background first */
	ST7789_DrawRectangle(X, Y, fWidth, fHeight, bgcolor);

	for (int j=0; j < fHeight; j++)
	{
		for (int i=0; i < fWidth; i++)
		{
			uint8_t z =  tempChar[fBPL * i + ((j & 0xF8) >> 3) + 1]; /* (j & 0xF8) >> 3, increase one by 8-bits */
			uint8_t b = 1 << (j & 0x07);
			if (( z & b ) != 0x00)
			{
				ST7789_DrawPixel(X+i, Y+j, color);
			}
		}
	}
}

void ST7789_DrawText(const char* str, const uint8_t font[], uint16_t X, uint16_t Y, uint16_t color, uint16_t bgcolor)
{
	uint8_t charWidth;			/* Width of character */
	uint8_t fOffset = font[0];	/* Offset of character */
	uint8_t fWidth = font[1];	/* Width of font */

	while (*str)
	{
		ST7789_DrawChar(*str, font, X, Y, color, bgcolor);

		/* Check character width and calculate proper position */
		uint8_t *tempChar = (uint8_t*)&font[((*str - 0x20) * fOffset) + 4];
		charWidth = tempChar[0];

		if(charWidth + 2 < fWidth)
		{
			/* If character width is smaller than font width */
			X += (charWidth + 2);
		}
		else
		{
			X += fWidth;
		}

		str++;
	}
}

void ST7789_DrawImage(const uint8_t* image, uint8_t orientation)
{
	if(orientation == SCREEN_HORIZONTAL_1)
	{
		ST7789_SetRotation(SCREEN_HORIZONTAL_1);
		ST7789_SetAddress(0,0,ST7789_SCREEN_WIDTH,ST7789_SCREEN_HEIGHT);
	}
	else if(orientation == SCREEN_HORIZONTAL_2)
	{
		ST7789_SetRotation(SCREEN_HORIZONTAL_2);
		ST7789_SetAddress(0,0,ST7789_SCREEN_WIDTH,ST7789_SCREEN_HEIGHT);
	}
	else if(orientation == SCREEN_VERTICAL_2)
	{
		ST7789_SetRotation(SCREEN_VERTICAL_2);
		ST7789_SetAddress(0,0,ST7789_SCREEN_HEIGHT,ST7789_SCREEN_WIDTH);
	}
	else if(orientation == SCREEN_VERTICAL_1)
	{
		ST7789_SetRotation(SCREEN_VERTICAL_1);
		ST7789_SetAddress(0,0,ST7789_SCREEN_HEIGHT,ST7789_SCREEN_WIDTH);
	}

	uint32_t counter = 0;
	for(uint32_t i = 0; i < ST7789_SCREEN_WIDTH*ST7789_SCREEN_HEIGHT*2/BURST_MAX_SIZE; i++)
	{
		ST7789_WriteBuffer((uint8_t*)(image + counter), BURST_MAX_SIZE);
		counter += BURST_MAX_SIZE;

		/* DMA Tx is too fast, It needs some delay */
		DelayUs(1);
	}
}
