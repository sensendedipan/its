

#ifndef __EEPROM_H
#define __EEPROM_H


#include <stm32f10x.h>
#include <stdint.h> 
#include <stdio.h>
#include <stdbool.h>
#include <string.h>




typedef struct __node
{	
	uint8_t  id; 	
	uint8_t  state;
	uint8_t  bad_cnt;
	uint8_t  dev_type;
	uint8_t  fault_type;
	uint8_t  fault_scale;
	uint32_t run_time;
	uint16_t red_u;
	uint16_t red_i;
	uint16_t yellow_u;
	uint16_t yellow_i;
	uint16_t green_u;
	uint16_t green_i;
	uint32_t mac[3];
} node_t;






















typedef enum NODE_STATE
{
    NODE_STATE_OFFLINE = 0,
    NODE_STATE_ONLINE
} NODE_STATE; 









#endif