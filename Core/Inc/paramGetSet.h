/*
 * paramGetSet.h
 *
 *  Created on: Jan 19, 2025
 *      Author: stanislawzalewski
 */

#ifndef INC_PARAMGETSET_H_
#define INC_PARAMGETSET_H_

#include <main.h>

#define BMP280_ADDR 0x76

// BMP280 Registers
#define TEMP_MSB  0xFA
#define CALIB_START 0x88
#define CTRL_MEAS 0xF4

extern uint16_t dig_T1;
extern int16_t dig_T2, dig_T3;


void heater_PWM_Set(float *dutyCycle);

void fan_PWM_Set(float *dutyCycle);

uint16_t BMP280_Read16_LE(uint8_t reg);
int16_t BMP280_ReadS16_LE(uint8_t reg);
void BMP280_CalibrationParamRead(void);
float BMP280_GetTempData(void);


#endif /* INC_PARAMGETSET_H_ */
