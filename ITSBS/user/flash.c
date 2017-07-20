

#include "itsb.h"





/**
  * @brief  get the cpu unique ID
  * @note   
  * @retval None
  */
bool getCpuId(uint32_t *mac_0, uint32_t *mac_1, uint32_t *mac_2)
{
	*mac_0 = *(uint32_t*)(0x1FFFF7E8);
	*mac_1 = *(uint32_t*)(0x1FFFF7EC);
	*mac_2 = *(uint32_t*)(0x1FFFF7F0);
	
	if ((*mac_0 != 0) || (*mac_1 != 0) || (*mac_2 != 0)) {
		return true;
		
	} else {
		return false;
	}
	
}



/**
  * @brief  get the ask for id period of radio by crc the cpuid 
  * @note   if get cpu id ok, just use the last 4 bits of crc result as period
  * @note   if get cpu id failed, use 1 as the default period
  * @retval None
  */
uint16_t getAskForIdPeriod(void)
{
	uint16_t value;
	uint32_t mac[3];
	
	if (getCpuId(&mac[0], &mac[1], &mac[2])) {
		value = (crc16((uint8_t *)mac, sizeof(mac)) & (0x000F));
		
		if (value == 0) {
			value = 100;
			
		} else {
			value = value*50;
		} 			
		
	} else {
		
		value = 500;
	}
	
	return value;
}



/**
  * @brief  unlock the stm32f103 inner flash
  * @note   
  * @retval None
  */
void unlockFlash(void)
{
	FLASH->KEYR = 0x45670123;
	FLASH->KEYR = 0xCDEF89AB;	
}



/**
  * @brief  lock the stm32f103 inner flash
  * @note   
  * @retval None
  */
void lockFlash(void)
{
	while (!FLASH->SR & FLASH_SR_BSY);
	FLASH->CR |= FLASH_CR_LOCK;
}



void waitFlashIdle(void)
{
	while (FLASH->SR & FLASH_FLAG_BSY) {
		wdgFeed();
	}
}



#define  STARTADDR  0x08000000     //! start addr              	 
volatile FLASH_Status FLASHStatus = FLASH_COMPLETE;  



/**
  * @brief  read n bytes from the inner flash
  * @param  readAddr: the data addr inside of the flash
  * @param  *readBuf: the destination addr the data save to
  * @param  n		: read n bytes
  * @retval 		:
  */
uint32_t readFlashBtyes(uint32_t readAddr, uint8_t *readBuf, uint32_t n) 
{
	uint32_t DataNum = 0;
	readAddr = (uint32_t)STARTADDR + readAddr; 
	
	while(DataNum < n) {
	   *(readBuf + DataNum) = *(__IO uint8_t*) readAddr++;
	   DataNum++;
	}
	return DataNum;
}



/**
  * @brief  write a word to the inner flash
  * @param  writeAddr: the addr of the data should write to
  * @param  data     : the right data
  * @retval none
  */
void writeFlashOneWord(uint32_t writeAddr,uint32_t data)
{
	FLASH_UnlockBank1();
	FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR); 
    FLASHStatus = FLASH_ErasePage(STARTADDR);

	if (FLASHStatus == FLASH_COMPLETE) {
		FLASHStatus = FLASH_ProgramWord(STARTADDR + writeAddr, data);   
	}
	FLASH_LockBank1();
} 





















