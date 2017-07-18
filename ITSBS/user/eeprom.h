
#ifndef __EEPROM_H
#define __EEPROM_H


#include <stm32f10x.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>





typedef struct __node
{
	uint8_t  node_id;
	uint8_t  state;
	uint8_t  bad_cnt;
	uint8_t  dev_type;
	uint8_t  fault_type;
	uint8_t  fault_scale;
	uint8_t  cfdt;
	uint16_t red_u;
	uint16_t red_i;
	uint16_t yellow_u;
	uint16_t yellow_i;
	uint16_t green_u;
	uint16_t green_i;
	uint32_t mac[3];
} node_t;





#endif