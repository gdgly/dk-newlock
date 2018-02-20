#include "led.h"
#include "stm32l1xx.h"
#include "stm32l1xx_gpio.h"
#include "stm32l1xx_rcc.h"
void LED_Init(void)
{
    
  GPIO_InitTypeDef GPIO_InitStructure; 
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//����
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;//����
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;//ʱ������
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_7|GPIO_Pin_6|GPIO_Pin_5;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  GPIO_SetBits(GPIOB, GPIO_Pin_9|GPIO_Pin_7|GPIO_Pin_6|GPIO_Pin_5);  
    	     	
}






