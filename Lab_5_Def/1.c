#include "main.h"
#include "stdio.h"
#define  STM32F40_41xxx

uint16_t delay_c = 0; 
uint8_t ID = 0;
int16_t x_dec,y_dec,z_dec,x_x,y_y;
uint8_t LCD_Memory[LCD_CACHE_SIZE];
short i;

void DrawMyLine(int x1, int y1, int x2, int y2, int mode);

//system tick handler
void SysTick_Handler(void){
	if(delay_c > 0)
		delay_c--;
}

//delay (ms)
void Delay_ms(uint16_t delay_t)
{
	delay_c = delay_t;
	while(delay_c){};
}

void tooglePixel(uint8_t x, uint8_t y, uint8_t isOn) {
	if(isOn == 0)
		Set_Pixel(x, y, PIXEL_OFF);
	else
		Set_Pixel(x, y, PIXEL_ON);
}


void drawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint8_t thickness) {
	for(int i = 0; i < thickness; i++) {
		uint16_t tx1 = x1 + i;
		uint16_t ty1 = y1 + i;
		uint16_t tx2 = x2 - i;
		uint16_t ty2 = y2 - i;
		Draw_Line(tx1, ty1, tx1, ty2);
		Draw_Line(tx2, ty1, tx2, ty2);
		Draw_Line(tx1, ty1, tx2, ty1);
		Draw_Line(tx1, ty2, tx2, ty2);
	}
}
//main function
int main(void)
{
	char ScrStr[20];
	LIS3DSH_t Axes_Data;
	SysTick_Config(SystemCoreClock/1000);
	LCD_init();
	LCD5110_set_XY(3,0);
	LCD5110_write_string("Loading");
	Delay_ms(1000);
	//Lcd3310_Pict(lcd_image_mas_iver);
	
//	ID = Read_Val_Reg(LIS3DSH_WHO_AM_I_ADDR);
//	InitLIS3DSH(TM_LIS3DSH_Sensitivity_2G,TM_LIS3DSH_Filter_50Hz);
//	Delay_ms(20);
//	ID = Read_Val_Reg(0x0C);
	
	
	
  while(1)
	{		
			drawRectangle(5, 10, 20, 30, 4);
		
		//DrawMyLine(5, 5, 35, 35, 0)
		
//			ReadAxes(&Axes_Data);
//			x_dec = Axes_Data.X/11;
//			y_dec = Axes_Data.Y/11;
//			z_dec = Axes_Data.Z/11;
//			//Draw_Circle(41,23,23,PIXEL_ON);
//			//Draw_Circle(41,23,16,PIXEL_ON);
//			//Draw_Circle(41,23,9,PIXEL_ON);
//			
//			//Draw_Line(0,24,83,24);
//			//Draw_Line(41,0,41,47);
//			//Draw_Circle((41+(x_dec/2)),23+(y_dec/2),3,PIXEL_ON);
//			//Draw_Circle((41+(x_dec/2)),23+(y_dec/2),2,PIXEL_ON);
//		
//			sprintf(ScrStr,"X=%d",x_dec);
//			LCD5110_set_XY(0,0);
//			LCD5110_write_string(ScrStr);

//			sprintf(ScrStr, "Y=%d",y_dec);
//			LCD5110_set_XY(8,0);
//			LCD5110_write_string(ScrStr);

//			sprintf(ScrStr, "Z=%d",z_dec);
//			LCD5110_set_XY(0,1);
//			LCD5110_write_string(ScrStr);

			Delay_ms(200);
			LCD5110_clear();
			for(i=0; i<=LCD_CACHE_SIZE; i++)
			{
				LCD_Memory[i]=0x00;
			}
	}
}

//void DrawMyLine(int x1, int y1, int x2, int y2, int mode)
//{
//    int yRange = y2-y1;  
//    int xRange = x2-x1;
//    int start,stop,step;
// 
//	  step = (mode == 2) ? 4 : (mode == 1 ? 2 : 1);
// 
//    if (abs(xRange) > abs(yRange)) {
//        start = x2 > x1 ? x1 : x2;
//        stop = x2 > x1 ? x2 : x1;
// 
//        for (int x = start; x <= stop; x += step) {
//            int y = y1 + (y2 - y1)*(x - x1)/(x2 - x1);

//            Set_Pixel(x, y, PIXEL_ON);
//        }
//    } else {
//        start = y2 > y1 ? y1 : y2;
//        stop = y2 > y1 ? y2 : y1;
// 
//        for (int y = start; y <= stop; y += step) {
//            int x = x1 + (x2 - x1)*(y - y1)/(y2 - y1);
// 
//            Set_Pixel(x, y, PIXEL_ON);
//        }
//    }
//}