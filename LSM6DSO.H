/*
 * lsm6ds0.h
 *
 *  Created on: Oct 7, 2026
 *      Author: KIIT0001
 */
#ifndef LSM6DSO_H
#define LSM6DSO_H

#include "stm32f4xx_hal.h"

// I2C Address (when SDO/SA0 is connected to GND)
#define LSM6DSO_I2C_ADDR 0xD4

// Register Addresses
#define LSM6DSO_WHO_AM_I 0x0F
#define LSM6DSO_CTRL1_XL 0x10
#define LSM6DSO_CTRL2_G  0x11
#define LSM6DSO_OUTX_L_G 0x22
#define LSM6DSO_OUTX_L_A 0x28

// Function Prototypes
uint8_t LSM6DSO_Init(I2C_HandleTypeDef *hi2c);
void LSM6DSO_ReadAccel(I2C_HandleTypeDef *hi2c, int16_t *x, int16_t *y, int16_t *z);
void LSM6DSO_ReadGyro(I2C_HandleTypeDef *hi2c, int16_t *x, int16_t *y, int16_t *z);

#endif /* LSM6DSO_H */
