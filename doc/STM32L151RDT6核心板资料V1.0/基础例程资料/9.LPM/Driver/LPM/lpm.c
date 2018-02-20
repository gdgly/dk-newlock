#include "lpm.h"

#define CR_VOS_MASK              ((uint32_t)0xFFFFE7FF)
#define PWR_VoltageScaling_Range1       PWR_CR_VOS_0
#define PWR_VoltageScaling_Range2       PWR_CR_VOS_1
#define PWR_VoltageScaling_Range3       PWR_CR_VOS


/*******************************************************************
�� �� ����__asm void WFI_SET(void)
����˵����THUMBָ�֧�ֻ������
		  �������·���ʵ��ִ�л��ָ��WFI
��	  ������
�� �� ֵ����
*******************************************************************/


void Low_Power_Select(u8 LPM)
{
	switch(LPM)
		{
			case 0://˯��ģʽ
				Sleep_Mode();
				break;
			case 1://�͵�ѹ����
				LP_Run_Mode();
				break;
			case 2://�͵�ѹ˯��ģʽ
				
				break;
			case 3://ֹͣģʽ
				Stop_Mode();
				break;
			case 4://����ģʽ
				Standby_Mode();
				break;
		}
}
void Sleep_Mode(void)
{
	RCC->APB1ENR|=1<<28;     //ʹ�ܵ�Դʱ��	
	PWR->CR &= ~(3<<0);		//���PDDS ��LPDSR
	PWR->CR |= 1<<0;		//set LPDSR
	/* Clear SLEEPDEEP bit of Cortex System Control Register */
	SCB->SCR &= (uint32_t)~((uint32_t)SCB_SCR_SLEEPDEEP);
	__WFI();				 //ִ��WFIָ��	
}
/* PWR_EnterLowPowerRunMode */
void LP_Run_Mode(void)
{
	RCC->APB1ENR |= 1<<28;     //ʹ�ܵ�Դʱ��
	PWR_VoltageScalingConfig(PWR_VoltageScaling_Range2);//1.5v
	PWR->CR |= PWR_CR_LPSDSR;
    PWR->CR |= PWR_CR_LPRUN;   	
}
void Stop_Mode(void)
{

	/*����low power mode*/
	LP_Run_Mode();
	RCC->APB1ENR|=1<<28;     //ʹ�ܵ�Դʱ��	

	PWR_VoltageScalingConfig(PWR_VoltageScaling_Range2);//1.5v
	 
	PWR->CR &= ~(3<<0);		//���PDDS ��LPDSR
	PWR->CR |= 1<<0;		//set LPDSR
	/* Set SLEEPDEEP bit of Cortex System Control Register */
	SCB->SCR |= SCB_SCR_SLEEPDEEP;
	RCC->APB1ENR &= ~(1<<28);     /*�رյ�Դʱ��*/	
	__WFI();
	/* Reset SLEEPDEEP bit of Cortex System Control Register */
	SCB->SCR &= (uint32_t)~((uint32_t)SCB_SCR_SLEEPDEEP);  	
}

void Standby_Mode(void)
{
	RCC->APB1ENR|=1<<28;     //ʹ�ܵ�Դʱ��	 
	PWR_VoltageScalingConfig(PWR_VoltageScaling_Range2);//1.5v	
	/* Clear Wakeup flag */
	PWR->CR |= PWR_CR_CWUF; 
	/* Select STANDBY mode */
	PWR->CR |= PWR_CR_PDDS; 
	/* Set SLEEPDEEP bit of Cortex System Control Register */
	SCB->SCR |= SCB_SCR_SLEEPDEEP;
	PWR->CSR |= 1<<8;//PA0 �����ж�
	/* Request Wait For Interrupt */
	__WFI();
	/* Reset SLEEPDEEP bit of Cortex System Control Register */
	SCB->SCR &= (uint32_t)~((uint32_t)SCB_SCR_SLEEPDEEP); 
}






