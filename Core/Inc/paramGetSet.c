/*
 * paramGetSet.c
 *
 *  Created on: Jan 19, 2025
 *      Author: stanislawzalewski
 */


#include "paramGetSet.h"
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

extern I2C_HandleTypeDef hi2c1;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim4;

uint16_t dig_T1;
int16_t dig_T2, dig_T3;

// This function checks the given the percentage value and transcripts it to right counter period for heater's PWM
void heater_PWM_Set(float *dutyCycle)
{
    int pwm_heater_value;

    // Check if duty cycle is within 0-100 range
    if (*dutyCycle >= 0 && *dutyCycle <= 100) {
        pwm_heater_value = (int)((*dutyCycle) * 999 / 100.0f);  // Convert 0-100 range to 0-999 range
        __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, pwm_heater_value);
    }
    else{
    	pwm_heater_value = 0;  // Convert 0-100 range to 0-999 range
    	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, pwm_heater_value);
    }
}


// This function checks the given the percentage value and transcripts it to right counter period for fan's PWM
void fan_PWM_Set(float *dutyCycle)
{
	int pwm_fan_value;
	if (*dutyCycle > 0 && *dutyCycle <= 100) {
		pwm_fan_value = (int)((*dutyCycle) * 999 / 100.0f);
		__HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_1, pwm_fan_value);
	}
}

// Function to write a value to a register (for control/commanding the BMP280)
uint16_t BMP280_Read16_LE(uint8_t reg) {
    uint8_t data[2];
    HAL_I2C_Master_Transmit(&hi2c1, BMP280_ADDR << 1, &reg, 1, HAL_MAX_DELAY);
    HAL_I2C_Master_Receive(&hi2c1, BMP280_ADDR << 1, data, 2, HAL_MAX_DELAY);
    return (data[1] << 8) | data[0];
}

int16_t BMP280_ReadS16_LE(uint8_t reg) {
    return (int16_t)BMP280_Read16_LE(reg);
}

// Function to read calibration data (read once at startup)
void BMP280_CalibrationParamRead(void)
{
	// Read calibration data
	    dig_T1 = BMP280_Read16_LE(CALIB_START);
	    dig_T2 = BMP280_ReadS16_LE(CALIB_START + 2);
	    dig_T3 = BMP280_ReadS16_LE(CALIB_START + 4);

	    // Set control measurement register (Normal mode, oversampling x1)
	    uint8_t cmd[2] = {CTRL_MEAS, 0x27};
	    HAL_I2C_Master_Transmit(&hi2c1, BMP280_ADDR << 1, cmd, 2, HAL_MAX_DELAY);
}

float BMP280_GetTempData(void)
{
	uint8_t reg = TEMP_MSB;
	    uint8_t data[3];

	    HAL_I2C_Master_Transmit(&hi2c1, BMP280_ADDR << 1, &reg, 1, HAL_MAX_DELAY);
	    HAL_I2C_Master_Receive(&hi2c1, BMP280_ADDR << 1, data, 3, HAL_MAX_DELAY);

	    int32_t rawTemp = ((int32_t)data[0] << 12) | ((int32_t)data[1] << 4) | (data[2] >> 4);

	    int32_t var1 = ((((rawTemp >> 3) - ((int32_t)dig_T1 << 1))) * ((int32_t)dig_T2)) >> 11;
	    int32_t var2 = (((((rawTemp >> 4) - ((int32_t)dig_T1)) * ((rawTemp >> 4) - ((int32_t)dig_T1))) >> 12) * ((int32_t)dig_T3)) >> 14;
	    int32_t t_fine = var1 + var2;

	    return (t_fine * 5 + 128) / 25600.0;  // Convert to °C
}




