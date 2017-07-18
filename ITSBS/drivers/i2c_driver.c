


#include "board.h"


#define I2C_TIMEOUT_MAX 1000




/**
  * @brief  iic init
  * @retval None
  */
void i2cInit(void) 
{
    GPIO_InitTypeDef  GPIO_InitStructure; 
    I2C_InitTypeDef   I2C_InitStructure; 
	
	RCC_APB2PeriphClockCmd(I2C1_RCC_APB2Periph, ENABLE);
    RCC_APB1PeriphClockCmd(I2C1_RCC_APB1Periph_I2C1, ENABLE);  
       
    GPIO_InitStructure.GPIO_Pin =  I2C1_GPIO_PIN_SCL | I2C1_GPIO_PIN_SDA;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;	 
    GPIO_Init(I2C1_GPIO, &GPIO_InitStructure);	

    I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
    I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
    I2C_InitStructure.I2C_OwnAddress1 = 0x00; 
    I2C_InitStructure.I2C_Ack = I2C_Ack_Enable ;
    I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_InitStructure.I2C_ClockSpeed = I2C1_SPEED;
  
    I2C_Cmd(I2C1, ENABLE);
 
    I2C_Init(I2C1, &I2C_InitStructure); 

	printf("ITSB: iic initialization OK ! \n");
}



/**
  * @brief  Writes a byte data to the EEPROM
  * @param  devAddr: The addr of the EEPROM.
  * @param  addr:    EEPROM's internal address to write to.
  * @param  data:    the data to be written to the EEPROM.
  * @retval None
  */
void i2cWrite(uint8_t devAddr, uint8_t addr, uint8_t data) 
{ 
   	I2C_AcknowledgeConfig(I2C1, ENABLE);
	while (I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));

    I2C_GenerateSTART(I2C2, ENABLE);
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT)); 

    I2C_Send7bitAddress(I2C1, devAddr, I2C_Direction_Transmitter);
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
  
    I2C_SendData(I2C1, addr);
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

    I2C_SendData(I2C1, data); 
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

    I2C_GenerateSTOP(I2C1, ENABLE);
}



/**
  * @brief  reads a byte data from the EEPROM
  * @param  devAddr: The addr of the EEPROM.
  * @param  addr:    EEPROM's internal address to be read.
  * @retval the data from the EEPROM.
  */
uint8_t i2cRead(uint8_t devAddr, uint8_t addr) 
{ 
    while (I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY)); 
      
    I2C_GenerateSTART(I2C1, ENABLE);
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
   
    I2C_Send7bitAddress(I2C1, devAddr, I2C_Direction_Transmitter);
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

    I2C_SendData(I2C1, addr);
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
      
    I2C_GenerateSTART(I2C1, ENABLE);
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
     
    I2C_Send7bitAddress(I2C1, devAddr + 1, I2C_Direction_Receiver);
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));

	I2C_AcknowledgeConfig(I2C1, DISABLE);
	I2C_GenerateSTOP(I2C1, ENABLE);

	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED));
	return I2C_ReceiveData(I2C1); 
}



bool i2cWaitEvent(uint32_t event) 
{
	uint32_t timeout = I2C_TIMEOUT_MAX;
	while (!I2C_CheckEvent(I2C1, event)) {
		if ((timeout--) == 0) return false;
	}
	return true;
}



bool i2cWaitEventCleared(uint32_t event) 
{
	uint32_t timeout = I2C_TIMEOUT_MAX;
	while (I2C_CheckEvent(I2C1, event)) {
		if ((timeout--) == 0) return false;
	}
	return true;
}





































