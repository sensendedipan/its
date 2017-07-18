
#ifndef _HAL_H
#define _HAL_H

#include <stm32f10x.h>
#include <stm32f10x_flash.h>
#include <stdbool.h>
#include <stdio.h>


/****************************************************************/
//! addr

#define ADDR_GPIO_REG_0			GPIOB->IDR
#define ADDR_GPIO_PIN_0			GPIO_Pin_5	//! PB.05

#define ADDR_GPIO_REG_1			GPIOB->IDR
#define ADDR_GPIO_PIN_1			GPIO_Pin_4	//! PB.04

#define ADDR_GPIO_REG_2			GPIOB->IDR
#define ADDR_GPIO_PIN_2			GPIO_Pin_3	//! PB.03

#define ADDR_GPIO_REG_3			GPIOD->IDR
#define ADDR_GPIO_PIN_3			GPIO_Pin_2	//! PD.02

#define ADDR_GPIO_REG_4			GPIOC->IDR
#define ADDR_GPIO_PIN_4			GPIO_Pin_9	//! PC.09

#define ADDR_GPIO_REG_5			GPIOC->IDR
#define ADDR_GPIO_PIN_5			GPIO_Pin_8	//! PC.08

#define ADDR_GPIO_REG_6			GPIOC->IDR
#define ADDR_GPIO_PIN_6			GPIO_Pin_7	//! PC.07

#define ADDR_GPIO_REG_7			GPIOC->IDR
#define ADDR_GPIO_PIN_7			GPIO_Pin_6	//! PC.06

#define ADDR_RCC_APB2Periph 	(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD)
#define ADDR_GPIOB_PINS			(ADDR_GPIO_PIN_0 | ADDR_GPIO_PIN_1 | ADDR_GPIO_PIN_2)
#define ADDR_GPIOC_PINS			(ADDR_GPIO_PIN_4 | ADDR_GPIO_PIN_5 | ADDR_GPIO_PIN_6 | ADDR_GPIO_PIN_7)
#define ADDR_GPIOD_PINS			(ADDR_GPIO_PIN_3)



/****************************************************************/
//! can/433 mode

#define MODE_GPIO_REG 			GPIOB->IDR
#define MODE_GPIO_PIN			GPIO_Pin_11		//! PB.11	

#define MODE_RCC_APB2Periph		RCC_APB2Periph_GPIOB



/****************************************************************/
//! radio
#define RADIO_GPIO_REG_M0		GPIOC
#define RADIO_GPIO_PIN_M0		GPIO_Pin_11		//! PC.11

#define RADIO_GPIO_REG_M1		GPIOC
#define RADIO_GPIO_PIN_M1		GPIO_Pin_10		//! PC.10

#define RADIO_GPIO_REG_AUX		GPIOA->IDR
#define RADIO_GPIO_PIN_AUX		GPIO_Pin_8		//! PA.08

#define RADIO_RCC_APB2Periph	(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC)
#define RADIO_GPIOC_PINS		RADIO_GPIO_PIN_M0 | RADIO_GPIO_PIN_M1
#define RADIO_GPIOA_PINS		RADIO_GPIO_PIN_AUX





/****************************************************************/
//! net state indicator lamp

#define LED_GPIO_REG 			GPIOB
#define LED_GPIO_PIN			GPIO_Pin_10		//! PB.10	

#define LED_RCC_APB2Periph		RCC_APB2Periph_GPIOB



/****************************************************************/
//! iic/eeprom(AT24C256)

#define I2C1_RCC_APB2Periph			RCC_APB2Periph_GPIOB
#define I2C1_RCC_APB1Periph_I2C1	RCC_APB1Periph_I2C1
#define I2C1_GPIO					GPIOB
#define	I2C1_GPIO_PIN_SCL			GPIO_Pin_6		//! PB.06
#define	I2C1_GPIO_PIN_SDA			GPIO_Pin_7		//! PB.07
#define	I2C1_GPIO_AF				GPIO_AF_I2C1
#define	I2C1_GPIO_PinSource_SCL		GPIO_PinSource6
#define I2C1_GPIO_PinSource_SDA		GPIO_PinSource7
#define I2C1_SPEED					400000




/****************************************************************/
//! spi/flash(W25Q16)

#define SPI2_RCC_APB2Periph				RCC_APB2Periph_GPIOB
#define SPI2_RCC_APB1Periph_SPI2		RCC_APB1Periph_SPI2
#define	SPI2_GPIO						GPIOB
#define	SPI2_GPIO_PIN_SCK				GPIO_Pin_13	//! PB.13
#define	SPI2_GPIO_PIN_MISO				GPIO_Pin_14	//! PB.14
#define SPI2_GPIO_PIN_MOSI				GPIO_Pin_15	//! PB.15
#define SPI2_GPIO_PinSource_SCK			GPIOB_PinSource13
#define	SPI2_GPIO_PinSource_MISO		GPIOB_PinSource14
#define	SPI2_GPIO_PinSource_MOSI		GPIOB_PinSource15
#define	SPI2_GPIO_BaudRatePrescaler		SPI_BaudRatePrescaler_256 

#define FLASH_GPIO_REG					GPIOB
#define	FLASH_GPIO_PIN_CS				GPIO_Pin_12		//! PB.12
#define FLASH_RCC_APB2Periph			RCC_APB2Periph_GPIOB


/****************************************************************/
//! adc

#define	ADC_RCC_APB2Periph			(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC)
#define	ADC_RCC_APB2Periph_ADC1		 RCC_APB2Periph_ADC1
#define	ADC_GPIO_PIN_V_R			 GPIO_Pin_6		//! PA.06
#define	ADC_GPIO_PIN_C_R			 GPIO_Pin_7		//! PA.07
#define	ADC_GPIO_PIN_V_Y			 GPIO_Pin_4		//! PA.04
#define	ADC_GPIO_PIN_C_Y			 GPIO_Pin_5		//! PA.05
#define	ADC_GPIO_PIN_V_G			 GPIO_Pin_0		//! PC.00
#define	ADC_GPIO_PIN_C_G			 GPIO_Pin_1		//! PC.01
#define	ADC_CHANNEL_V_R				 ADC_Channel_6	//! ADC1_IN6
#define ADC_CHANNEL_C_R				 ADC_Channel_7	//! ADC1_IN7
#define	ADC_CHANNEL_V_Y				 ADC_Channel_4	//! ADC1_IN4
#define ADC_CHANNEL_C_Y				 ADC_Channel_5	//! ADC1_IN5
#define	ADC_CHANNEL_V_G				 ADC_Channel_10	//! ADC1_IN10
#define ADC_CHANNEL_C_G				 ADC_Channel_11	//! ADC1_IN11
#define ADC_GPIOA_PINS				(ADC_GPIO_PIN_V_R | ADC_GPIO_PIN_C_R | ADC_GPIO_PIN_V_Y | ADC_GPIO_PIN_C_Y)
#define ADC_GPIOC_PINS				(ADC_GPIO_PIN_V_G | ADC_GPIO_PIN_C_G)

#define NUMBER_ANALOG_ADC1			6
#define ADC1_DR_Address				((uint32_t)0x40012400+0x4c)

/****************************************************************/
//! usart1/radio

#define USART1_RCC_APB2Periph			RCC_APB2Periph_GPIOA
#define	USART1_RCC_APB2Periph_USART1	RCC_APB2Periph_USART1
#define	USART1_GPIO						GPIOA
#define	USART1_GPIO_PIN_TX				GPIO_Pin_9	//! PA.09
#define	USART1_GPIO_PIN_RX				GPIO_Pin_10 //! PA.10
#define	USART1_GPIO_PinSource_TX		GPIO_PinSource9
#define USART1_GPIO_PinSource_RX		GPIO_PinSource10
#define	USART1_GPIO_AF					GPIO_AF_USART1
#define USART1_USART					USART1
#define USART1_USART_IRQHandler			USART1_IRQHandler
#define	USART1_USART_IRQn				USART1_IRQn

#define RADIO_GPIO_REG_AUX				GPIOA->IDR
#define RADIO_GPIO_PIN_AUX				GPIO_Pin_8	//! PA.08

#define RADIO_GPIO_REG_M0				GPIOC
#define RADIO_GPIO_PIN_M0				GPIO_Pin_11 //! PC.11

#define RADIO_GPIO_REG_M1				GPIOC
#define RADIO_GPIO_PIN_M1				GPIO_Pin_10 //! PC.10

#define RADIO_RCC_APB2Periph			(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC)



/****************************************************************/
//! usart2

#define USART2_RCC_APB2Periph			RCC_APB2Periph_GPIOA
#define	USART2_RCC_APB1Periph_USART2	RCC_APB1Periph_USART2
#define	USART2_GPIO						GPIOA
#define	USART2_GPIO_PIN_TX				GPIO_Pin_2	//! PA.02
#define	USART2_GPIO_PIN_RX				GPIO_Pin_3  //! PA.03
#define	USART2_GPIO_PinSource_TX		GPIO_PinSource2
#define USART2_GPIO_PinSource_RX		GPIO_PinSource3
#define	USART2_GPIO_AF					GPIO_AF_USART2
#define USART2_USART					USART2
#define USART2_USART_IRQHandler			USART2_IRQHandler
#define	USART2_USART_IRQn				USART2_IRQn



/****************************************************************/
//! timer2/1KHz timer
#define TIMER2_RCC_APB1Periph			RCC_APB1Periph_TIM2
#define TIMER2_TIMER					TIM2
#define TIMER2_IRQn              		TIM2_IRQn
#define TIMER2_TIMER_IRQHandler       	TIM2_IRQHandler

/****************************************************************/
//! timer3/1KHz timer
#define TIMER3_RCC_APB1Periph			RCC_APB1Periph_TIM3
#define TIMER3_TIMER					TIM3
#define TIMER3_IRQn              		TIM3_IRQn
#define TIMER3_TIMER_IRQHandler       	TIM3_IRQHandler







#endif









