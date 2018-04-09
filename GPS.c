#include "Driver_USART.h"               // ::CMSIS Driver:USART
#include "Board_GLCD.h"                 // ::Board Support:Graphic LCD
#include "GLCD_Config.h"                // Keil.MCB1700::Board Support:Graphic LCD
#include "cmsis_os.h"                   // ARM::CMSIS:RTOS:Keil RTX
#include "osObjects.h"
#include "stdio.h"
#include "stdlib.h"

extern ARM_DRIVER_USART Driver_USART1;
extern GLCD_FONT GLCD_Font_6x8;
extern GLCD_FONT GLCD_Font_16x24;
osThreadId ID_Tache1;
osThreadId ID_Tache2;
char tab[80];

int dif(int x,int y)
{
	int result;
	if (x>y)
	{result=x-y;}
	else
	{result=y-x;}
	return result;
}

int filtreLat(int x)
{
	static int a[5];
	static int result;
	if(a[4]==0)
	{
	a[0]=x;
	return a[0];
	}
	else{
	a[0]=x;
	if((dif(a[0],a[1]))<50)
	{
	result=(a[0]+a[1]+a[2]+a[3]+a[4])/5;
	a[4]=a[3];
	a[3]=a[2];
	a[2]=a[1];
	a[1]=a[0];
	}
	else {a[0] = a[1];}
	
	return result;
	}
}

int filtreLon(int x)
{
	static int e[5];
	static int result;
	if(e[4]==0)
	{
	e[0]=x;
	return e[0];
	}
	else{
	e[0]=x;
	if((dif(e[0],e[1]))<50)
	{
	result=(e[0]+e[1]+e[2]+e[3]+e[4])/5;
	e[4]=e[3];
	e[3]=e[2];
	e[2]=e[1];
	e[1]=e[0];
	}
	
	return result;
	}
}

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
		
		if(x>10)
		{
		GLCD_SetForegroundColor(GLCD_COLOR_RED);
		for(i=0;i<3;i++){
		GLCD_DrawHLine(x-1,y-1+i,3);}
//		GLCD_DrawRectangle(x-2, y-2, 4, 4);
		}
}

void eff(int x,int y){
		char i;
		if(x>10){
		GLCD_SetForegroundColor(GLCD_COLOR_RED);
		for(i=0;i<4;i++){
				GLCD_DrawHLine(x-2,y-2+i,4);}}
}

void tra(int x,int y){
		char i;
		if(x>10){
		GLCD_SetForegroundColor(GLCD_COLOR_BLUE);
		for(i=0;i<4;i++){
				GLCD_DrawHLine(x-2,y-2+i,4);}}
}

void decale(char * string,int x){
	int i;
	for(i=0;i<x;i++)
	{*(string+i)='0';}
}

void efface(char *tab)
{
	static char i;
	for(i=0;i<80;i++){
	*(tab+i)=0;
	}
}

void event_UART(uint32_t event)
{
	switch (event) {
		
		case ARM_USART_EVENT_RECEIVE_COMPLETE : 	osSignalSet(ID_Tache1, 0x01);
																							break;
		
		case ARM_USART_EVENT_SEND_COMPLETE  : 	osSignalSet(ID_Tache2, 0x01);
																							break;
		
		default : break;
	}
}

void Tache1(void const *argument)
{
	char let[1];
	char j;
	while(1)
	{
		Driver_USART1.Receive(let,1);
		osSignalWait(0x01,osWaitForever);
		if(let[0]==10)		//Si on recoit un CR 
		{
			j=0;
			efface(tab);		//efface buffer
		}
		else 
		{
			tab[j]=let[0];	//remplir le buffer
							 j++;
		}
}}
	
void Tache2(void const *argument)
{
char i,j,k,l,m;
char speed[10],course[10],lon[10],lat[10],text[20];
unsigned int x,y;
int lati,loni,ligne;
while(1)
	{
		if(tab[3]=='R'){
		for(i=0;i<80;i++)
		{
			if(tab[i]==','){j++;}
			if(j==7)
			{	
				if(tab[i+1]!=','){
				speed[k]=tab[i+1];
				k++;}
			}
//			else if (j==8)
//			{
//				if(tab[i+1]!=','){
//				course[l]=tab[i+1];
//				l++;}
//			}
				else if (j==3)
			{
				if((tab[i+1]!=',')&&(tab[i+1]!='.'))
				{
				lat[l]=tab[i+1];
				l++;
				}
			}
				else if (j==5)
			{
				if((tab[i+1]!=',')&&(tab[i+1]!='.'))
				{
				lon[m]=tab[i+1];
				m++;
				}
			}
		}
		eff(x,y);
		decale(lat,4);
		decale(lon,5);
		lati=atoi(lat);
		loni=atoi(lon);
		lati=filtreLat(lati);
		loni=filtreLon(loni);
		lati=1.7124*(double)lati+3214;
		loni=1.6625*(double)loni-3318;
//		sprintf(text,"%6d %6d",lati,loni);
//		GLCD_DrawString(1,70,text);
		x=-(double)(lati-8460)*0.0241+(double)(loni-7750)*0.079+55;
		y=-(double)(lati-8460)*0.1505-(double)(loni-7750)*0.0225+15;
//		x=1.2*(double)x-12;
		if(y>235){y=235;}
		else if(y<5){y=5;}
//		position(x,y);
		tra(x,y);
//		sprintf(text,"%d %d",x,y);
//		GLCD_DrawString(1,30*ligne+10,text);
//		ligne++;
//		if(ligne==8){ligne=0;}
		}
		
		j=0;k=0;l=0;m=0;
//		GLCD_DrawString(1,10,speed);
//		GLCD_DrawString(1,40,lon);
//		vit=1.852*atof(speed);
//		ang=atof(course);
//		sprintf(text,"Vitesse : %3.1f km/h",vit);
//		GLCD_DrawString(1,70,text);
//		position(80,170);
//		osDelay(800);
//		traj(80,170);
//		osDelay(500);
	}
}

void Init_UART(void){
	Driver_USART1.Initialize(event_UART);
	Driver_USART1.PowerControl(ARM_POWER_FULL);
	Driver_USART1.Control(	ARM_USART_MODE_ASYNCHRONOUS |
							ARM_USART_DATA_BITS_8		|
							ARM_USART_STOP_BITS_1		|
							ARM_USART_PARITY_NONE		|
							ARM_USART_FLOW_CONTROL_NONE,
							9600);
	Driver_USART1.Control(ARM_USART_CONTROL_TX,1);
	Driver_USART1.Control(ARM_USART_CONTROL_RX,1);
}

osThreadDef(Tache1,osPriorityNormal,1,0);
osThreadDef(Tache2,osPriorityNormal,1,0);

int main (void){
	osKernelInitialize ();
	GLCD_Initialize();
	GLCD_ClearScreen();
	GLCD_SetFont(&GLCD_Font_6x8);
	Init_UART();
	DrawMap();
	
	ID_Tache1 = osThreadCreate (osThread(Tache1), NULL);
	ID_Tache2 = osThreadCreate (osThread(Tache2), NULL);

  osKernelStart ();  	// start thread execution 
	osDelay(osWaitForever);
	return 0;
}
