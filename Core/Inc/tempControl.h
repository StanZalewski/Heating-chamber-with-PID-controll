/*
 * tempControl.h
 *
 *  Created on: Jan 20, 2025
 *      Author: stanislawzalewski
 */

#ifndef SRC_TEMPCONTROL_H_
#define SRC_TEMPCONTROL_H_

#include <main.h>
extern TIM_HandleTypeDef htim2;

extern float actualTemperature;
extern float targetTemperature;

extern float Kp;  // Proportional gain
extern float Ki;  // Integral gain
extern float Kd; // Derivative gain

extern float error;
extern float previous_error;
extern float integral;     // Integral term accumulator
extern float derivative;   // Derivative term
extern float outputGeneral;       // PID output
extern float outputHeater;
extern float outputFan;

void PID_Control(void);

#endif /* SRC_TEMPCONTROL_H_ */
