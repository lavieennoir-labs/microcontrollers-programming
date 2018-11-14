#include "main.h"
#include "stdio.h"
#define  STM32F40_41xxx

uint16_t delay_c = 0; 
uint8_t ID = 0;
int16_t x_dec,y_dec,z_dec,x_x,y_y;
uint8_t LCD_Memory[LCD_CACHE_SIZE];
short i;

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
	
	ID = Read_Val_Reg(LIS3DSH_WHO_AM_I_ADDR);
	InitLIS3DSH(TM_LIS3DSH_Sensitivity_2G,TM_LIS3DSH_Filter_50Hz);
	Delay_ms(20);
	ID = Read_Val_Reg(0x0C);
	
	
	
  while(1)
	{		
			ReadAxes(&Axes_Data);
			x_dec = Axes_Data.X/11;
			y_dec = Axes_Data.Y/11;
			z_dec = Axes_Data.Z/11;
			//Draw_Circle(41,23,23,PIXEL_ON);
			//Draw_Circle(41,23,16,PIXEL_ON);
			//Draw_Circle(41,23,9,PIXEL_ON);
			
			//Draw_Line(0,24,83,24);
			//Draw_Line(41,0,41,47);
			//Draw_Circle((41+(x_dec/2)),23+(y_dec/2),3,PIXEL_ON);
			//Draw_Circle((41+(x_dec/2)),23+(y_dec/2),2,PIXEL_ON);
		
			sprintf(ScrStr,"X=%d",x_dec);
			LCD5110_set_XY(0,0);
			LCD5110_write_string(ScrStr);

			sprintf(ScrStr, "Y=%d",y_dec);
			LCD5110_set_XY(8,0);
			LCD5110_write_string(ScrStr);

			sprintf(ScrStr, "Z=%d",z_dec);
			LCD5110_set_XY(0,1);
			LCD5110_write_string(ScrStr);

			Delay_ms(200);
			LCD5110_clear();
			for(i=0; i<=LCD_CACHE_SIZE; i++)
			{
				LCD_Memory[i]=0x00;
			}
	}
}