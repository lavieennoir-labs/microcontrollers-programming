#include "stm32f4xx.h"
#include "cmsis_os.h"

void task1(void const *p);
void task2(void const *p);
void task3(void const *p);
void task4(void const *p);
void task5(void const *p);
void task6(void const *p);

osThreadId thread1, 
					 thread2, 
					 thread3, 
					 thread4, 
					 thread5, 
					 thread6;

void delay_ms(unsigned short delay_t){
	int n = 30000 * delay_t;
	for(int i = 0; i < n; i++);
}

void initThreads(){
	osThreadDef(task1, osPriorityNormal,1,0);
	//osThreadDef(task2, osPriorityNormal,1,0);
	//osThreadDef(task3, osPriorityNormal,1,0);
	//osThreadDef(task4, osPriorityNormal,1,0);
	//osThreadDef(task5, osPriorityNormal,1,0);
	
	thread1 = osThreadCreate(osThread(task1), (void*)0x4000);
	//thread2 = osThreadCreate(osThread(task2), NULL);
	//thread3 = osThreadCreate(osThread(task3), NULL);
	//thread4 = osThreadCreate(osThread(task4), NULL);
	//thread5 = osThreadCreate(osThread(task5), NULL);		
}

void initLeds(void){
	SysTick_Config(SystemCoreClock/1000);
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
	GPIOD->MODER |= 0x55000000;
	GPIOD->OTYPER = 0;
	GPIOD->OSPEEDR = 0;
}

int main(){
	initLeds();
	
	if (osKernelInitialize() != osOK)
		return -1;
	initThreads();		
	if (osKernelStart() != osOK)
		return -1;
	
	//M	
	for(int i = 0; i < 10000000; i++);
	if (osKernelInitialize() != osOK)
		return -1;
	
  osThreadDef(task6, osPriorityNormal,1,0);
	thread6 = osThreadCreate(osThread(task6), NULL);
	
	if (osKernelStart() != osOK)
		return -1;
		
	//D
	delay_ms(10000);
	osThreadTerminate(thread3);
	
	//R
	delay_ms(20000);
	osThreadTerminate(thread1);
	
	//while (1){}
}





void task1(void const *p){	
	while (1){		
		GPIOD->ODR &= 0x0FFF;
		GPIOD->ODR |= (unsigned int)p;//0x4000;
		//delay_ms(100);	
		osThreadYield();		
	}
}
void task2(void const *p){	
	while (1){		
		GPIOD->ODR &= 0x0FFF;
		GPIOD->ODR |= 0x1000;	
		//delay_ms(100);		
		osThreadYield();
	}
}
void task3(void const *p){	
	while (1){		
		GPIOD->ODR &= 0x0FFF;
		GPIOD->ODR |= 0x8000;
		//delay_ms(100);			
		osThreadYield();
	}
}
void task4(void const *p){	
	while (1){		
		GPIOD->ODR &= 0x0FFF;
		GPIOD->ODR |= 0x6000;	
		//delay_ms(100);		
		osThreadYield();
	}
}
void task5(void const *p){	
	while (1){		
		GPIOD->ODR &= 0x0FFF;
		GPIOD->ODR |= 0x9000;	
		//delay_ms(100);		
		osThreadYield();
	}
}
void task6(void const *p){	
	while (1){		
		GPIOD->ODR &= 0x0FFF;
		GPIOD->ODR |= 0xd000;	
		//delay_ms(100);		
		osThreadYield();
	}
}