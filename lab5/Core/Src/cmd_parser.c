/*
 * cmd_parser.c
 *
 *  Created on: Dec 9, 2024
 *      Author: Admin
 */
#include "cmd_parser.h"

uint8_t pr_state = pr_idel;

void cmd_parser_fsm(void){
	switch(pr_state){
	case pr_idel:
		if(tmp == '!'){
			id_start = (id_buffer - 1 + MAX_BUFFER_SIZE) % MAX_BUFFER_SIZE;
			pr_state = pr_start;
		}
		break;
	case pr_start:
		if(tmp == '!'){
			id_start = (id_buffer - 1 + MAX_BUFFER_SIZE) % MAX_BUFFER_SIZE;
			pr_state = pr_start;
		}
		if(tmp == '#'){
			flag_cmd = 1;
			pr_state = pr_idel;
		}
		break;
	default:
		break;
	}
}

