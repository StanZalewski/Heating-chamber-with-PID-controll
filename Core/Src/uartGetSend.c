/*
 * uartGetSend.c
 *
 *  Created on: Jan 19, 2025
 *      Author: stanislawzalewski
 */

#include "uartGetSend.h"
#include <stdint.h>
#include <string.h>
#include <stdio.h>


extern float targetTemperature; // variable from tempControl


#define MAX_CMD_LEN 13


char uartTempJson[100];

char uartCommandReceive[MAX_CMD_LEN + 1];  // Buffer to store the received command
uint8_t receivedBytes[MAX_CMD_LEN];  // Buffer to receive data in one go  // Buffer to receive data in one go





void UART_SendCommand(char *command)
{
    while (*command)
    {
    	if (*command == '\r'){
    		HAL_UART_Transmit(&huart3, (uint8_t*)command, 1,HAL_MAX_DELAY);
    		memset(command, 0, sizeof(command)+1);
    		break;
    	}
    	HAL_UART_Transmit(&huart3, (uint8_t*)command, 1, HAL_MAX_DELAY);
        command++;
    }
}


void UART_TargetActual_Json(float *actualTemp, float *targetTemp, float *duty_heater, float *duty_fan){
	int decimalActual = (int)*actualTemp;
	int fractionalActual = (int)((*actualTemp - decimalActual) * 100);

	int decimalTarget = (int)*targetTemp;
	int fractionalTarget = (int)((*targetTemp - decimalTarget) * 100);

	int dutyH = (int)(*duty_heater);
	int dutyF = (int)(*duty_fan);

	sprintf(uartTempJson, "{\"actual\": \"%d.%02d\", \"target\": \"%d.%02d\", \"Duty_heater\": \"%d\", \"Duty_fan\": \"%d\"}\r", decimalActual, fractionalActual, decimalTarget, fractionalTarget, dutyH, dutyF);
	UART_SendCommand(uartTempJson);

}

void ProcessReceivedString(char *receivedCommand)
{
    float tempValue;

    // Check if the command matches "Target:<float>"
    if (sscanf(receivedCommand, "Target:%f", &tempValue) == 1)
    {
        targetTemperature = tempValue;  // Assign parsed value to global variable
    }
    else
    {
    	char errorCommand[20]; //error message
    	strcpy(errorCommand, "Wrong Command!!!");
    	UART_SendCommand(errorCommand);
    }
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    // Re-enable UART receive interrupt for next batch
    HAL_UART_Receive_IT(&huart3, receivedBytes, MAX_CMD_LEN);

    // Check if the interrupt is from the correct UART instance
    if (huart->Instance == USART3) {
        uint8_t i = 0;

        // Process the received data (the whole batch)
        // Copy the received bytes until '\r' or MAX_CMD_LEN
        while (i < MAX_CMD_LEN && receivedBytes[i] != '\r') {
            uartCommandReceive[i] = receivedBytes[i];
            i++;
        }

        // Null-terminate the string
        uartCommandReceive[i] = '\0';

        // If '\r' is found, process the command
        if (i < MAX_CMD_LEN && receivedBytes[i] == '\r') {
            ProcessReceivedString(uartCommandReceive);
        } else {
            // Handle the case where no '\r' was found within the buffer (optional)
            // For example, handle buffer overflow or incomplete command
            uartCommandReceive[0] = '\0';  // Reset to avoid incorrect processing
        }

        // Reset the received data buffer for the next reception
        memset(receivedBytes, 0, MAX_CMD_LEN);
    }}




