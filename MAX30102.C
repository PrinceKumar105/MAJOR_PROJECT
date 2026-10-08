/*
 * MAX30102.c
 *
 *  Created on: Sep 29, 2026
 *      Author: KIIT0001
 */


#include "MAX30102.h"

// Helper function to write to a register
static void Write_Reg(I2C_HandleTypeDef *hi2c, uint8_t reg, uint8_t value) {
    HAL_I2C_Mem_Write(hi2c, MAX30102_ADDR, reg, I2C_MEMADD_SIZE_8BIT, &value, 1, 100);
}

uint8_t MAX30102_Init(I2C_HandleTypeDef *hi2c) {
    uint8_t part_id = 0;

    // Check Part ID (Should be 0x15)
    HAL_I2C_Mem_Read(hi2c, MAX30102_ADDR, REG_PART_ID, I2C_MEMADD_SIZE_8BIT, &part_id, 1, 100);
    if (part_id != 0x15) {
        return 0; // Initialization failed
    }

    // Software Reset
    Write_Reg(hi2c, REG_MODE_CONFIG, 0x40);
    HAL_Delay(50);

    // FIFO Config: Sample Averaging = 4, Rollover Enable
    Write_Reg(hi2c, REG_FIFO_CONFIG, 0x50);

    // Mode Config: SpO2 mode (Red and IR)
    Write_Reg(hi2c, REG_MODE_CONFIG, 0x03);

    // SpO2 Config: ADC Range = 4096nA, Sample Rate = 100Hz, Pulse Width = 411us (18-bit)
    Write_Reg(hi2c, REG_SPO2_CONFIG, 0x27);

    // LED Pulse Amplitude: ~6.4mA for both Red and IR
    Write_Reg(hi2c, REG_LED1_PA, 0x24);
    Write_Reg(hi2c, REG_LED2_PA, 0x24);

    // Enable FIFO Ready Interrupt
    Write_Reg(hi2c, REG_INTR_ENABLE_1, 0x40);

    // Reset FIFO Pointers
        Write_Reg(hi2c, REG_FIFO_WR_PTR, 0x00);
        Write_Reg(hi2c, REG_OVF_COUNTER, 0x00);
        Write_Reg(hi2c, REG_FIFO_RD_PTR, 0x00);

        // CLEAR LINGERING INTERRUPTS BEFORE STARTING
        uint8_t dummy;
        HAL_I2C_Mem_Read(hi2c, MAX30102_ADDR, REG_INTR_STATUS_1, I2C_MEMADD_SIZE_8BIT, &dummy, 1, 100);

        return 1; // Initialization successful
    }

void MAX30102_Read_FIFO(I2C_HandleTypeDef *hi2c, uint32_t *red_led, uint32_t *ir_led) {
    uint8_t temp[6];
    uint8_t int_status;

    // 1. Read Interrupt Status 1 to clear the flag and release the INT pin HIGH
    HAL_I2C_Mem_Read(hi2c, MAX30102_ADDR, REG_INTR_STATUS_1, I2C_MEMADD_SIZE_8BIT, &int_status, 1, 100);

    // 2. Read 6 bytes from the FIFO data register
    if (HAL_I2C_Mem_Read(hi2c, MAX30102_ADDR, REG_FIFO_DATA, I2C_MEMADD_SIZE_8BIT, temp, 6, 100) == HAL_OK) {
        // Mask to 18-bit and shift
        *red_led = ((uint32_t)temp[0] << 16 | (uint32_t)temp[1] << 8 | temp[2]) & 0x03FFFF;
        *ir_led  = ((uint32_t)temp[3] << 16 | (uint32_t)temp[4] << 8 | temp[5]) & 0x03FFFF;
    }
}
