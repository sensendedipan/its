
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
	uint32_t run_time;
	uint32_t mac[3];
} node_t;









typedef struct __can_msg_t
{
	uint32_t stdId;
	uint32_t extId;
	uint8_t  ide;
	uint8_t  rtr;
	uint8_t  dlc;
	uint8_t  fmi;
	uint8_t  data[8];
	
} can_msg_t;

























typedef enum {
    ITS_MODE_UNINIT = 0,
    ITS_MODE_IDLE,
	ITS_MODE_GET_PERIOD,
	ITS_MODE_MESH,
	ITS_MODE_NORMAL_MISSION,
	ITS_MODE_SELF_CHECK,
	ITS_MODE_NONE
} itsbs_mode_t; //! The ITS slaver work mode


#endif