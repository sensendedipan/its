


#include "board.h"



/**
  * @brief  usart2 init
  * @param	baud
  * @retval None
  */
void usart2Init(uint16_t baud) 
{
	GPIO_InitTypeDef   GPIO_InitStructure;
	USART_InitTypeDef  USART_InitStructure;	
	NVIC_InitTypeDef 	NVIC_InitStructure;
	
	USART_DeInit(USART2_USART);
	RCC_APB2PeriphClockCmd(USART2_RCC_APB2Periph, ENABLE);
	RCC_APB1PeriphClockCmd(USART2_RCC_APB1Periph_USART2, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = USART2_GPIO_PIN_TX;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_Init(USART2_GPIO, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = USART2_GPIO_PIN_RX;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(USART2_GPIO, &GPIO_InitStructure);
	
	USART_InitStructure.USART_BaudRate = baud;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART2_USART, &USART_InitStructure);
    USART_GetFlagStatus(USART2_USART, USART_FLAG_TC); 
	USART_ITConfig(USART2_USART, USART_IT_RXNE, ENABLE);
	USART_Cmd(USART2_USART, ENABLE);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	NVIC_InitStructure.NVIC_IRQChannel = USART2_USART_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 4;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	printf("ITSB: usart2 (baud:%d) initialization OK ! \n", baud);
}
 


void usart2Stop(void) 
{
	USART_DeInit(USART2_USART);	
}



void usart2SendChar(uint8_t data) 
{  
	
    USART_SendData(USART2_USART, data);  
    while (USART_GetFlagStatus(USART2_USART, USART_FLAG_TC) == RESET){}  
} 



void usart2SendBuffer(uint8_t *buffer, uint16_t count) 
{
	
	while (count--) {    
		usart2SendChar(*buffer);    
		buffer++;    
	}
}


void USART2_USART_IRQHandler() 
{
	
	uint8_t data;

	if (USART_GetITStatus(USART2_USART, USART_IT_RXNE) != RESET) { 
		data = USART_ReceiveData(USART2_USART);

	}
}


/**
  * @brief  use usrt2 as printf() 
  * @note
  */
int fputc(int ch, FILE *f)
{
    USART_SendData(USART2_USART, (unsigned char) ch);  
    while (USART_GetFlagStatus(USART2_USART, USART_FLAG_TC) == RESET){} 
	return (ch);
} 

 

 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
