#include "stdio.h"
#include "function.h"
#include "string.h"
#include "DIALOG.h"

#define MAX_INPUT_LENGTH 10
#define TEXT_WEIGHCONTAINER "Please weigh the container."

//#define POS_WEIGHT 8
//#define POS_PRICE 7
//#define POS_TOTAL 7
#define POS_WEIGHT 9
#define POS_PRICE 10
#define POS_TOTAL 11
#define POS_CURRENT 9
#define POS_REDUCED 9

#define STA_NOTSET 0
#define STA_WAITING 1
#define STA_SET 2

#define ID_TEXT_0    (GUI_ID_USER + 0x03)
#define ID_TEXT_1    (GUI_ID_USER + 0x04)
#define ID_TEXT_2    (GUI_ID_USER + 0x06)
#define ID_TEXT_3    (GUI_ID_USER + 0x08)
#define ID_TEXT_4    (GUI_ID_USER + 0x0A)
#define ID_CHECKBOX_0    (GUI_ID_USER + 0x07)

extern WM_HWIN hWinMain;

double curWeight;
double redWeight;
double revWeight;
double oriWeight;
double curPrice;
double sumPrice;
double totPrice;


char strWeight[32] = "Weight:  "; // Position: 8
char strPrice[32] = "Price:    "; //   Position: 7
char strCurrent[32] = "Current: ";//Position: 9
char strTotal[32] = "Total:     "; //   Position: 7
char strReduced[32] = "Reduced: ";//Position: 9
char strUnit[3] = " g";

/* 
Status: 
	0 - Not set
	1 - Waiting
	2 - Set 
*/
int priceStatus;
int netwtStatus; 

char inputPrice[32];
int inputLength;
int hasDot;

void ShowInputPrice(WM_HWIN hItem){
	if (inputLength == 0){
		EDIT_SetText(hItem, "0");
		return;
	}
	EDIT_SetText(hItem, inputPrice);
}

void InputInit(void){
	memset(inputPrice, 0, sizeof(inputPrice));
	inputLength = 0;
	hasDot = 0;
}

void EditAddKey(char key){
	if ( inputLength == MAX_INPUT_LENGTH ){
		return;
	}
	if ( inputLength == 0 && key == '0'){
		return;
	}
	if ( hasDot == 1 && key == '.'){
		return;
	}
	if ( inputLength == 0 && key == '.'){
		inputPrice[ inputLength++ ] = '0';
	}
	if ( key == '.'){
		hasDot = 1;
	}
	inputPrice[ inputLength++ ] = key;
}

void EditDelKey(){
	if (inputLength == 0){
		return;
	}
	if(inputPrice[ --inputLength ] == '.'){
		hasDot = 0;
	}
	inputPrice[inputLength] = 0;
}

void StringToDouble(){
	int dotted = 0;
	double value = 0;
	double power = 10;
	int i = 0;
	for (i = 0; i < inputLength; ++i){
		if ( inputPrice[i] == '.' ){
			dotted = 1;
			continue;
		}
		if ( !dotted ){
			value = value * (double)10 + (double)(inputPrice[i] - '0');
		}
		else{
			value = value + ((double)(inputPrice[i] - '0')) / (double) power;
			power *= 10;
		}
	}
	//printf("%f\n", value);
	curPrice = value;
}

void SystemReset(){
	curWeight = 0;
	redWeight = 0;
	revWeight = 0;
	oriWeight = 0;
	curPrice  = 0;
	sumPrice  = 0;
	totPrice  = 0;

	priceStatus = STA_NOTSET;
	netwtStatus = STA_NOTSET;

	strWeight[ POS_WEIGHT ]    = 0;
	strPrice [ POS_PRICE ]     = 0;
	strCurrent [ POS_CURRENT ] = 0;
	strTotal [ POS_TOTAL ]     = 0;
	strReduced [ POS_REDUCED ] = 0;

	CHECKBOX_SetState( WM_GetDialogItem(hWinMain, ID_CHECKBOX_0) , 0);
}

void PrintString(char *str, double value, int pos){
	if (value < 0.01 && value > - 0.01){
		value = 0;
	}
	sprintf(str + pos, "%.1lf", value);
}

void UIDisplayValue(){
	WM_HWIN hItem;

	if(priceStatus == STA_WAITING){
		return;
	}
	//Update Weight Value
	if (netwtStatus == STA_SET){
		PrintString(strWeight, curWeight < redWeight ? 0 : curWeight - redWeight, POS_WEIGHT);
	}
	else {
		PrintString(strWeight, curWeight, POS_WEIGHT);
	}
	strcpy(strWeight + strlen(strWeight), strUnit);
	hItem = WM_GetDialogItem(hWinMain, ID_TEXT_0);
	TEXT_SetText(hItem, strWeight);

	//Update Price Value
	PrintString(strPrice, curPrice, POS_PRICE);
	hItem = WM_GetDialogItem(hWinMain, ID_TEXT_1);
	TEXT_SetText(hItem, strPrice);

	//Update Current Value
	if (netwtStatus == STA_WAITING){
		sumPrice = 0;
	}
	else if (netwtStatus == STA_SET){
		sumPrice = curWeight < redWeight ? 0 : (curWeight - redWeight) * curPrice;
	}
	else{
		sumPrice = curWeight * curPrice;
	}
	PrintString(strCurrent, sumPrice, POS_CURRENT);
	hItem = WM_GetDialogItem(hWinMain, ID_TEXT_4);
	TEXT_SetText(hItem, strCurrent);

	//Update Total Value
	PrintString(strTotal, totPrice, POS_TOTAL);
	hItem = WM_GetDialogItem(hWinMain, ID_TEXT_3);
	TEXT_SetText(hItem, strTotal);

	//Update Reduced Price
	if ( netwtStatus == STA_NOTSET ){
		hItem = WM_GetDialogItem(hWinMain, ID_TEXT_2);
		TEXT_SetText(hItem, "");
	}
	else if ( netwtStatus == STA_WAITING ){
		hItem = WM_GetDialogItem(hWinMain, ID_TEXT_2);
		TEXT_SetText(hItem, TEXT_WEIGHCONTAINER);
	}
	else if ( netwtStatus == STA_SET ){
		PrintString(strReduced, redWeight, POS_REDUCED);
		strcpy(strReduced + strlen(strReduced), strUnit);
		hItem = WM_GetDialogItem(hWinMain, ID_TEXT_2);
		TEXT_SetText(hItem, strReduced);
	}
}

void EnterPrice(){
	if ( netwtStatus == STA_WAITING ){
		redWeight = curWeight;
		netwtStatus = STA_SET;
		return;
	}
	totPrice += sumPrice;
}

void ReviseWeight(){
	revWeight = oriWeight;
}
