/*
 * lsm6ds0.c
 *
 *  Created on: Oct 7, 2026
 *      Author: KIIT0001
 */

#include "lsm6dso.h"

uint8_t LSM6DSO_Init(I2C_HandleTypeDef *hi2c) {
    uint8_t who_am_i = 0;

    // Accel: 208 Hz ODR, +/- 2g Full Scale[cite: 50]
    uint8_t ctrl1_xl_val = 0x50;

    // Gyro: 208 Hz ODR, 250 dps Full Scale[cite: 51]
    uint8_t ctrl2_g_val = 0x50;

    // Check communication using the WHO_AM_I register
    HAL_I2C_Mem_Read(hi2c, LSM6DSO_I2C_ADDR, LSM6DSO_WHO_AM_I, 1, &who_am_i, 1, HAL_MAX_DELAY);

    if (who_am_i != 0x6C) {
        return 0; // Initialization failed
    }

    // Turn on Accelerometer
    HAL_I2C_Mem_Write(hi2c, LSM6DSO_I2C_ADDR, LSM6DSO_CTRL1_XL, 1, &ctrl1_xl_val, 1, HAL_MAX_DELAY);

    // Turn on Gyroscope
    HAL_I2C_Mem_Write(hi2c, LSM6DSO_I2C_ADDR, LSM6DSO_CTRL2_G, 1, &ctrl2_g_val, 1, HAL_MAX_DELAY);

    return 1; // Initialization successful
}

void LSM6DSO_ReadAccel(I2C_HandleTypeDef *hi2c, int16_t *x, int16_t *y, int16_t *z) {
    uint8_t data[6];
    if (HAL_I2C_Mem_Read(hi2c, LSM6DSO_I2C_ADDR, LSM6DSO_OUTX_L_A, 1, data, 6, HAL_MAX_DELAY) == HAL_OK) {
        *x = (int16_t)(data[1] << 8 | data[0]);
        *y = (int16_t)(data[3] << 8 | data[2]);
        *z = (int16_t)(data[5] << 8 | data[4]);
    }
}

void LSM6DSO_ReadGyro(I2C_HandleTypeDef *hi2c, int16_t *x, int16_t *y, int16_t *z) {
    uint8_t data[6];
    if (HAL_I2C_Mem_Read(hi2c, LSM6DSO_I2C_ADDR, LSM6DSO_OUTX_L_G, 1, data, 6, HAL_MAX_DELAY) == HAL_OK) {
        *x = (int16_t)(data[1] << 8 | data[0]);
        *y = (int16_t)(data[3] << 8 | data[2]);
        *z = (int16_t)(data[5] << 8 | data[4]);
    }
}
