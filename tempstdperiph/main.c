#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"

void Delay_ms(uint32_t ms);
void RCC_Configuration(void);
ErrorStatus HSEStartUpStatus;

GPIO_InitTypeDef GPIO_InitDef;

RCC_ClocksTypeDef RCC_Clocks;

//unsigned short light = 0x1000;
unsigned short light = 0x1000;
unsigned short blink_delay = 500;
 
int main(void) {
	  RCC_GetClocksFreq (&RCC_Clocks);

    RCC_Configuration();

    RCC_GetClocksFreq (&RCC_Clocks);
	
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
 
    GPIO_InitDef.GPIO_Pin = GPIO_Pin_12 |GPIO_Pin_13 |GPIO_Pin_14 |GPIO_Pin_15;
    GPIO_InitDef.GPIO_OType = GPIO_OType_PP;
    GPIO_InitDef.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitDef.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitDef.GPIO_Speed = GPIO_Speed_100MHz;
    //Initialize pins
    GPIO_Init(GPIOD, &GPIO_InitDef);
 
    volatile int i;
    while (1) {
			
				GPIO_ToggleBits(GPIOD, light);		
        Delay_ms(blink_delay);
				GPIO_ToggleBits(GPIOD, light);
			
				if(light == 0x8000)
					light = 0x1000;
				else
					light = light << 1;		
    }
}

void Delay_ms(uint32_t ms)
{
        volatile uint32_t nCount;
        RCC_ClocksTypeDef RCC_Clocks;
				RCC_GetClocksFreq (&RCC_Clocks);

        nCount=(RCC_Clocks.HCLK_Frequency/10000)*ms;
        for (; nCount!=0; nCount--);
}
//------------
void RCC_Configuration(void)
{
    /*RCC system reset(for debug purpose) */
    RCC_DeInit();

    /* Enable HSE */
    RCC_HSEConfig(RCC_HSE_ON);

    /* Wait till HSE is ready */
    HSEStartUpStatus = RCC_WaitForHSEStartUp();

    if (HSEStartUpStatus == SUCCESS)
    {
        /* HCLK = SYSCLK */
        RCC_HCLKConfig(RCC_SYSCLK_Div1);

        /* PCLK2 = HCLK*/
        RCC_PCLK2Config(RCC_HCLK_Div1);

        /* PCLK1 = HCLK*/
        RCC_PCLK1Config(RCC_HCLK_Div1);

        /* Enable PLL */
        RCC_PLLCmd(ENABLE);

        /* Wait till PLL is ready */
        while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET) {}

        /* Select PLL as system clock source */
        RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

        /* Wait till PLL is used as system clock source */
        while (RCC_GetSYSCLKSource() != 0x08) {}
    }

    /*Then need to enable peripheral clocks ----------------------------------------------*/
}
