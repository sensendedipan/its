

#include "board.h"




/**
  * @brief  led init
  * @retval None
  */
void ledInit(void) 
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(LED_RCC_APB2Periph, ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Pin	= LED_GPIO_PIN;
	GPIO_Init(LED_GPIO_REG, &GPIO_InitStructure);
	
	GPIO_ResetBits(LED_GPIO_REG, LED_GPIO_PIN);
	
	printf("ITSB: led initialization OK ! \n");
}
	


void ledOn(void) 
{
	GPIO_SetBits(LED_GPIO_REG, LED_GPIO_PIN);
}



void ledOff(void) 
{
	GPIO_ResetBits(LED_GPIO_REG, LED_GPIO_PIN);
}



#ifndef NULL
    #define NULL  0
#endif 

typedef void (*ledStateFunc)(void);


static void ledIdleFunc(void);
static ledStateFunc sLedFlash_on   = ledIdleFunc;
static ledStateFunc sLedFlash_off  = ledIdleFunc;

static uint8_t  sChFlashFsmSta   = 2;             
static uint8_t  sChFlashCnt      = 0;
static uint16_t sHwFlashOnTime   = 0;
static uint16_t sHwFlashOffTime  = 0;



static void ledIdleFunc(void)
{
    /* do nothing */
}



static void ledFlashOn(void)
{
    (*sLedFlash_on)();
}



static void ledFlashOff(void)
{
    (*sLedFlash_off)();
}



void ledFlashInit(void)
{
    sLedFlash_on  = ledOn;
    sLedFlash_off = ledOff;

    ledFlashOff();
	
	printf("ITSB: ledFlash initialization OK ! \n");
}



void ledFlashSet(uint8_t chFlashCnt, uint16_t hwOnTime, uint16_t hwOffTime)
{
    sChFlashCnt     = chFlashCnt;
    sHwFlashOnTime  = hwOnTime;
    sHwFlashOffTime = hwOffTime;

    sChFlashFsmSta = 0;
}



void ledFlashLoop(void)
{
    static uint16_t sHwTime     = 0;    
    static uint8_t sChLedSta    = 0;              
    static uint8_t sChFlashMode = 0;             

    switch (sChFlashFsmSta)
    {
	case 0:
		if (0 == sChFlashCnt) {
			sChFlashMode = 1;
			
		} else {
			sChFlashMode = 0;
		}

		ledFlashOff();                         
		sChLedSta = 1;                             
		sChFlashFsmSta = 1;                       

		break;

	case 1:
		if (sChLedSta) {
			if (0 == sHwTime) {
				if ((0 == sChFlashCnt) && (0 == sChFlashMode)) {
					sChFlashFsmSta = 2;
					break;
				}

				if (sHwFlashOnTime) {
					ledFlashOn();
					sHwTime = sHwFlashOnTime;     
					sChLedSta = 0;                 
				}
				
			} else {
				sHwTime--;                         
			}
			
		} else {
			if (0 == sHwTime) {
				if (sHwFlashOffTime) {
					ledFlashOff();
					sHwTime = sHwFlashOffTime;
					sChLedSta = 1;

					if (!sChFlashMode) {
						if (sChFlashCnt) {
							sChFlashCnt--;
						
						} else {
							/* do nothing */
						}
					}
				}
				
			} else {
				sHwTime--;
			}
		}

		break;

	case 2:
		/* do nothing */
		break;

	default:
		break;
    }
}



