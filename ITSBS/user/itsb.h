

#ifndef _ITSB_H
#define _ITSB_H

#include "board.h"
#include "eeprom.h"

#define ASK_FOR_ID_RETRY_MAX	20	//! if ask for id more than ASK_FOR_ID_RETRY_MAX/2 times, then modify send period; if more than ASK_FOR_ID_RETRY_MAX times, selfcheck or reboot

extern node_t myNode;

extern bool gNetworkRunning;
extern bool gCanAskForIdTriger;
extern bool gCanAskForIdDurNormMode;
extern uint16_t gRadioNoDataCnt;


uint16_t crc16(uint8_t * buf, uint32_t len);

bool getCpuId(uint32_t *mac_0, uint32_t *mac_1, uint32_t *mac_2);
uint16_t getAskForIdPeriod(void);


void lockFlash(void);
void unlockFlash(void);
void writeFlashOneWord(uint32_t writeAddr,uint32_t data);
uint32_t readFlashBtyes(uint32_t readAddr, uint8_t *readBuf, uint32_t n);

void systemReboot(void);


void taskParseRadioData(void);
itsbs_mode_t taskNormalMission(void);
itsbs_mode_t taskMesh(void);
itsbs_mode_t taskSelfCheck(void);






































#endif