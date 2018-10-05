#include "stm32f4xx.h"
#include "cmsis_os.h"

void initLeds(void);
void initThreads(void);
void firstTask(void const *p);
void secondTask(void const *p);

osThreadId firstThread;
osThreadId secondThread;

unsigned int counter;
osEvent evt;

int main(){
	counter = 0;
	initLeds();
	
	if (osKernelInitialize() != osOK){
		return -1;
	}
	
	initThreads();
		
	if (osKernelStart() != osOK) {
		return -1;
	}
	//while (1){}
}

void initThreads(){
	osThreadDef(firstTask, osPriorityNormal,1,0);
  osThreadDef(secondTask, osPriorityNormal,1,0);
	
	firstThread = osThreadCreate(osThread(firstTask), NULL);
	secondThread = osThreadCreate(osThread(secondTask), NULL);
	

	//int prior = osThreadGetPriority(firstThread);
	//int p2 = prior;
	//GPIOD->ODR |= 0x2000;	
}

unsigned short delay_c = 0;
unsigned short blink_delay = 500;
unsigned short light = 0x1000;

void delay_ms(unsigned short delay_t){
	int n = 30000 * delay_t;
	for(int i =0; i < n; i++);
}

void initLeds(void){
	SysTick_Config(SystemCoreClock/1000);
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
	GPIOD->MODER |= 0x55000000;
	GPIOD->OTYPER = 0;
	GPIOD->OSPEEDR = 0;
}

void firstTask(void const *p){	
	while (1){
		GPIOD->ODR |= 0x5000;
	
	
		if (counter % 1000 == 0)
			osSignalSet(secondThread, 0x0001);  
		
		if(counter == 10000)
			counter = 0;
		
		delay_ms(1);
		counter++;
	}
}

void secondTask(void const *p){	
	unsigned int blue = 0x8000;
	
	while (1){
		
		evt = osSignalWait(0x0001, osWaitForever); /* here we are waiting for a signal */
		if (evt.status == osEventSignal) /* if the signal has came, the thread’s logic start executing */
		{
			if((GPIOD->ODR & blue) == blue)			
				GPIOD->ODR &= ~blue;
			else
				GPIOD->ODR |= blue;
		} /* the end of the thread’s logic */

		//osThreadYield(); 
	}	
}
