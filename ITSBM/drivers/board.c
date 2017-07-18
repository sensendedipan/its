


#include "board.h"
#include "itsb.h"


uint16_t g_500ms = 0;



 

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
	timer3Init(TRAVERSE_PERIOD/2);		
	wdgInit();
	
	while (!getCpuId(&node[0].mac[0], &node[0].mac[1], &node[0].mac[2])) {
		ledFlashSet(1, 1000, 10);
	}
	
    ledFlashSet(1, 1000, 10);
}

















