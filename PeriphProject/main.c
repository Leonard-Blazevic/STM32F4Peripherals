#include "stm32f4xx.h"

static __IO uint32_t TimingDelay;

TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
TIM_OCInitTypeDef  TIM_OCInitStructure;
GPIO_InitTypeDef GPIO_InitStructure;

uint16_t CCR1_Val = 1400;
uint16_t PrescalerValue = 0;

void TIM_Config(void);

int main(void)
{
	int i=0;
  TIM_Config();
  
  RCC_TIMCLKPresConfig(RCC_TIMPrescActivated);

  /* Compute the prescaler value */
  PrescalerValue = (uint16_t) (SystemCoreClock / 21000000) - 1;

  /* Time base configuration */
  TIM_TimeBaseStructure.TIM_Period = 699;
  TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

  /* PWM1 Mode configuration: Channel1 */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = CCR1_Val;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

  TIM_OC1Init(TIM3, &TIM_OCInitStructure);

  TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);

  /* TIM4 enable counter */
  TIM_Cmd(TIM3, ENABLE);

  while (1)
  {
		for (i=0;i<1000000;i++);
		
		if(CCR1_Val > 0){
			CCR1_Val-=20;
			TIM_OCInitStructure.TIM_Pulse = CCR1_Val;
			TIM_OC1Init(TIM3, &TIM_OCInitStructure);
		}
		else{
			TIM_OCInitStructure.TIM_Pulse = CCR1_Val = 1400;
			TIM_OC1Init(TIM3, &TIM_OCInitStructure);
		}
		
	}
}

void TIM_Config(void)
{
  /* TIM4 clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

  /* GPIOD clock enable */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
 
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource4, GPIO_AF_TIM3);
}



void TimingDelay_Decrement(void)
{
  if (TimingDelay != 0)
  {
    TimingDelay--;
  }
}
