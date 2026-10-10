/*
 * ST7789_STM32_Driver.c
 *
 *  Created on: Oct 8, 2026
 *      Author: KIIT0001
 */


#include "ST7789_STM32_Driver.h"

volatile uint16_t LCD_HEIGHT = ST7789_SCREEN_HEIGHT;
volatile uint16_t LCD_WIDTH	 = ST7789_SCREEN_WIDTH;

void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi)
{
  /* Deselect when Tx Complete */
  if(hspi == HSPI_INSTANCE)
  {
	  HAL_GPIO_WritePin(LCD_CS_PORT, LCD_CS_PIN, GPIO_PIN_SET);
  }
}

static void ST7789_SPI_Tx(uint8_t data)
{
	while(!__HAL_SPI_GET_FLAG(HSPI_INSTANCE, SPI_FLAG_TXE));
	HAL_SPI_Transmit_DMA(HSPI_INSTANCE, &data, 1);
	//HAL_SPI_Transmit(HSPI_INSTANCE, &data, 1, 10);
}

static void ST7789_SPI_TxBuffer(uint8_t *buffer, uint16_t len)
{
	while(!__HAL_SPI_GET_FLAG(HSPI_INSTANCE, SPI_FLAG_TXE));
	HAL_SPI_Transmit_DMA(HSPI_INSTANCE, buffer, len);
	//HAL_SPI_Transmit(HSPI_INSTANCE, buffer, len, 10);
}

void ST7789_WriteCommand(uint8_t cmd)
{
	HAL_GPIO_WritePin(LCD_DC_PORT, LCD_DC_PIN, GPIO_PIN_RESET);	//command
	HAL_GPIO_WritePin(LCD_CS_PORT, LCD_CS_PIN, GPIO_PIN_RESET);	//select
	ST7789_SPI_Tx(cmd);
	//HAL_GPIO_WritePin(LCD_CS_PORT, LCD_CS_PIN, GPIO_PIN_SET);	//deselect
}

void ST7789_WriteData(uint8_t data)
{
	HAL_GPIO_WritePin(LCD_DC_PORT, LCD_DC_PIN, GPIO_PIN_SET);	//data
	HAL_GPIO_WritePin(LCD_CS_PORT, LCD_CS_PIN, GPIO_PIN_RESET);	//select
	ST7789_SPI_Tx(data);
	//HAL_GPIO_WritePin(LCD_CS_PORT, LCD_CS_PIN, GPIO_PIN_SET);	//deselect
}

void ST7789_WriteBuffer(uint8_t *buffer, uint16_t len)
{
	HAL_GPIO_WritePin(LCD_DC_PORT, LCD_DC_PIN, GPIO_PIN_SET);	//data
	HAL_GPIO_WritePin(LCD_CS_PORT, LCD_CS_PIN, GPIO_PIN_RESET);	//select
	ST7789_SPI_TxBuffer(buffer, len);
	//HAL_GPIO_WritePin(LCD_CS_PORT, LCD_CS_PIN, GPIO_PIN_SET);	//deselect
}

void ST7789_SetAddress(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
	uint8_t buffer[4];
	buffer[0] = x1 >> 8;
	buffer[1] = x1;
	buffer[2] = x2 >> 8;
	buffer[3] = x2;

	ST7789_WriteCommand(0x2A);
	ST7789_WriteBuffer(buffer, sizeof(buffer));

	buffer[0] = y1 >> 8;
	buffer[1] = y1;
	buffer[2] = y2 >> 8;
	buffer[3] = y2;

	ST7789_WriteCommand(0x2B);
	ST7789_WriteBuffer(buffer, sizeof(buffer));

	ST7789_WriteCommand(0x2C);
}

void ST7789_Reset(void)
{
	HAL_GPIO_WritePin(LCD_RST_PORT, LCD_RST_PIN, GPIO_PIN_RESET);	//Disable
	HAL_Delay(10);
	HAL_GPIO_WritePin(LCD_CS_PORT, LCD_CS_PIN, GPIO_PIN_RESET);		//Select
	HAL_Delay(10);
	HAL_GPIO_WritePin(LCD_RST_PORT, LCD_RST_PIN, GPIO_PIN_SET);		//Enable
	HAL_GPIO_WritePin(LCD_CS_PORT, LCD_CS_PIN, GPIO_PIN_SET); 		//Deselect
}

void ST7789_Enable(void)
{
	HAL_GPIO_WritePin(LCD_RST_PORT, LCD_RST_PIN, GPIO_PIN_SET);		//Enable
}

void ST7789_Init(void)
{
	ST7789_Enable();
	ST7789_Reset();

	// SOFTWARE RESET
	ST7789_WriteCommand(0x01);
	HAL_Delay(150);

	// EXIT SLEEP
	ST7789_WriteCommand(0x11);
	HAL_Delay(120);

	// COLOR MODE: 16-bit RGB565
	ST7789_WriteCommand(0x3A);
	ST7789_WriteData(0x55);

	// MEMORY DATA ACCESS CONTROL (Default)
	ST7789_WriteCommand(0x36);
	ST7789_WriteData(0x00);

	// PORCH SETTING
	ST7789_WriteCommand(0xB2);
	ST7789_WriteData(0x0C);
	ST7789_WriteData(0x0C);
	ST7789_WriteData(0x00);
	ST7789_WriteData(0x33);
	ST7789_WriteData(0x33);

	// GATE CONTROL
	ST7789_WriteCommand(0xB7);
	ST7789_WriteData(0x35);

	// VCOM SETTING
	ST7789_WriteCommand(0xBB);
	ST7789_WriteData(0x19);

	// LCM CONTROL
	ST7789_WriteCommand(0xC0);
	ST7789_WriteData(0x2C);

	// VDV and VRH COMMAND ENABLE
	ST7789_WriteCommand(0xC2);
	ST7789_WriteData(0x01);

	// VRH SET
	ST7789_WriteCommand(0xC3);
	ST7789_WriteData(0x12);

	// VDV SET
	ST7789_WriteCommand(0xC4);
	ST7789_WriteData(0x20);

	// FRAME RATE CONTROL
	ST7789_WriteCommand(0xC6);
	ST7789_WriteData(0x0F);

	// POWER CONTROL 1
	ST7789_WriteCommand(0xD0);
	ST7789_WriteData(0xA4);
	ST7789_WriteData(0xA1);

	// *** CRITICAL FOR ST7789 IPS PANELS: COLOR INVERSION ON ***
	ST7789_WriteCommand(0x20);

	// TURN ON DISPLAY
	ST7789_WriteCommand(0x29);
	HAL_Delay(120);

	// STARTING ROTATION
	ST7789_SetRotation(SCREEN_VERTICAL_1);
}
void ST7789_SetRotation(uint8_t rotation)
{
	ST7789_WriteCommand(0x36); // MADCTL Command
	HAL_Delay(1);

	switch(rotation)
	{
	case SCREEN_VERTICAL_1:
		ST7789_WriteData(0x00);
		LCD_WIDTH = 240;
		LCD_HEIGHT = 320;
		break;
	case SCREEN_HORIZONTAL_1:
		ST7789_WriteData(0x60);
		LCD_WIDTH  = 320;
		LCD_HEIGHT = 240;
		break;
	case SCREEN_VERTICAL_2:
		ST7789_WriteData(0xC0);
		LCD_WIDTH  = 240;
		LCD_HEIGHT = 320;
		break;
	case SCREEN_HORIZONTAL_2:
		ST7789_WriteData(0xA0);
		LCD_WIDTH  = 320;
		LCD_HEIGHT = 240;
		break;
	default:
		break;
	}
}

void ST7789_DrawColor(uint16_t color)
{
	uint8_t buffer[2] = {color>>8, color};
	ST7789_WriteBuffer(buffer, sizeof(buffer));
}

void ST7789_DrawColorBurst(uint16_t color, uint32_t size)
{
	uint32_t BufferSize = 0;

	if((size*2) < BURST_MAX_SIZE)
	{
		BufferSize = size;
	}
	else
	{
		BufferSize = BURST_MAX_SIZE;
	}

	HAL_GPIO_WritePin(LCD_DC_PORT, LCD_DC_PIN, GPIO_PIN_SET);
	HAL_GPIO_WritePin(LCD_CS_PORT, LCD_CS_PIN, GPIO_PIN_RESET);

	uint8_t chifted = color>>8;
	uint8_t BurstBuffer[BufferSize];

	for(uint32_t j = 0; j < BufferSize; j+=2)
	{
		BurstBuffer[j] = chifted;
		BurstBuffer[j+1] = color;
	}

	uint32_t SendingSize = size * 2;
	uint32_t SendingInBlock = SendingSize / BufferSize;
	uint32_t RemainderFromBlock = SendingSize % BufferSize;

	if(SendingInBlock != 0)
	{
		for(uint32_t j = 0; j < (SendingInBlock); j++)
		{
			HAL_SPI_Transmit(HSPI_INSTANCE, BurstBuffer, BufferSize, 10);
		}
	}

	HAL_SPI_Transmit(HSPI_INSTANCE, BurstBuffer, RemainderFromBlock, 10);
	HAL_GPIO_WritePin(LCD_CS_PORT, LCD_CS_PIN, GPIO_PIN_SET);
}

void ST7789_FillScreen(uint16_t color)
{
	ST7789_SetAddress(0, 0, LCD_WIDTH, LCD_HEIGHT);
	ST7789_DrawColorBurst(color, LCD_WIDTH*LCD_HEIGHT);
}

void ST7789_DrawPixel(uint16_t x,uint16_t y,uint16_t color)
{
	if((x >=LCD_WIDTH) || (y >=LCD_HEIGHT)) return;

	uint8_t bufferX[4] = {x>>8, x, (x+1)>>8, (x+1)};
	uint8_t bufferY[4] = {y>>8, y, (y+1)>>8, (y+1)};
	uint8_t bufferC[2] = {color>>8, color};

	ST7789_WriteCommand(0x2A);						//ADDRESS
	ST7789_WriteBuffer(bufferX, sizeof(bufferX));	//XDATA

	ST7789_WriteCommand(0x2B);						//ADDRESS
	ST7789_WriteBuffer(bufferY, sizeof(bufferY));	//YDATA

	ST7789_WriteCommand(0x2C);						//ADDRESS
	ST7789_WriteBuffer(bufferC, sizeof(bufferC));	//COLOR
}

void ST7789_DrawRectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t color)
{
	if((x >=LCD_WIDTH) || (y >=LCD_HEIGHT)) return;

	if((x+width-1)>=LCD_WIDTH)
	{
		width=LCD_WIDTH-x;
	}

	if((y+height-1)>=LCD_HEIGHT)
	{
		height=LCD_HEIGHT-y;
	}

	ST7789_SetAddress(x, y, x+width-1, y+height-1);
	ST7789_DrawColorBurst(color, height*width);
}

void ST7789_DrawHLine(uint16_t x, uint16_t y, uint16_t width, uint16_t color)
{
	if((x >=LCD_WIDTH) || (y >=LCD_HEIGHT)) return;

	if((x+width-1)>=LCD_WIDTH)
	{
		width=LCD_WIDTH-x;
	}

	ST7789_SetAddress(x, y, x+width-1, y);
	ST7789_DrawColorBurst(color, width);
}

void ST7789_DrawVLine(uint16_t x, uint16_t y, uint16_t height, uint16_t color)
{
	if((x >=LCD_WIDTH) || (y >=LCD_HEIGHT)) return;

	if((y+height-1)>=LCD_HEIGHT)
	{
		height=LCD_HEIGHT-y;
	}

	ST7789_SetAddress(x, y, x, y+height-1);
	ST7789_DrawColorBurst(color, height);
}
