


#include "board.h"
#include "itsb.h"


uint16_t g_500ms = 0;

#define DATA_POLL_FREQ   10 //! the frequence that master ask for data



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
	timer3Init(1000/DATA_POLL_FREQ);		
	wdgInit();
}

















