

#ifndef _BOARD_ITSB_H
#define _BOARD_ITSB_H



#include <stm32f10x.h>
#include <stdint.h> 
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "hal.h"
#include "fifo.h"
#include "messages.h"










/****************************************************************/
//! board
void boardInit(void);





/****************************************************************/
//! addr
void addrInit(void);
uint8_t readAddr(void);



/****************************************************************/
//! can/433 mode



/****************************************************************/
//! radio
void radioInit(void);



/****************************************************************/
//! net state indicator lamp
void ledInit(void);
void ledOn(void);
void ledOff(void);

void ledFlashInit(void);
void ledFlashSet(uint8_t chFlashCnt, uint16_t hwOnTime, uint16_t hwOffTime);
void ledFlashLoop(void);

/****************************************************************/
//! iic
void i2cInit(void);
void i2cWrite(uint8_t devAddr, uint8_t addr, uint8_t data);
uint8_t i2cRead(uint8_t devAddr, uint8_t addr);
bool i2cWaitEvent(uint32_t event);
bool i2cWaitEventCleared(uint32_t event);



/****************************************************************/
//! eeprom/AT24C256
#define AT24CX_ADDR			0xA0
#define AT24CX_PAGESIZE		64

void i2cEepromInit(void);
static bool i2cReadBlock(uint8_t* pBuffer, uint16_t readAddr, uint16_t n);
static bool i2cPageWrite(uint8_t* pBuffer, uint16_t writeAddr, uint8_t n);
static bool i2cWaitEepromStandbyState(void);
void eepromWaitEepromStandbyState(void);
void eepromPageWrite(uint8_t* pBuffer, uint16_t writeAddr, uint8_t n);
void eepromWriteBlock(uint8_t* pBuffer, uint32_t writeAddr, uint32_t n);
void eepromReadBlock(uint8_t* pBuffer, uint32_t readAddr, uint32_t n);
bool eepromAT24CXCheck(void);



/****************************************************************/
//! spi
void spiInit(void);
uint8_t spiReadWriteByte(uint8_t data);



/****************************************************************/
//! flash/W25Q16
#define FLASH_PAGESIZE   256

void spiFlashInit(void);
static uint8_t spiFlashReadStatusReg(void);
static void spiFlashWriteStatusReg(uint8_t data);
static void spiFlashWaitBusy(void);
static void spiFlashWriteEnable(void);
static void spiFlashWriteDisable(void);
void spiFlashRead(uint8_t *buffer, uint32_t readAddr, uint16_t n);
void spiFlashWritePage(uint8_t *buffer, uint32_t writeAddr, uint16_t n);
void spiFlashEraseChip(void);
void spiFlashEraseSector(uint32_t addrSect);
void spiFlashEraseBlock32(uint32_t addrBlock32);
void spiFlashEraseBlock64(uint32_t addrBlock64);
uint16_t spiFlashReadID(void);



/****************************************************************/
//! adc
void adcInit(void);
uint16_t getAnalogValue(uint8_t index);



/****************************************************************/
//! usart1/radio
void usart1Init(uint16_t baud);
void usart1Stop(void);
void usart1SendChar(uint8_t data);
void usart1SendBuffer(uint8_t *buffer, uint16_t count);

void radioReset(void);
static void radioSetM0M1(bool m0, bool m1);
void radioSetMode(uint8_t mode);
void radioGetParameter(void);
void radioGetVersion(void);
void radioSetParameter(uint16_t addr, uint32_t baud, uint32_t sped, uint8_t chan);










/****************************************************************/
//! usart2
void usart2Init(uint16_t baud);
void usart2Stop(void);
void usart2SendChar(uint8_t data);
void usart2SendBuffer(uint8_t *buffer, uint16_t count);



/****************************************************************/
//! timer2
void timer2Init(uint16_t ms);



/****************************************************************/
//! timer3



void timer3Init(uint16_t ms);



/****************************************************************/
//! watchdog
void wdgInit(void);
void wdgFeed(void);



/****************************************************************/
//! delay
void delayInit(void);
void delayMs(uint16_t nms);
void delayUs(uint32_t nus);





/****************************************************************/
//! some defineds

enum RADIO_MODE {
	 RADIO_MODE_NORMAL,
	 RADIO_MODE_WAKEUP,
	 RADIO_MODE_POWER_DOWN,
	 RADIO_MODE_SLEEP
};


enum ADC_CHANNEL {
	 ADC_CHANNEL_RED_VOLT,
	 ADC_CHANNEL_RED_CURT,
	 ADC_CHANNEL_YELLOW_VOLT,
	 ADC_CHANNEL_YELLOW_CURT,
	 ADC_CHANNEL_GREEN_VOLT,
	 ADC_CHANNEL_GREEN_CURT
};

#endif