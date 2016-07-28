#include "stdlib.h"
#include "led.h"
#include "usart.h"
#include "adc.h"
//#include "dac.h"
//#include "mmc_sd.h"
#include "ui.h"
#include "content.h"
#include "function.h"

#define ADC_TIMES 30
extern double curWeight;
extern double revWeight;
extern double oriWeight;



int sumtmp = 0;

void TimerContent(void){
	//int i;
	u16 adcx;
	sumtmp++;
	if(sumtmp == 20){
		adcx = Get_Adc_Average(ADC_CH1, ADC_TIMES);
		curWeight = ( (double) (adcx) / (double) 4096 ) * (double)3.3;
		oriWeight = curWeight / (double) 0.004512;
		curWeight = oriWeight < revWeight ? 0 : oriWeight - revWeight;
		UIDisplayValue();
		sumtmp = 0;
	}
}
