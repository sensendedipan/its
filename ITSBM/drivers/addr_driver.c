


#include "board.h"




void addrInit(void) 
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(ADDR_RCC_APB2Periph, ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	
	GPIO_InitStructure.GPIO_Pin	= ADDR_GPIOB_PINS;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin	= ADDR_GPIOC_PINS;
	GPIO_Init(GPIOC, &GPIO_InitStructure);	
	GPIO_InitStructure.GPIO_Pin	= ADDR_GPIOD_PINS;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	printf("ITSB: addr initialization OK ! \n");
}


uint8_t readAddr(void) 
{
	
	uint8_t addr = 0;
	addr = 	GPIO_ReadInputDataBit(GPIOB, ADDR_GPIO_PIN_0) << 0 |
			GPIO_ReadInputDataBit(GPIOB, ADDR_GPIO_PIN_1) << 1 |
			GPIO_ReadInputDataBit(GPIOB, ADDR_GPIO_PIN_2) << 2 |
			GPIO_ReadInputDataBit(GPIOD, ADDR_GPIO_PIN_3) << 3 |
			GPIO_ReadInputDataBit(GPIOC, ADDR_GPIO_PIN_4) << 4 |
			GPIO_ReadInputDataBit(GPIOC, ADDR_GPIO_PIN_5) << 5 |
			GPIO_ReadInputDataBit(GPIOC, ADDR_GPIO_PIN_6) << 6 |
			GPIO_ReadInputDataBit(GPIOC, ADDR_GPIO_PIN_7) << 7 ;
	return addr;
}
