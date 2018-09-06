#include "stm32f4xx.h"

unsigned short delay_c = 0;
unsigned short blink_delay = 500;
unsigned short light = 0x1000;

void SysTick_Handler(void){
	if(delay_c > 0)
	delay_c--;
}

void delay_ms(unsigned short delay_t){
	delay_c = delay_t;
	while(delay_c){};
}

void init(void){
	SysTick_Config(SystemCoreClock/1000);
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
	GPIOD->MODER |= 0x55000000;
	GPIOD->OTYPER = 0;
	GPIOD->OSPEEDR = 0;
}

void main_loop(void) {
	GPIOD->ODR = light;
	delay_ms(blink_delay);
	GPIOD->ODR = 0;
	delay_ms(blink_delay);
	if(light == 0x8000)
		light = 0x1000;
	else
		light = light << 1;	
}

int main(void) {
	init();
	while(1) {
		main_loop();
	}
}
