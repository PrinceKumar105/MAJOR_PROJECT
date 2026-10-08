/*
 * MAX30102.h
 *
 *  Created on: Sep 29, 2026
 *      Author: KIIT0001
 */

#ifndef MAX30102_H
#define MAX30102_H

#include "stm32f4xx_hal.h"

// I2C Address (0x57 shifted left by 1)
#define MAX30102_ADDR           (0x57 << 1)

// Register Addresses
#define REG_INTR_STATUS_1       0x00
#define REG_INTR_ENABLE_1       0x02
#define REG_FIFO_WR_PTR         0x04
#define REG_OVF_COUNTER         0x05
#define REG_FIFO_RD_PTR         0x06
#define REG_FIFO_DATA           0x07
#define REG_FIFO_CONFIG         0x08
#define REG_MODE_CONFIG         0x09
#define REG_SPO2_CONFIG         0x0A
#define REG_LED1_PA             0x0C // Red LED
#define REG_LED2_PA             0x0D // IR LED
#define REG_PART_ID             0xFF

// Function Prototypes
uint8_t MAX30102_Init(I2C_HandleTypeDef *hi2c);
void MAX30102_Read_FIFO(I2C_HandleTypeDef *hi2c, uint32_t *red_led, uint32_t *ir_led);

#endif /* MAX30102_H */ /* INC_MAX30102_H_ */
