


#include "board.h"


static uint32_t timer2_1000ms = 0;

/**
  * @brief  init timer2  
  * @note   
  */
void timer2Init(uint16_t ms)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(TIMER2_RCC_APB1Periph, ENABLE);
    TIM_DeInit(TIMER2_TIMER);
	TIM_TimeBaseStructure.TIM_Period = 10*ms-1;		 							

	TIM_TimeBaseStructure.TIM_Prescaler = (7200-1);	// 10KHz			           
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 		
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 
	TIM_TimeBaseInit(TIMER2_TIMER, &TIM_TimeBaseStructure);
	TIM_ClearFlag(TIMER2_TIMER, TIM_FLAG_Update);							    		
	TIM_ITConfig(TIMER2_TIMER, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIMER2_TIMER, ENABLE);																		
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);	
	NVIC_InitStructure.NVIC_IRQChannel = TIMER2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	
	NVIC_Init(&NVIC_InitStructure);
	
	printf("ITSB: timer2 initialization at %d ms OK ! \n", ms);
}



/**
  * @brief  init timer3 
  * @note   
  */
void timer3Init(uint16_t ms)
{
	TIM_TimeBaseInitTypeDef   TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(TIMER3_RCC_APB1Periph, ENABLE);
    TIM_DeInit(TIMER3_TIMER);
	TIM_TimeBaseStructure.TIM_Period = 10*ms-1;  
	TIM_TimeBaseStructure.TIM_Prescaler = (7200-1);  // 10KHz  
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 

	TIM_TimeBaseInit(TIMER3_TIMER, &TIM_TimeBaseStructure);
	TIM_ITConfig(TIMER3_TIMER, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIMER3_TIMER, ENABLE);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);	
	NVIC_InitStructure.NVIC_IRQChannel = TIMER3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	
	NVIC_Init(&NVIC_InitStructure);	
	
	printf("ITSB: timer3 initialization at %d ms OK ! \n", ms);
}




void TIMER2_TIMER_IRQHandler()
{

	
	if (TIM_GetITStatus(TIMER2_TIMER, TIM_IT_Update) != RESET) {   
		TIM_ClearITPendingBit(TIMER2_TIMER, TIM_IT_Update);

		wdgFeed();
		ledFlashLoop();		
		
		if (timer2_1000ms++ > 1000) {
			timer2_1000ms = 0;
			
			timer2_1000ms_callback();

		}
	}

}


     
void TIMER3_TIMER_IRQHandler()
{
	if (TIM_GetITStatus(TIMER3_TIMER, TIM_IT_Update) != RESET) {   
		TIM_ClearITPendingBit(TIMER3_TIMER, TIM_IT_Update);
		
		gCanAskForData = !gCanAskForData;				
		gCheckNotesState = !gCheckNotesState;

	}
}



void timer2_1000ms_callback(void)
{
	static uint8_t temp_5000ms;
	
	if (temp_5000ms++ > MESH_TIME_MAX) {
		temp_5000ms = 0;
		
		gMeshFinished = true;
	}
	
				
}



















