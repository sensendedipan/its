



#include "board.h"


/* W25Q16   = 16Mbit = 2M-byte = 32 Blocks = 512 Sectors = 8192 Pages = 2097152 Bytes
 * a page   = 256-bytes
 * a sector = 16pages = 4K-bytes 
 * a block  = 16sectors = 256pages = 64K-bytes

 *   block_0 -- block_31:   32blocks
 *   0x000000h-0x1fffffh 共32个block 每个占64K
 *   block_0 : 0x000000  -  0x00ffff
 *   block_1 : 0x010000  -  0x01ffff
 *   block_2 : 0x020000  -  0x02ffff
 *   block_3 : 0x030000  -  0x03ffff 
 *   block_4 : 0x040000  -  0x04ffff
 *   block_5 : 0x050000  -  0x05ffff 
 *   block_6 : 0x060000  -  0x06ffff 
 *   block_7 : 0x070000  -  0x07ffff
 *   block_8 : 0x080000  -  0x08ffff 
 *   block_9 : 0x090000  -  0x06ffff 
 *   block_10: 0x0A0000  -  0x07ffff
 *   block_11: 0x0B0000  -  0x08ffff     
 *                .
 */
 
#define FLASH_ID                0XEF14//W25Q16 ID
#define W25Q_WriteEnable		0x06 
#define W25Q_WriteDisable		0x04 
#define W25Q_ReadStatusReg		0x05 
#define W25Q_WriteStatusReg		0x01 
#define W25Q_ReadData			0x03 
#define W25Q_FastReadData		0x0B 
#define W25Q_FastReadDual		0x3B 
#define W25Q_PageProgram		0x02 
#define W25Q_BlockErase64K  	0xD8 
#define W25Q_BlockErase32K  	0x52 
#define W25Q_SectorErase		0x20 
#define W25Q_ChipErase			0xC7 
#define W25Q_PowerDown			0xB9 
#define W25Q_ReleasePowerDown	0xAB 
#define W25Q_DeviceID			0xAB 
#define W25Q_ManufactDeviceID	0x90 
#define W25Q_JedecDeviceID		0x9F 

#define FLASH_DESELECT()        GPIO_SetBits(FLASH_GPIO_REG,   FLASH_GPIO_PIN_CS)
#define FLASH_SELECT()          GPIO_ResetBits(FLASH_GPIO_REG, FLASH_GPIO_PIN_CS)



/**
  * @brief  initialize the flash's cs pin
  */
void spiFlashInit(void) 
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(FLASH_RCC_APB2Periph, ENABLE);
	GPIO_InitStructure.GPIO_Pin = FLASH_GPIO_PIN_CS;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(FLASH_GPIO_REG, &GPIO_InitStructure);

	FLASH_DESELECT();
	
	if (spiFlashReadID() == 0xEF14) {
		printf("ITSB: w25q16 initialization OK ! \n");
		
	} else {
		printf("ITSB: w25q16 initialization Failed ! \n");
	}
}


/**
  * @brief  Read the status registers of W25Q16
  * @note   BITS    9    8    7    6    5    4    3    2    1    0  
  * @note   QE SPR1 SPR0  SEC   TB  BP2  BP1  BP0  WEL  BUSY
  * @note   SPR:  默认0， 状态寄存器保护位，配合WP使用
  * @note   TB:   BP2,BP1,BP0:FLASH区域写保护设置
  * @note   WEL： 写使能锁定
  * @note   BUSY: 忙标记位(1: 忙; 0:空闲)
  */
static uint8_t spiFlashReadStatusReg(void) 
{  
	uint8_t byte = 0;   
	FLASH_SELECT();                           
	spiReadWriteByte(W25Q_ReadStatusReg);      
	byte = spiReadWriteByte(0Xff);              
	FLASH_DESELECT();                               
	return byte;   
} 


/**
  * @brief  write the status registers of W25Q16
  * @param  data:
  * @note   only SPR TB BP2 BP1 BP0 can be writen into
  */
static void spiFlashWriteStatusReg(uint8_t data) 
{
	FLASH_SELECT();                            
	spiReadWriteByte(W25Q_ReadStatusReg);     
	spiReadWriteByte(data);               
	FLASH_DESELECT();
}


/**
  * @brief  wait for not busy
  */
static void spiFlashWaitBusy(void) 
{   
	while((spiFlashReadStatusReg()&0x01)==0x01);
}


/**
  * @brief  spi write enable
  * @note   set WEL 
  */
static void spiFlashWriteEnable(void) 
{
	FLASH_SELECT();                            
    spiReadWriteByte(W25Q_WriteEnable);     
	FLASH_DESELECT();                          	      
} 



/**
  * @brief  spi write disable
  * @note   clr WEL 
  */
static void spiFlashWriteDisable(void) 
{
	FLASH_SELECT();                            
    spiReadWriteByte(W25Q_WriteDisable);     
	FLASH_DESELECT();                          	      
}


/**
  * @brief  read ID
  * @note   W25Q80		0xEF13
  * @note   W25Q16		0xEF14
  * @note   W25Q32		0xEF15
  * @note   W25Q64		0xEF16
  * @note   W25Q128		0xEF17
  */
uint16_t spiFlashReadID(void) 
{
	uint16_t temp = 0;	  
	FLASH_SELECT();				    
	spiReadWriteByte(W25Q_ManufactDeviceID); 
	spiReadWriteByte(0x00); 	    
	spiReadWriteByte(0x00); 	    
	spiReadWriteByte(0x00); 	 	
	temp |= spiReadWriteByte(0xFF)<<8;  
	temp |= spiReadWriteByte(0xFF);	 
	FLASH_DESELECT();				    
	return temp;
}


/**
  * @brief  read n bytes flash data from readaddr to pbuffer
  * @param  buffer  : address for hold data
  * @param  readAddr: address for read data
  * @param  n       : n bytes to read (max is 65535)
  */
void spiFlashRead(uint8_t *buffer, uint32_t readAddr, uint16_t n) 
{ 
 	uint16_t i;    												    
	FLASH_SELECT();                              
    spiReadWriteByte(W25Q_ReadData);            
    spiReadWriteByte((uint8_t)((readAddr)>>16));   
    spiReadWriteByte((uint8_t)((readAddr)>>8));   
    spiReadWriteByte((uint8_t)readAddr);   
    for (i = 0; i < n; i++) { 
        buffer[i]=spiReadWriteByte(0xFF);   
    }
	FLASH_DESELECT();                            	      
}


/**
  * @brief  write n bytes to flash (n must be less than 256)
  * @param  buffer   : address of data where it comes from
  * @param  writeAddr: address of date need to be writen to 
  * @param  n        : n bytes to be writen(less than 256, for a page's size is 256Bytes)
  */
void spiFlashWritePage(uint8_t *buffer, uint32_t writeAddr, uint16_t n) 
{
 	uint16_t i;  
    spiFlashWriteEnable();             
	FLASH_SELECT();                          
    spiReadWriteByte(W25Q_PageProgram);        
    spiReadWriteByte((u8)((writeAddr)>>16)); 
    spiReadWriteByte((u8)((writeAddr)>>8));   
    spiReadWriteByte((u8)writeAddr);   
    for (i = 0; i < n; i++) {
		spiReadWriteByte(buffer[i]);
	}
	FLASH_DESELECT();                            
	spiFlashWaitBusy();					   
} 



/**
  * @brief  erase the whole chip
  * @note   W25Q16: 25s
  * @note   W25Q32: 40s
  * @note   W25Q64: 40s
  */
void spiFlashEraseChip(void) 
{                                             
    spiFlashWriteEnable();                 
    spiFlashWaitBusy();   
  	FLASH_SELECT();                             
    spiReadWriteByte(W25Q_ChipErase);        
	FLASH_DESELECT();                              	      
	spiFlashWaitBusy();   				   
} 



/**
  * @brief  erase a sector
  * @param  addrSect: 0 ~ 511 for W25Q16  
  * @note   each sector is 4096 bytes 
  * @note   erase a sector needs at least 150ms
  */
void spiFlashEraseSector(uint32_t addrSect) 
{                                             
	addrSect *= 4096;
    spiFlashWriteEnable();
    spiFlashWaitBusy();   
  	FLASH_SELECT();                          
    spiReadWriteByte(W25Q_SectorErase);       
    spiReadWriteByte((uint8_t)((addrSect)>>16));  
    spiReadWriteByte((uint8_t)((addrSect)>>8));   
    spiReadWriteByte((uint8_t)addrSect);  
	FLASH_DESELECT();                            	      
    spiFlashWaitBusy();   				  
} 


/**
  * @brief  erase a 32K block
  * @param  addrBlock32: 0 ~ 31 for W25Q16 
  * @note   each block is 32K bytes
  */
void spiFlashEraseBlock32(uint32_t addrBlock32) 
{                                             
	addrBlock32 *= 32768;
    spiFlashWriteEnable();
    spiFlashWaitBusy();   
  	FLASH_SELECT();                          
    spiReadWriteByte(W25Q_BlockErase32K);       
    spiReadWriteByte((uint8_t)((addrBlock32)>>16));  
    spiReadWriteByte((uint8_t)((addrBlock32)>>8));   
    spiReadWriteByte((uint8_t)addrBlock32);  
	FLASH_DESELECT();                            	      
    spiFlashWaitBusy();   				  
} 


/**
  * @brief  erase a 64K block
  * @param  addrBlock64: 0 ~ 15 for W25Q16
  */
void spiFlashEraseBlock64(uint32_t addrBlock64) 
{                                             
	addrBlock64 *= 65536;
    spiFlashWriteEnable();
    spiFlashWaitBusy();   
  	FLASH_SELECT();                          
    spiReadWriteByte(W25Q_BlockErase64K);       
    spiReadWriteByte((uint8_t)((addrBlock64)>>16));  
    spiReadWriteByte((uint8_t)((addrBlock64)>>8));   
    spiReadWriteByte((uint8_t)addrBlock64);  
	FLASH_DESELECT();                            	      
    spiFlashWaitBusy();   				  
} 





























