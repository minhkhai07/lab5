/*
 * uart_com.h
 *
 *  Created on: Dec 9, 2024
 *      Author: Admin
 */

#ifndef INC_UART_COM_H_
#define INC_UART_COM_H_
#include "Global.h"
#include "Timer.h"
#define idel				0
#define check_cmd			1
#define send				2
#define wait_ack			3
#define resend				4
#define fault				5
extern UART_HandleTypeDef huart2;
extern ADC_HandleTypeDef hadc1;
void uart_com_fsm(void);


#endif /* INC_UART_COM_H_ */
