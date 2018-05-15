/*
*******************************************************************************
*	                                  
*	ģ������ : printfģ��    
*	�ļ����� : printf.c
*	˵    �� : ʵ��printf��scanf�����ض��򵽴���1����֧��printf��Ϣ��USART1
*				ʵ���ض���ֻ��Ҫ���2������:
*				int fputc(int ch, FILE *f);
*				int fgetc(FILE *f);
*				����KEIL MDK������������ѡ������Ҫ��MicorLibǰ��򹳣����򲻻������ݴ�ӡ��USART1��
*				
*				���cģ���޶�Ӧ��h�ļ���
*
*******************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include <stdio.h>			


#if 1

struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
_sys_exit(int x) 
{ 
	x = x; 
} 

/*********************************************************
  * @function  fputc
  * @role      �ض���putc��������������ʹ��printf�����Ӵ���1��ӡ���
  * @input     None
  * @output    None
  * @return    None
  ********************************************************/
int fputc(int ch, FILE *f)
{      
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET){;}
    USART_SendData(USART1,(uint8_t)ch);   
	return ch;
}

/*********************************************************
  * @function  fgetc
  * @role      �ض���getc��������������ʹ��scanff�����Ӵ���1��������
  * @input     None
  * @output    None
  * @return    None
  ********************************************************/
int fgetc(FILE *f)
{
	/* �ȴ�����1�������� */
	while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);
	return (int)USART_ReceiveData(USART1);
}


#endif

