/*
 * tempControl.c
 *
 *  Created on: Jan 20, 2025
 *      Author: stanislawzalewski
 */


#include "tempControl.h"


float actualTemperature = 45.27;
float targetTemperature = 40.25;

float Kp = 12.5;  // Proportional gain 13.0
float Ki = 0.35;  // Integral gain 0.45
float Kd = 0.035; // Derivative gain 0.035 was good at standing a temperature

float error = 0.0;
float previousError = 0.0;
float integral = 0.0;     // Integral term accumulator
float derivative = 0.0;   // Derivative term
float outputGeneral = 0.0;
float outputHeater = 0.0;
float outputFan = 0.0;

// Function to perform PID control
void PID_Control(void) {
    // Calculate error
    error = targetTemperature - actualTemperature;

    // Calculate PID terms
    integral += error * 0.01; // 0.01 is the time step (adjust as needed)
    derivative = (error - previousError) / 0.01;

    // Calculate PID output
    outputGeneral = Kp * error + Ki * integral + Kd * derivative;

    // Prevent integral windup when heater output is saturated
    if (outputHeater == 75.0 || outputHeater == 0.0) {
        integral = 0;
    }

    // Heater control logic
    outputHeater = outputGeneral;
    if (outputHeater > 75.0) outputHeater = 75.0;
    if (outputHeater < 0.0) outputHeater = 0.0;

    // Fan control logic
    if (error < 0.0) {
        // Calculate fan output for cooling
        outputFan = -error * Kp;  // Proportional to temperature overshoot
        if (outputFan > 100.0) outputFan = 100.0;
    } else {
        // Turn off fan if not needed
        outputFan = 0.0;
    }

    // Apply outputs
    heater_PWM_Set(&outputHeater);
    fan_PWM_Set(&outputFan);

    // Update previous error
    previousError = error;
}
