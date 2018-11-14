#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "main.h"
#include "font.h"

extern uint8_t LCD_Memory[LCD_CACHE_SIZE];
static int   LoWaterMark;   
static int   HiWaterMark;   
//Define the hardware operation function
void LCD5110_GPIO_Config(void);
void LCD5110_SCK(unsigned char temp);
void LCD5110_MO(unsigned char temp);
void LCD5110_CS(unsigned char temp);
void LCD5110_RST(unsigned char temp);
void LCD5110_DC(unsigned char temp);

void LCD_init()
{
	LCD5110_GPIO_Config();

	LCD5110_DC(1);//LCD_DC = 1;
	LCD5110_MO(1);//SPI_MO = 1;
	LCD5110_SCK(1);//SPI_SCK = 1;
	LCD5110_CS(1);//SPI_CS = 1;
	
	LCD5110_RST(0);//LCD_RST = 0;
	Delay_ms(10);
	LCD5110_RST(1);//LCD_RST = 1;

	LCD5110_LCD_write_byte(0x21,0);
	LCD5110_LCD_write_byte(0xc6,0);
	LCD5110_LCD_write_byte(0x06,0);
	LCD5110_LCD_write_byte(0x13,0);
	LCD5110_LCD_write_byte(0x20,0);
	LCD5110_clear();
	LCD5110_LCD_write_byte(0x0c,0);
}

void LCD5110_LCD_write_byte(unsigned char dat,unsigned char mode)
{
	unsigned char i;

	LCD5110_CS(0);//SPI_CS = 0;

	if (0 == mode)
		LCD5110_DC(0);//LCD_DC = 0;
	else
		LCD5110_DC(1);//LCD_DC = 1;

	for(i=0;i<8;i++)
	{
		LCD5110_MO(dat & 0x80);//SPI_MO = dat & 0x80;
		dat = dat<<1;
		LCD5110_SCK(0);//SPI_SCK = 0;
		LCD5110_SCK(1);//SPI_SCK = 1;
	}
	LCD5110_CS(1);//SPI_CS = 1;

}

void LCD5110_write_char(unsigned char c)
{
	unsigned char line;
	unsigned char ch = 0;

	c = c - 32;

	for(line=0;line<6;line++)
	{
		ch = font6_8[c][line];
		LCD5110_LCD_write_byte(ch,1);
		
	}
}
void LCD5110_write_char_reg(unsigned char c)
{
	unsigned char line;
	unsigned char ch = 0;

	c = c - 32;

	for(line=0;line<6;line++)
	{
		ch = ~font6_8[c][line];
		LCD5110_LCD_write_byte(ch,1);
	}
}

void LCD5110_write_string(char *s)
{
	unsigned char ch;
	
  while(*s!='\0')
	{
		ch = *s;
		LCD5110_write_char(ch);
		s++;
	}
}


void LCD5110_clear()
{
	unsigned char i,j;
	
	for(i=0;i<6;i++)
		for(j=0;j<84;j++)
			LCD5110_LCD_write_byte(0,1);	
}

void LCD5110_set_XY(unsigned char X,unsigned char Y)
{
	unsigned char x;
	
	x = 6*X;
	LCD5110_LCD_write_byte(0x40|Y,0);
	LCD5110_LCD_write_byte(0x80|x,0);
}

void LCD5110_Write_Dec(unsigned int b)
{
	unsigned char datas[4];

	datas[0] = b/1000;
	b = b - datas[0]*1000;
	datas[1] = b/100;
	b = b - datas[1]*100;
	datas[2] = b/10;
	b = b - datas[2]*10;
	datas[3] = b;

	datas[0]+=48;
	datas[1]+=48;
	datas[2]+=48;
	datas[3]+=48;

	LCD5110_write_char(datas[0]);
	LCD5110_write_char(datas[1]);
	LCD5110_write_char(datas[2]);
	LCD5110_write_char(datas[3]);
}

void Lcd3310_Pict(const char img[])
{
	unsigned char x,y;
	
	for(y=0;y < 8;y++)
	{
		goto_XY(0, y);
		for(x=0;x < 84;x++)
		LCD5110_LCD_write_byte(img[y*84+x],1);
	}
}

void goto_XY(unsigned char X,unsigned char Y)
{
	LCD5110_LCD_write_byte(0x80|X,0);
	LCD5110_LCD_write_byte(0x40|Y,0);
}

uint8_t Set_Pixel (unsigned char x,unsigned char y,uint8_t mode)
{
	 int  index;
   uint8_t  offset;
   uint8_t  data;


   if (x >= X_MAX || y >= Y_MAX) return 0;
   index = (( y / 8 ) * 84 ) + x;
   offset  = y - (( y / 8 ) * 8);
   data = LCD_Memory[index];
	
   if (mode == PIXEL_OFF )
   {
     data &= (~(0x01 << offset ));
   }
   else if ( mode == PIXEL_ON)
   {
     data |= (0x01 << offset);
   }
   else if (mode  == PIXEL_XOR)
   {
     data ^= (0x01 << offset);
   }
   LCD_Memory[index] = data;
   if (index < LoWaterMark )
   {
      LoWaterMark = index;
   }
   if (index > HiWaterMark )
   {
      HiWaterMark = index;
   }
	 
   return 1;
}

void update (void)
{
	int i;
	
	if (LoWaterMark < 0)
     LoWaterMark = 0;
  else if (LoWaterMark >= LCD_CACHE_SIZE)
     LoWaterMark = LCD_CACHE_SIZE - 1;

  if (HiWaterMark < 0)
     HiWaterMark = 0;
  else if ( HiWaterMark >= LCD_CACHE_SIZE )
      HiWaterMark = LCD_CACHE_SIZE - 1;
	

	goto_XY(0,0);
  for (i = 0; i <= LCD_CACHE_SIZE; i++ )
  {
    LCD5110_LCD_write_byte(LCD_Memory[i], 1);
  }
  LoWaterMark = LCD_CACHE_SIZE - 1;
  HiWaterMark = 0;
}

void DrawRect (uint8_t x1, uint8_t x2, uint8_t y1, uint8_t y2)
{
	Draw_Line(x1,y1,x1,y2);
	Draw_Line(x1,y1,x2,y1);
	Draw_Line(x2,y1,x2,y2);
	Draw_Line(x2,y1,x2,y2);
	update();
}

uint8_t Draw_Circle(uint8_t x, uint8_t y, uint8_t radius, uint8_t mode)
{
    signed char xc = 0;
    signed char yc = 0;
    signed char p = 0;

    if ( x >= X_MAX || y >= Y_MAX) return 0;

    yc = radius;
    p = 3 - (radius<<1);
    while (xc <= yc)  
    {
        Set_Pixel(x + xc, y + yc, mode);
        Set_Pixel(x + xc, y - yc, mode);
        Set_Pixel(x - xc, y + yc, mode);
        Set_Pixel(x - xc, y - yc, mode);
        Set_Pixel(x + yc, y + xc, mode);
        Set_Pixel(x + yc, y - xc, mode);
        Set_Pixel(x - yc, y + xc, mode);
        Set_Pixel(x - yc, y - xc, mode);
        if (p < 0) p += (xc++ << 2) + 6;
            else p += ((xc++ - yc--)<<2) + 10;
    }
		update();
    return 1;
}

void Draw_Line (uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2)
{
    int dx, dy, stepx, stepy, fraction;

    dy = y2 - y1;
    dx = x2 - x1;

    if (dy < 0)
    {
      dy = -dy;
      stepy = -1;
    }
    else
    {
      stepy = 1;
    }

    if (dx < 0)
    {
			dx = -dx;
      stepx = -1;
    }
    else
    {
      stepx = 1;
    }
    dx <<= 1;
    dy <<= 1;
    Set_Pixel(x1, y1,PIXEL_ON);

    if (dx > dy)
    {
        fraction = dy - (dx >> 1);
        while (x1 != x2)
        {
            if (fraction >= 0)
            {
              y1 += stepy;
              fraction -= dx;
            }
            x1 += stepx;
            fraction += dy;
            Set_Pixel(x1, y1,PIXEL_ON);
        }
    }
    else
    {
        fraction = dx - (dy >> 1);
        while ( y1 != y2 )
        {
            if ( fraction >= 0 )
            {
                x1 += stepx;
                fraction -= dy;
            }
            y1 += stepy;
            fraction += dx;
            Set_Pixel(x1, y1,PIXEL_ON);
        }
    }
}


void LCD5110_GPIO_Config()
{
	GPIO_InitTypeDef GPIOA_Init;
	GPIO_InitTypeDef GPIOC_Init;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOC, ENABLE);
	
	GPIOA_Init.GPIO_Pin = GPIO_Pin_8;
	GPIOA_Init.GPIO_Speed = GPIO_Speed_25MHz;
	GPIOA_Init.GPIO_Mode = GPIO_Mode_OUT;
	GPIOA_Init.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIOA_Init.GPIO_OType = GPIO_OType_PP;

	GPIOC_Init.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;
	GPIOC_Init.GPIO_Speed = GPIO_Speed_25MHz;
	GPIOC_Init.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIOC_Init.GPIO_Mode = GPIO_Mode_OUT;
	GPIOC_Init.GPIO_OType = GPIO_OType_PP;

	GPIO_Init(GPIOA,&GPIOA_Init);
	GPIO_Init(GPIOC,&GPIOC_Init);

}

void LCD5110_CS(unsigned char temp)
{
	if (temp) GPIO_SetBits(GPIOC, GPIO_Pin_9);
	else GPIO_ResetBits(GPIOC, GPIO_Pin_9);
}

void LCD5110_RST(unsigned char temp)
{
	if (temp) GPIO_SetBits(GPIOC, GPIO_Pin_8);
	else GPIO_ResetBits(GPIOC, GPIO_Pin_8);
}

void LCD5110_DC(unsigned char temp)
{
	if (temp) GPIO_SetBits(GPIOC, GPIO_Pin_7);
	else GPIO_ResetBits(GPIOC, GPIO_Pin_7);
}

void LCD5110_MO(unsigned char temp)
{
	if (temp) GPIO_SetBits(GPIOC, GPIO_Pin_6);
	else GPIO_ResetBits(GPIOC, GPIO_Pin_6);
}

void LCD5110_SCK(unsigned char temp)
{
	if (temp) GPIO_SetBits(GPIOA, GPIO_Pin_8);
	else GPIO_ResetBits(GPIOA, GPIO_Pin_8);
}

