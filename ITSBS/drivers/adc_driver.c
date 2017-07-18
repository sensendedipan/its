


#include "board.h"


__IO uint16_t Analog_values[NUMBER_ANALOG_ADC1];


void adcInit(void) 
{

	GPIO_InitTypeDef GPIO_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;	
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE); // enable dma clock
	RCC_APB2PeriphClockCmd(ADC_RCC_APB2Periph | ADC_RCC_APB2Periph_ADC1, ENABLE); // enable adc1 and gpio clock

	GPIO_InitStructure.GPIO_Pin = ADC_GPIOA_PINS;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
	
	GPIO_InitStructure.GPIO_Pin = ADC_GPIOC_PINS;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOC, &GPIO_InitStructure);		
		
	DMA_DeInit(DMA1_Channel1); // channel 1
	DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address; // set adc1 periphera base addr
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&Analog_values; 
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize = NUMBER_ANALOG_ADC1;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;	
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;		
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);

	DMA_Cmd(DMA1_Channel1, ENABLE);

	ADC_DeInit(ADC1);
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	
	ADC_InitStructure.ADC_ScanConvMode = ENABLE ; 
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; 	
	ADC_InitStructure.ADC_NbrOfChannel = NUMBER_ANALOG_ADC1;	 	
	ADC_Init(ADC1, &ADC_InitStructure);
	
	ADC_RegularChannelConfig(ADC1, ADC_Channel_6,  1, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_7,  2, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_4,  3, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_5,  4, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 5, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_11, 6, ADC_SampleTime_55Cycles5);

	ADC_DMACmd(ADC1, ENABLE);

	ADC_Cmd(ADC1, ENABLE);
  
	ADC_ResetCalibration(ADC1);
	while (ADC_GetResetCalibrationStatus(ADC1));

	ADC_StartCalibration(ADC1);
	while (ADC_GetCalibrationStatus(ADC1)); // check whether calib ok

	ADC_SoftwareStartConvCmd(ADC1, ENABLE);	
	
	printf("ITSB: adc initialization OK ! \n");
	
}


uint16_t getAnalogValue(uint8_t index) 
{

	if (index >= NUMBER_ANALOG_ADC1) return 0;

	if (Analog_values[index] < 0) {
		return 4096 - Analog_values[index];
		
	} else {
		
		return Analog_values[index];		
	}

}
