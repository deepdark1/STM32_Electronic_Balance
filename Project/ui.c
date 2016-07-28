#include "GUIDEMO.h"
#include "FramewinDLG.h"
#include "led.h"
#include "malloc.h"
#include "function.h"
#include "mmc_sd.h"
#include "dac.h"
#include "adc.h"
#include "stdio.h"
#include "ui.h"
#include "timer.h"
#include "delay.h"
#include "function.h"


WM_HWIN hWinMain;


void UI_Main(){
	GUI_SetBkColor(GUI_WHITE);
	BUTTON_SetDefaultSkin(BUTTON_SKIN_FLEX);
	PROGBAR_SetDefaultSkin(PROGBAR_SKIN_FLEX);
	
	CreateFramewin();
	UIDisplayValue();
	while(1)
	{
		GUI_Delay(1);
	}
}

