#include "Driver_USART.h"               // ::CMSIS Driver:USART
#include "Board_GLCD.h"                 // ::Board Support:Graphic LCD
#include "GLCD_Config.h"                // Keil.MCB1700::Board Support:Graphic LCD
#include "stdio.h"
#include "stdlib.h"

extern ARM_DRIVER_USART Driver_USART1;
extern GLCD_FONT GLCD_Font_6x8;
extern GLCD_FONT GLCD_Font_16x24;

void DrawMap(void)
{
	GLCD_DrawRectangle(241,15,13,18);	//B1
	GLCD_DrawRectangle(241,33,10,54);	//R
	GLCD_DrawRectangle(175,46,90,17);	//B2
	GLCD_DrawRectangle(172,63,16,45);	//A
	GLCD_DrawRectangle(175,108,16,5);	//jianshenfang
	GLCD_DrawRectangle(238,87,23,60);	//C
	GLCD_DrawRectangle(127,133,97,24);	//E
	GLCD_DrawRectangle(127,181,97,45);	//GMP
	GLCD_DrawRectangle(55,135,34,92);		//G
	GLCD_DrawRectangle(77,20,34,92);	//H
	GLCD_DrawChar(247,113,'C');
	GLCD_DrawChar(217,51,'B');
	GLCD_DrawChar(178,81,'A');
	GLCD_DrawChar(172,141,'E');
	GLCD_DrawChar(69,179,'G');
	GLCD_DrawChar(92,64,'H');
	GLCD_DrawChar(172,200,'F');
}

void position(int x,int y){
		char i;
		GLCD_SetForegroundColor(GLCD_COLOR_RED);
		for(i=0;i<6;i++){
				GLCD_DrawHLine(x-3,y-3+i,6);}
}

void traj(int x,int y){
		char i;
		GLCD_SetForegroundColor(GLCD_COLOR_WHITE);
		for(i=0;i<6;i++){
				GLCD_DrawHLine(x-3,y-3+i,6);}
}



  int main(void)

	{
		GLCD_Initialize();
	  GLCD_ClearScreen();
	  GLCD_SetFont(&GLCD_Font_6x8);
		DrawMap();
		while(1)
		{
		int i;
		position(80,170);
    for(i=0;i<8667777;i++);
		traj(80,170);
    for(i=0;i<6667777;i++);
		}
	}
	