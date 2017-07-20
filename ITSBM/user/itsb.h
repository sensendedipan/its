

#ifndef _ITSB_H
#define _ITSB_H

#include "board.h"
#include "eeprom.h"

#define NODE_ID_NUM_MAX		20 	//! supporst up to 20 nodes
#define NODE_BAD_CNT_MAX	10  //! if ask for data without ack more than this times, then suppose this node down  
#define TRAVERSE_PERIOD   	200 //! ask for data per 200ms
#define MESH_TIME_MAX		5	//! if can't receive ask for id msg more than 5 seconds, then suppose mesh finished!  

extern bool gGetStartNewMeshAck;
extern bool gMeshFinished;
extern bool gCheckNotesState;
extern bool gCanAskForDataPeriod;
extern node_t node[NODE_ID_NUM_MAX];



uint16_t crc16(uint8_t * buf, uint32_t len);




bool getCpuId(uint32_t *mac_0, uint32_t *mac_1, uint32_t *mac_2);
uint8_t getAskForIdPeriod(void);


void lockFlash(void);
void unlockFlash(void);
void writeFlashOneWord(uint32_t writeAddr, uint32_t data);
uint32_t readFlashBtyes(uint32_t readAddr, uint8_t *readBuf, uint32_t n);



void checkNodesStatePeriod(void);
void askForDataPeriod(void);
void startNewMesh(void);
void taskParseRadioData(void);
















#endif