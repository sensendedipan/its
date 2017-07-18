



#include "board.h"




/**
  * @brief  if the eeprom check well then we concerned the eeprom init well
  * @retval none 
  */
void i2cEepromInit(void)
{
	if (eepromAT24CXCheck() == true) {
		printf("ITSB: at24C256 initialization OK ! \n");
		
	} else {
		printf("ITSB: at24C256 initialization Failed ! \n");
	}
}

/**
  * @brief  Reads a block of data from the EEPROM.
  * @param  pBuffer : pointer to the buffer that receives the data read from the EEPROM.
  * @param  readAddr : EEPROM's internal address to read from.
  * @param  n : number of bytes to read from the EEPROM.
  * @retval None
  */
static bool i2cReadBlock(uint8_t* pBuffer, uint16_t readAddr, uint16_t n) 
{
	if (!i2cWaitEventCleared(I2C_FLAG_BUSY))
		return false;

	I2C_GenerateSTART(I2C1, ENABLE);
	if (!i2cWaitEvent(I2C_EVENT_MASTER_MODE_SELECT))
		return false;

	I2C_Send7bitAddress(I2C1, AT24CX_ADDR, I2C_Direction_Transmitter);
	if (!i2cWaitEvent(I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
		return false;

	I2C_SendData(I2C1, (uint8_t)((readAddr & 0xFF00) >> 8));
	if (!i2cWaitEvent(I2C_EVENT_MASTER_BYTE_TRANSMITTING))
		return false;
	
	I2C_SendData(I2C1, (uint8_t)(readAddr & 0x00FF));
	if (!i2cWaitEvent(I2C_EVENT_MASTER_BYTE_TRANSMITTED))
		return false;

	I2C_GenerateSTART(I2C1, ENABLE);
	if (!i2cWaitEvent(I2C_EVENT_MASTER_MODE_SELECT))
		return false;

	I2C_Send7bitAddress(I2C1, AT24CX_ADDR, I2C_Direction_Receiver);
	if (!i2cWaitEvent(I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
		return false;

	if (n > 1) {
		I2C_AcknowledgeConfig(I2C1, ENABLE);
	}

	while (n) {
		if (n == 1) {
			I2C_AcknowledgeConfig(I2C1, DISABLE);
		}
		if (!i2cWaitEvent(I2C_EVENT_MASTER_BYTE_RECEIVED))
			return false;
		*pBuffer++ = I2C_ReceiveData(I2C1);
		n--;
	}

	I2C_GenerateSTOP(I2C1, ENABLE);
	return true;
}


/**
  * @brief  Writes more than one byte to the EEPROM with a single WRITE cycle.
  * @note   The number of byte can't exceed the EEPROM page size.
  * @param  pBuffer : pointer to the buffer containing the data to be written to the EEPROM.
  * @param  writeAddr : EEPROM's internal address to write to.
  * @param  n : number of bytes to write to the EEPROM.
  * @retval None
  */
static bool i2cPageWrite(uint8_t* pBuffer, uint16_t writeAddr, uint8_t n) 
{
	if (n >= AT24CX_PAGESIZE)
		return false;
	
	if (!i2cWaitEventCleared(I2C_FLAG_BUSY))
		return false;

	I2C_GenerateSTART(I2C1, ENABLE);
	if (!i2cWaitEvent(I2C_EVENT_MASTER_MODE_SELECT))
		return false;

	I2C_Send7bitAddress(I2C1, AT24CX_ADDR, I2C_Direction_Transmitter);
	if (!i2cWaitEvent(I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
		return false;

	I2C_SendData(I2C1, (uint8_t)((writeAddr & 0xFF00) >> 8));
	if (!i2cWaitEvent(I2C_EVENT_MASTER_BYTE_TRANSMITTING))
		return false;
	
	I2C_SendData(I2C1, (uint8_t)(writeAddr & 0x00FF));
	if (!i2cWaitEvent(I2C_EVENT_MASTER_BYTE_TRANSMITTING))
		return false;

	/* While there is data to be written */
	while (n--) {
		I2C_SendData(I2C1, *pBuffer);
		if (!i2cWaitEvent(I2C_EVENT_MASTER_BYTE_TRANSMITTING))
			return false;
		pBuffer++;
	}

	if (!i2cWaitEvent(I2C_EVENT_MASTER_BYTE_TRANSMITTED))
		return false;

	I2C_GenerateSTOP(I2C1, ENABLE);
	return true;
}



/**
  * @brief  Writes buffer of data to the I2C EEPROM.
  * @param  pBuffer : pointer to the buffer  containing the data to be written to the EEPROM.
  * @param  writeAddr : EEPROM's internal address to write to.
  * @param  n : number of bytes to write to the EEPROM.
  * @retval None
  */
void eepromPageWrite(uint8_t* pBuffer, uint16_t writeAddr, uint8_t n) 
{
	while (!i2cPageWrite(pBuffer, writeAddr, n)) {
		i2cInit();
	}
}


/**
  * @brief  Wait for EEPROM Standby state
  * @param  None
  * @retval None
  */
static bool i2cWaitEepromStandbyState(void) 
{
	do {
		I2C_GenerateSTART(I2C1, ENABLE);
		if (!i2cWaitEvent(I2C_EVENT_MASTER_MODE_SELECT))
		return false;

		I2C_Send7bitAddress(I2C1, AT24CX_ADDR, I2C_Direction_Transmitter);
	} while (!i2cWaitEvent(I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

	I2C_GenerateSTOP(I2C1, ENABLE);
	return true;
}



/**
  * @brief  
  * @note   
  * @retval None
  */
void eepromWaitEepromStandbyState(void) 
{
	while (!i2cWaitEepromStandbyState()) {
		i2cInit();
	}
}



/**
  * @brief  Writes buffer of data to the I2C EEPROM.
  * @param  pBuffer : pointer to the buffer  containing the data to be written to the EEPROM.
  * @param  writeAddr : EEPROM's internal address to write to.
  * @param  n : number of bytes to write to the EEPROM.
  * @retval None
  */
void eepromWriteBlock(uint8_t* pBuffer, uint32_t writeAddr, uint32_t n) 
{
	uint8_t offset = writeAddr % AT24CX_PAGESIZE;
	uint8_t count = AT24CX_PAGESIZE - offset;
	if (n < count) {
		count = n;
	}
	
	while (count > 0) {
		eepromPageWrite(pBuffer, writeAddr, count);
		eepromWaitEepromStandbyState();
		writeAddr += count;
		pBuffer += count;
		n -= count;
		count = AT24CX_PAGESIZE;
		if (n < AT24CX_PAGESIZE) {
			count = n;
		}
	}
}



/**
  * @brief  Reads a block of data from the EEPROM.
  * @param  pBuffer : pointer to the buffer that receives the data read from the EEPROM.
  * @param  ReadAddr : EEPROM's internal address to read from.
  * @param  NumByteToRead : number of bytes to read from the EEPROM.
  * @retval None
  */
void eepromReadBlock(uint8_t* pBuffer, uint32_t readAddr, uint32_t n) 
{
	while (!i2cReadBlock(pBuffer, readAddr, n)) {
		i2cInit();
	}
}



/**
  * @brief  check if the eeprom is good or bad
  * @brief  store a flag in the last addr(32768 bytes) of the AT24C256 
  * @retval false: test failed  
  *         true:  test OK
  */
bool eepromAT24CXCheck(void) 
{
	uint8_t temp;
	uint8_t *ptemp = &temp;
	eepromReadBlock(ptemp, 32767, 1);
	
	if (*ptemp == 0x55) {
		return true;
		
	} else {
		*ptemp = 0x55;
		eepromWriteBlock(ptemp, 32767, 1);
		eepromReadBlock(ptemp, 32767, 1);
		
		if (*ptemp == 0x55) return false;
	}
	
	return false;	
}








