# Temperature and Fan Control System for STM32 Nucleo F7

## Overview
This repository contains the source code for a temperature control system utilizing PWM to manage both a 40W heater and a cooling fan. The system leverages an STM32 Nucleo F7 microcontroller to ensure precise temperature regulation with a PID controller.

### Project Features:
- **Temperature Control**: Uses PWM signals to control N-channel MOSFETs connected to a 40W heater.
- **Fan Speed Control**: Adjusts fan speed with a separate PWM signal for cooling.
- **PID Control**: Implements a PID controller in `tempControl.c` to maintain temperature with less than 1% steady-state error.
- **Sensor Integration**: Utilizes the GY-280 thermometer sensor via I2C for temperature readings.
- **Communication**: Sends system status via UART in JSON format and allows setting target temperature through commands.

## Hardware Requirements
- **STM32 Nucleo F7 Board**
- N-channel MOSFET for heater control
- 40W Heater
- Fan with PWM control capability
- GY-280 Thermometer Sensor

## Software Structure
The project is organized into the following files:

### main.c
- Initialization of all peripherals and system start-up.

### paramGetSet.c
- **Functions:**
  - `heater_PWM_Set()`: Sets the duty cycle for the heater.
  - `fan_PWM_Set()`: Adjusts the fan speed.
  - I2C communication functions for interfacing with the GY-280 sensor.

### uartGetSet.c
- **UART Communication:**
  - Sends JSON formatted data including:
    - Target temperature
    - Actual temperature
    - Duty cycles for both heater and fan
  - Receives commands to set target temperature via "Target:" command.

### tempControl.c
- **PID Controller Implementation:**
  - Functions for PID calculation to achieve precise temperature control.

## Installation
1. Clone this repository:
   ```bash
   git clone [your-repo-url]
