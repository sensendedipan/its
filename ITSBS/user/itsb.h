

#ifndef _ITSB_H
#define _ITSB_H

#include "board.h"
#include "eeprom.h"

#define ASK_FOR_ID_RETRY_MAX	5	//! if ask for id more than 5 times, then modify send period

extern node_t myNode;

extern bool gNetworkRunning;
extern bool gCanAskForIdTriger;
extern bool gCanAskForIdDurNormMode;



uint16_t crc16(uint8_t * buf, uint32_t len);

bool getCpuId(uint32_t *mac_0, uint32_t *mac_1, uint32_t *mac_2);
uint8_t getAskForIdPeriod(void);


void lockFlash(void);
void unlockFlash(void);
void writeFlashOneWord(uint32_t writeAddr,uint32_t data);
uint32_t readFlashBtyes(uint32_t readAddr, uint8_t *readBuf, uint32_t n);




void taskParseRadioData(void);
void taskNormalMission(void);
itsbs_mode_t taskMesh(void);
void taskSelfCheck(void);







































#endif