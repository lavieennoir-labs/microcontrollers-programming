#include "stm32f4xx.h"
#include "cmsis_os.h"

void initLeds(void);
void initThreads(void);
void firstTask(void const *p);
void secondTask(void const *p);

osThreadId firstThread;
osThreadId secondThread;


int main(){
	initLeds();
	if (osKernelInitialize() != osOK){
	    return -1;
	}
	
	initThreads();
		
	if (osKernelStart() != osOK) {
		return -1;
	}
}

void initThreads(){
	osThreadDef(firstTask, osPriorityNormal,1,2000);
  osThreadDef(secondTask, osPriorityNormal,1,2000);
	
	firstThread = osThreadCreate(osThread(firstTask), NULL);
	secondThread = osThreadCreate(osThread(secondTask), NULL);
	

	int prior = osThreadGetPriority(firstThread);
	int p2 = prior;
}

unsigned short delay_c = 0;
unsigned short blink_delay = 500;
unsigned short light = 0x1000;

//void delay_ms(unsigned short delay_t){
//	delay_c = delay_t;
//	while(delay_c){};
//}

void initLeds(void){
	//SysTick_Config(SystemCoreClock/1000);
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
	GPIOD->MODER |= 0x55000000;
	GPIOD->OTYPER = 0;
	GPIOD->OSPEEDR = 0;
}

void firstTask(void const *p){
	GPIOD->ODR |= 0x5000;
	
	while (1){

}
}

void secondTask(void const *p){
	
}
