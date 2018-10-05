#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_syscfg.h"

void initLeds();
void initButton();
void initExtInterrupt();

GPIO_InitTypeDef GPIO_InitDef;
EXTI_InitTypeDef   EXTI_InitStructure;
NVIC_InitTypeDef NVIC_InitStructure;

unsigned short light = 0x1000;

void EXTI0_IRQHandler(void){
	if(EXTI_GetITStatus(EXTI_Line0) == SET) {
	      GPIO_ToggleBits(GPIOD, light);		//4
		
				if(light == 0x8000)
					light = 0x1000;
				else
					light = light << 1;	
      
				GPIO_ToggleBits(GPIOD, light);		
		for (int i = 0; i < 1000000; i++);
		EXTI_ClearITPendingBit(EXTI_Line0);
		/* here we clear the interrupt flag, otherwise the interrupt will be handled many times */
	}
}
 
int main(void) {
		initLeds();
    initButton();
		initExtInterrupt();
	
	GPIO_ToggleBits(GPIOD, light);		
	
    while (1) 
		{
				
		}
}

void initLeds(){
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE); 
	
    GPIO_InitDef.GPIO_Pin = GPIO_Pin_12 |GPIO_Pin_13 |GPIO_Pin_14 |GPIO_Pin_15;
    GPIO_InitDef.GPIO_OType = GPIO_OType_PP;
    GPIO_InitDef.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitDef.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitDef.GPIO_Speed = GPIO_Speed_100MHz;
	
	 GPIO_Init(GPIOD, &GPIO_InitDef);
}

void initButton() {
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//3

    GPIO_InitTypeDef gpio;
    GPIO_StructInit(&gpio);
    gpio.GPIO_Mode = GPIO_Mode_IN;
    gpio.GPIO_PuPd = GPIO_PuPd_NOPULL;
    gpio.GPIO_Pin = GPIO_Pin_0;//2
    GPIO_Init(GPIOA, &gpio);//5
}

void initExtInterrupt() {

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA,EXTI_PinSource0);//1
    EXTI_InitStructure.EXTI_Line = EXTI_Line0;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger =  EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);


    NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}
