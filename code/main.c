/*
Push-button B1 (PC13) creates risign and falling edge interrupt.
This software prints (UART, 115200 BAUD) text when interrupt detected.
*/

/* Includes */

#include "stm32l1xx.h"
#define HSI_VALUE    ((uint32_t)16000000)
#include "nucleo152start.h"
#include <stdio.h>
#include <string.h>

/* Private typedef */
/* Private define  */
/* Private macro */
/* Private variables */
/* Private function prototypes */
/* Private functions */
void delay_Ms(int delay);
void USART2_write(char data);
void USART2_Init(void);
 int pulses = 0;

/**
**===========================================================================
**
**  Abstract: main program
**
**===========================================================================
*/
int main(void)
{
	__disable_irq();			//global disable IRQs, M3_Generic_User_Guide p135.
	USART2_Init();

  /* Configure the system clock to 32 MHz and update SystemCoreClock */
  SetSysClock();
  SystemCoreClockUpdate();

  /* TODO - Add your application code here */

RCC->AHBENR|=1; 				//GPIOA ABH bus clock ON. p154
RCC->AHBENR|=4;					//GPIOC ABH bus clock ON. p154
RCC->APB2ENR |=1;				//Bit 0 SYSCFGEN: System configuration controller clock enable. p157

//GPIOA->MODER&=~0x00000C00;		//clear (input reset state for PA5). p184
//GPIOA->MODER|=0x00000400; 		//GPIOA pin 5 (PA5) to output. p184
//GPIOC->MODER &= ~0x0C000000;	//clear (input state for PC13). p184



GPIOA->MODER &=~(1 << 14 | 1 << 15);   // input reset state for PA7
GPIOA->MODER &=~(1 << 12 | 1 << 13);   // input reset state for PA6

SYSCFG->EXTICR[1] &= ~0x0f00;	//clear port selection for EXTI6
//SYSCFG->EXTICR[3] |= 0x0020;	//select port C (PC13) for EXTI13. p223

EXTI->IMR |= (1 << 6);			//unmask EXTI6 (MR6), p242
//EXTI->FTSR |= 0x2000;			//select falling edge trigger, p243
EXTI->RTSR |= (1 << 6);			//select risign edge trigger, p243

NVIC_EnableIRQ(EXTI9_5_IRQn); //enable IRQ M3_Generic_User_Guide p130

//some info about interrupt system for stm32f4
//https://stm32f4-discovery.net/2014/08/stm32f4-external-interrupts-tutorial/

__enable_irq();			//global enable IRQs, M3_Generic_User_Guide p135

// configuation for DAC
int data=0;
		GPIOA->MODER |= 0x00000300;		//PA4 analog
		RCC->APB1ENR |=1<<29; 			//enable DAC clock
		DAC->CR|=1;						//enable DAC
		DAC->CR|=2;						//disable Buffer

GPIOA->MODER|=0x400; 	//GPIOA pin 5 to output. p184
GPIOA->ODR^=0x20;		//0010 0000 xor bit 5. p186
//delay_Ms(1000);
GPIOA->ODR^=0x20;		//0010 0000 xor bit 5. p186
//delay_Ms(1000);

  /* Infinite loop */
   while (1)
  {
	   GPIOA->ODR&=~0x20;
	   DAC->DHR12R1=pulses;
	   //delay_Ms(10);

	   //data = pulses;

	   float vol = ((float)pulses * 3.3 )/4096;
	   int newvol = (float)vol * 1000;
	   int deci = (int)vol;
	   int floattin = newvol % 1000;

	   char buf[100];
	   if (floattin < 10 && floattin < 100 ) {
		   sprintf (buf, "voltage is: %d.00%d", deci,floattin);
	   }
	   else if ( floattin < 100 && floattin > 10){
		   sprintf (buf, "voltage is: %d.0%d", deci,floattin);
	   } else
	   sprintf (buf, "voltage is: %d.%d", deci,floattin);
	   int len = 0;
	   while(buf[len]!='\0')
	   	  len++;

	   		for(int i=0;i<len;i++)
	   		{
	   			USART2_write(buf[i]);
	   		}

	   		USART2_write('\n');
	   		USART2_write('\r');
	   		delay_Ms(1000);

  }
  return 0;
}

void delay_Ms(int delay)
{
	int i=0;
	for(; delay>0;delay--)
		for(i=0;i<2460;i++); 	//measured with oscilloscope
}

void USART2_Init(void)
{
	RCC->APB1ENR|=0x00020000; 	//set bit 17 (USART2 EN)
	RCC->AHBENR|=0x00000001; 	//enable GPIOA port clock bit 0 (GPIOA EN)
	GPIOA->AFR[0]=0x00000700;	//GPIOx_AFRL p.188,AF7 p.177
	GPIOA->MODER|=0x00000020; 	//MODER2=PA2(TX) to mode 10=alternate function mode. p184

	USART2->BRR = 0x00000116;	//115200 BAUD and crystal 32MHz. p710, 116
	USART2->CR1 = 0x00000008;	//TE bit. p739-740. Enable transmit
	USART2->CR1 |= 0x00002000;	//UE bit. p739-740. Uart enable
}

void USART2_write(char data)
{
	//wait while TX buffer is empty
	while(!(USART2->SR&0x0080)){} 	//TXE: Transmit data register empty. p736-737
		USART2->DR=(data);			//p739
}

void EXTI9_5_IRQHandler(void)
{

	GPIOA->ODR^=0x20;
	char buf[100];

	if((GPIOA->IDR&(1<<7)) && pulses >= 15) //if input PC13 state logical 0 = 0V
	{
		pulses = pulses -15;

	}

	else if ((GPIOA->IDR&(1<<7)) == 0 && pulses <= 4080 )
	{
		pulses = pulses + 15;


	}
	sprintf(buf, "number of pulses %d \n\r", pulses);

	int len = 0;
	while (buf[len] != '\0') len++;
	for (int i = 0; i < len; i++)
	{
		USART2_write(buf[i]);
	}

	EXTI->PR=(1 << 6 );		//Pending, This bit is cleared by writing a ‘1’ to the bit. p245
}
