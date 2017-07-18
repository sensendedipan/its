

#include "board.h"

#define  WatchDog_Time_MS  100

/**
  * @brief  watchdog init
  * @retval None
  */
void wdgInit(void)
{
	if (RCC_GetFlagStatus(RCC_FLAG_IWDGRST) != RESET) {
		RCC_ClearFlag();
	}
	
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
	IWDG_SetPrescaler(IWDG_Prescaler_256); //! 40000/256 = 156Hz (~6.4ms)
	
	IWDG_SetReload(WatchDog_Time_MS/6.4);  //! 100ms
	IWDG_ReloadCounter();
	
	IWDG_Enable();
	printf("ITSB: watchdog (time:%dms) initialization OK ! \n", WatchDog_Time_MS);
}



/**
  * @brief  feed dog
  * @retval None
  */
void wdgFeed(void)
{
	IWDG_ReloadCounter();
}


















