/*
 * ST7789_GFX.h
 *
 *  Created on: Oct 8, 2026
 *      Author: KIIT0001
 */


#ifndef ST7789_GFX_H
#define ST7789_GFX_H

#include "stm32f4xx_hal.h"
#include "fonts.h"

#define HORIZONTAL_IMAGE	0
#define VERTICAL_IMAGE		1

void ST7789_DrawHollowCircle(uint16_t X, uint16_t Y, uint16_t radius, uint16_t color);
void ST7789_DrawFilledCircle(uint16_t X, uint16_t Y, uint16_t radius, uint16_t color);
void ST7789_DrawHollowRectangleCoord(uint16_t X0, uint16_t Y0, uint16_t X1, uint16_t Y1, uint16_t color);
void ST7789_DrawFilledRectangleCoord(uint16_t X0, uint16_t Y0, uint16_t X1, uint16_t Y1, uint16_t color);
void ST7789_DrawChar(char ch, const uint8_t font[], uint16_t X, uint16_t Y, uint16_t color, uint16_t bgcolor);
void ST7789_DrawText(const char* str, const uint8_t font[], uint16_t X, uint16_t Y, uint16_t color, uint16_t bgcolor);
void ST7789_DrawImage(const uint8_t* image, uint8_t orientation);

#endif
