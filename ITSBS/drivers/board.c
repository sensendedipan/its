


#include "board.h"




/**
  * @brief  board init
  * @retval None
  */
void boardInit(void)
{
	delayInit();
	usart2Init(57600);
	
	addrInit();
	ledInit();
	ledFlashInit();
	radioInit();
	i2cInit();
	i2cEepromInit();
	spiInit();
	spiFlashInit();
	adcInit();
	usart1Init(57600);

	timer2Init(1);
	timer3Init(200);		
	wdgInit();
	
	while (!getCpuId(&myNode.mac[0], &myNode.mac[1], &myNode.mac[2])) { //! if get mac failed, then continue to get till succeed!
		ledFlashSet(1, 1000, 10);
	}
	
	ledFlashSet(1, 1000, 10);
}

















