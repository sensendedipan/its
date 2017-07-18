

#ifndef _ITSB_H
#define _ITSB_H

#include "board.h"
#include "eeprom.h"

#define NODE_ID_NUM_MAX		20 //! supporst up to 20 nodes
#define NODE_BAD_CNT_MAX	10  //! if ask for data without ack more than this times, then suppose this node down  

extern uint8_t  node_current;
extern uint16_t g_id_request;
extern bool g_flag_ask_for_data;
extern node_t node[NODE_ID_NUM_MAX];
extern bool g_flag_check_nodes;

extern uint8_t g_test;


uint16_t crc16(uint8_t * buf, uint32_t len);




bool getCpuId(uint32_t *mac_0, uint32_t *mac_1, uint32_t *mac_2);
uint8_t getAskForIdPeriod(void);


void lockFlash(void);
void unlockFlash(void);
void writeFlashOneWord(uint32_t writeAddr, uint32_t data);
uint32_t readFlashBtyes(uint32_t readAddr, uint8_t *readBuf, uint32_t n);



void checkNodesState(bool flag);
void taskParseRadioData(void);












typedef enum {
	ITS_MODE_UNINIT = 0,
	ITS_MODE_IDLE,
	ITS_MODE_MESH,
	ITS_MODE_NORMAL,
	ITS_MODE_NONE	
} itsm_mode_t; //! the ITS master work mode





#endif