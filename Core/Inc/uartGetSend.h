/*
 * uartGetSend.h
 *
 *  Created on: Jan 19, 2025
 *      Author: stanislawzalewski
 */

#ifndef SRC_UARTGETSEND_H_
#define SRC_UARTGETSEND_H_

#define MAX_CMD_LEN 13

#include <main.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

extern UART_HandleTypeDef huart3;

extern char uartTempJson[100];
extern char uartCommandReceive[MAX_CMD_LEN + 1];  // Buffer to store the received command
extern uint8_t receivedBytes[MAX_CMD_LEN];


void UART_SendCommand(char *command);
void UART_TargetActual_Json(float *actualTemp, float *targetTemp, float *duty_heater, float *duty_fan);
void ProcessReceivedString(char *receivedCommand);
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);

#endif /* SRC_UARTGETSEND_H_ */
