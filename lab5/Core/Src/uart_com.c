/*
 * uart_com.c
 *
 *  Created on: Dec 9, 2024
 *      Author: Admin
 */
#include "uart_com.h"

uint8_t sucess_message[] = "Transmited, next transmit\r\n";
uint8_t fault_message[] = "Not respone \r\n";

uint8_t start = 0;
uint8_t end = 0;
uint8_t state = idel;
uint8_t len, length, leng;

void uart_com_fsm(void){
switch(state){
	case idel:
		if(flag_cmd){
			start = id_start;
			end = (id_buffer - 1 + MAX_BUFFER_SIZE) % MAX_BUFFER_SIZE;
			state = check_cmd;
			flag_cmd = 0;
		}
		break;
	case check_cmd:
		len = ((end - start + MAX_BUFFER_SIZE) % MAX_BUFFER_SIZE) - 1;
		if(len != 3){
			state = idel;
			break;
		}
		if(buffer[(start + 1) % MAX_BUFFER_SIZE] == 'R' && buffer[(start + 2) % MAX_BUFFER_SIZE] == 'S'
				&& buffer[(start + 3) % MAX_BUFFER_SIZE] == 'T'){
				state = send;
		}
		else{
				state = idel;
		}
		break;
	case send:
		ADC_value = HAL_ADC_GetValue (&hadc1);
		HAL_UART_Transmit(&huart2,(void *)str,sprintf(str, "ADC is %lu\r\n", ADC_value), 1000);
		setTimer(0,3000);
		activeTimer(0);
		state = wait_ack;
		break;
	case wait_ack:
		if(flag_cmd){
			HAL_UART_Transmit(&huart2, sucess_message, sizeof(sucess_message), 1000);
			start = id_start;
			end = (id_buffer - 1 + MAX_BUFFER_SIZE) % MAX_BUFFER_SIZE;
			flag_cmd = 0;
			length = ((end - start + MAX_BUFFER_SIZE) % MAX_BUFFER_SIZE) - 1;
			if(length == 2 && buffer[(start + 1) % MAX_BUFFER_SIZE] == 'O'
					&& buffer[(start + 2) % MAX_BUFFER_SIZE] == 'K'){
				ignoreTimer(0);
				HAL_UART_Transmit(&huart2, sucess_message, sizeof(sucess_message), 1000);
				state = idel;
				break;
			}
		}
		if(Timer_Flag[0]){
			HAL_UART_Transmit(&huart2, (void *)str, sprintf(str, "Resend ADC is %lu\r\n", ADC_value), 1000);
			setTimer(0,3000);
			setTimer(1,6000);
			activeTimer(1);
			state = resend;
		}
		break;
	case resend:
		if(Timer_Flag[0]){
			HAL_UART_Transmit(&huart2, (void *)str, sprintf(str, "Resend ADC is %lu\r\n", ADC_value), 1000);
			setTimer(0,3000);
		}

		if(flag_cmd){
			HAL_UART_Transmit(&huart2, sucess_message, sizeof(sucess_message), 1000);
			start = id_start;
			end = (id_buffer - 1 + MAX_BUFFER_SIZE) % MAX_BUFFER_SIZE;
			flag_cmd = 0;
			leng = ((end - start + MAX_BUFFER_SIZE) % MAX_BUFFER_SIZE) - 1;
			if(leng == 2 && buffer[(start + 1) % MAX_BUFFER_SIZE] == 'O'
					&& buffer[(start + 2) % MAX_BUFFER_SIZE] == 'K'){
				ignoreTimer(0);
				ignoreTimer(1);
				HAL_UART_Transmit(&huart2, sucess_message, sizeof(sucess_message), 1000);
				state = idel;
				break;
			}
		}

		else if(Timer_Flag[1]){
			ignoreTimer(0);
			ignoreTimer(1);
			state = fault;
		}
		break;
	case fault:
		HAL_UART_Transmit(&huart2, fault_message, sizeof(fault_message), 1000);
		state = idel;
		break;
	default:
		break;
	}
}

