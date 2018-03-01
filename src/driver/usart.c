/*************************************************
  Copyright (C), 1988-1999,  Tech. Co., Ltd.
  File name:      
  Author:       Version:        Date:
  Description:   
  Others:         
  Function List:  
    1. ....
  History:         
                   
    1. Date:
       Author:
       Modification:
*************************************************/


/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/
#include "stm32l1xx_usart.h"
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include "usart.h"
#include "timer.h"



extern uint8_t protocol_buff[512];


static usart_buff_t sb = SerialBuffDefault();


usart_buff_t *gprs_buff = &sb;			//GPRS ���ջ�����
usart_buff_t *usart1_rx_buff = &sb;
usart_buff_t *usart2_rx_buff = &sb;
usart_buff_t *usart3_rx_buff = &sb;


uint8_t usart1_buff[USART_BUFF_LENGHT] = {0};
uint8_t usart2_buff[USART_BUFF_LENGHT] = {0};
uint8_t usart3_buff[USART_BUFF_LENGHT] = {0};
uint16_t usart1_cnt = 0;
uint16_t usart2_cnt = 0;
uint16_t usart3_cnt = 0;

uint8_t usart1_rx_status = 0;
uint8_t usart2_rx_status = 0;
uint8_t usart3_rx_status = 0;







void usart_gpio_init(void)
{
	GPIO_InitTypeDef gpio_init_structure;
	
		// UART1
	gpio_init_structure.GPIO_Pin = GPIO_Pin_9;				// UART1 TX				    
  	gpio_init_structure.GPIO_Mode = GPIO_Mode_OUT;
  	gpio_init_structure.GPIO_Speed = GPIO_Speed_10MHz;			
  	GPIO_Init(GPIOA, &gpio_init_structure);
	gpio_init_structure.GPIO_Pin = GPIO_Pin_10;				
  	gpio_init_structure.GPIO_Mode = GPIO_Mode_OUT;
  	gpio_init_structure.GPIO_Speed = GPIO_Speed_10MHz;			 
  	GPIO_Init(GPIOA, &gpio_init_structure);
	
	// UART2
	gpio_init_structure.GPIO_Pin = GPIO_Pin_2;				// UART2 TX				    
  	gpio_init_structure.GPIO_Mode = GPIO_Mode_OUT;
  	gpio_init_structure.GPIO_Speed = GPIO_Speed_10MHz;			
  	GPIO_Init(GPIOA, &gpio_init_structure);
	gpio_init_structure.GPIO_Pin = GPIO_Pin_3;				
  	gpio_init_structure.GPIO_Mode = GPIO_Mode_OUT;
  	gpio_init_structure.GPIO_Speed = GPIO_Speed_10MHz;			 
  	GPIO_Init(GPIOA, &gpio_init_structure);


	// UART3
	gpio_init_structure.GPIO_Pin = GPIO_Pin_10;				// UART3 TX				    
  	gpio_init_structure.GPIO_Mode = GPIO_Mode_OUT;
  	gpio_init_structure.GPIO_Speed = GPIO_Speed_10MHz;			
  	GPIO_Init(GPIOB, &gpio_init_structure);
	gpio_init_structure.GPIO_Pin = GPIO_Pin_11;				
  	gpio_init_structure.GPIO_Mode = GPIO_Mode_OUT;
  	gpio_init_structure.GPIO_Speed = GPIO_Speed_10MHz;			 
  	GPIO_Init(GPIOB, &gpio_init_structure);
	

}






/*
*********************************************************************************************************
*                                          usart1_init()
*
* Description : ��usart1���г�ʼ��
*
* Argument(s) : none
*
* Return(s)   : none
*
* Caller(s)   : AppTaskStart()
*
* Note(s)     : none.
*********************************************************************************************************
*/
void usart1_init(uint32_t band_rate, uint8_t word_length, uint8_t parity, uint8_t stop_bit)
{
	
	USART_InitTypeDef usart_init_structre;
	
	usart_init_structre.USART_BaudRate = band_rate;
	usart_init_structre.USART_WordLength = word_length;
	usart_init_structre.USART_StopBits = stop_bit;
	usart_init_structre.USART_Parity = parity;
	usart_init_structre.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	usart_init_structre.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &usart_init_structre);
		
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	
	USART_Cmd(USART1, ENABLE);
	
}





/*
*********************************************************************************************************
*                                          usart2_init()
*
* Description : ��usart2���г�ʼ��.
*
* Argument(s) : none
*
* Return(s)   : none
*
* Caller(s)   : AppTaskStart()
*
* Note(s)     : none.
*********************************************************************************************************
*/
void usart2_init(uint32_t band_rate)
{
	USART_InitTypeDef usart_init_structre;
	
	usart_init_structre.USART_BaudRate = band_rate;
	usart_init_structre.USART_WordLength = USART_WordLength_8b;
	usart_init_structre.USART_StopBits = USART_StopBits_1;
	usart_init_structre.USART_Parity = USART_Parity_No;
	usart_init_structre.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	usart_init_structre.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART2, &usart_init_structre);
		
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	
	USART_Cmd(USART2, ENABLE);
	
}




/*
*********************************************************************************************************
*                                          usart3_init()
*
* Description : ��usart3���г�ʼ��
*
* Argument(s) : none
*
* Return(s)   : none
*
* Caller(s)   : AppTaskStart()
*
* Note(s)     : none.
*********************************************************************************************************
*/
void usart3_init(uint32_t band_rate)
{
	USART_InitTypeDef usart_init_structre;
	
	usart_init_structre.USART_BaudRate = band_rate;
	usart_init_structre.USART_WordLength = USART_WordLength_8b;
	usart_init_structre.USART_StopBits = USART_StopBits_1;
	usart_init_structre.USART_Parity = USART_Parity_No;
	usart_init_structre.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	usart_init_structre.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART3, &usart_init_structre);
		
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
	
	USART_Cmd(USART3, ENABLE);
	
}



/*
*********************************************************************************************************
*                                          USART1_IRQHandler()
*
* Description : Create application kernel objects tasks.
*
* Argument(s) : none
*
* Return(s)   : none
*
* Caller(s)   : AppTaskStart()
*
* Note(s)     : none.
*********************************************************************************************************
*/
void USART1_IRQHandler(void)
{

	uint8_t ch = 0;	
	
   	if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
    {   
	    USART_ClearITPendingBit(USART1, USART_IT_RXNE);	
				
        ch = USART_ReceiveData(USART1);	 
		
		timer_is_timeout_1ms(timer_uart1, 0);
		
//		if(usart1_rx_status == 0)
		{
			
			if (usart1_rx_buff->index < USART_BUFF_LENGHT)
			{	
				usart1_rx_buff->pdata[usart1_rx_buff->index++] = ch;
				usart1_rx_status = 1;
			}
			else
			{
				memset(usart1_rx_buff, 0, sizeof(usart_buff_t));	//��������
					
			}
		}
	}
	
	if(USART_GetITStatus(USART1, USART_IT_TXE) != RESET)                  
  	{ 
     	USART_ITConfig(USART1, USART_IT_TXE, DISABLE);					   
  	}	
	

}

void usart1_recv_data(void)
{

	if(timer_is_timeout_1ms(timer_uart1, 40) == 0)	//40msû���յ�������Ϊ�����������		
	{

//		USART_OUT(USART2, usart1_buff);
//		USART_OUT(USART1, usart1_rx_buff->pdata);
		
		memcpy(gprs_buff, usart2_rx_buff, sizeof(usart_buff_t));
		
		USART_OUT(USART1, gprs_buff->pdata);
		
		memset(usart1_rx_buff, 0, sizeof(usart_buff_t));
	}

}

/*
*********************************************************************************************************
*                                          USART2_IRQHandler()
*
* Description : Create application kernel objects tasks.
*
* Argument(s) : none
*
* Return(s)   : none
*
* Caller(s)   : AppTaskStart()
*
* Note(s)     : none.
*********************************************************************************************************
*/
void USART2_IRQHandler(void)
{
	uint8_t ch = 0;	

   	if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
    {   
	    USART_ClearITPendingBit(USART2, USART_IT_RXNE);	
		
		timer_is_timeout_1ms(timer_uart2, 0);		//��ʱ������
		
		if(usart2_rx_status == 0)
		{
			ch = USART_ReceiveData(USART2);	 
			
			if (usart2_rx_buff->index < USART_BUFF_LENGHT)
			{			
				usart2_rx_buff->pdata[usart2_rx_buff->index++] = ch;;

			}
			else
			{
				memset(usart2_rx_buff, 0, sizeof(usart_buff_t));	//��������			
			}
		}

	}
	
	if(USART_GetITStatus(USART2, USART_IT_TXE) != RESET)                  
  	{ 
     	USART_ITConfig(USART2, USART_IT_TXE, DISABLE);					   
  	}	
	 	
}


void usart2_recv_data(void)
{		
	if(timer_is_timeout_1ms(timer_uart2, 40) == 0)	//40msû���յ�������Ϊ�����������		
	{
		
//		USART_OUT(USART1, usart2_buff);
//		memcpy(protocol_buff, usart2_buff, 512);	
		memcpy(gprs_buff, usart2_rx_buff->pdata, 512);
		
		memset(usart2_rx_buff, 0, sizeof(usart_buff_t));
		
	}	
}



/*
*********************************************************************************************************
*                                          USART3_IRQHandler()
*
* Description : Create application kernel objects tasks.
*
* Argument(s) : none
*
* Return(s)   : none
*
* Caller(s)   : AppTaskStart()
*
* Note(s)     : none.
*********************************************************************************************************
*/
void USART3_IRQHandler(void)
{
	uint8_t ch = 0;	

   	if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
    {   
	    USART_ClearITPendingBit(USART3, USART_IT_RXNE);	
		timer_is_timeout_1ms(timer_uart3, 0);
		
//		if(usart3_rx_status == 0)
		{	
			ch = USART_ReceiveData(USART3);	 

			if (usart3_cnt < USART_BUFF_LENGHT)
			{
				usart3_buff[usart3_cnt++] = ch;	
				usart3_rx_status = 1;				
			}
			else
			{
				memset(usart3_buff, 0, USART_BUFF_LENGHT);
				usart3_cnt = 0;
			}
		}	
	}
	
	if(USART_GetITStatus(USART3, USART_IT_TXE) != RESET)                  
  	{ 
     	USART_ITConfig(USART3, USART_IT_TXE, DISABLE);					   
  	}	
	
}

/*
*********************************************************************************************************
*                                          usart3_recv_data()
*
* Description : Create application kernel objects tasks.
*
* Argument(s) : none
*
* Return(s)   : none
*
* Caller(s)   : AppTaskStart()
*
* Note(s)     : none.
*********************************************************************************************************
*/
void usart3_recv_data(void)
{
	
	if(timer_is_timeout_1ms(timer_uart3, 50)==0)	//40msû���յ�������Ϊ�����������		
	{

		USART_OUT(USART1, usart3_buff);
//		memcpy(gps_buff, usart3_buff, 512);
		
		memset(usart3_buff, 0, 512);	
		usart3_cnt = 0;	
	}	
}







/*
*********************************************************************************************************
*                                          USART_OUT()
*
* Description : Create application kernel objects tasks.
*
* Argument(s) : none
*
* Return(s)   : none
*
* Caller(s)   : AppTaskStart()
*
* Note(s)     : ��������ϸ���ֲ��ʱ���� TC �� TXE ��־λ�ڸ�λ��ʱ����1 ��������һ��whileѭ������û���õġ������������״ε�һ���ַ���û�б������
*               �ͱ�������ַ����ǵ������ʵ�ʿ����Ķ�ʧ���󡣽���취�ͺܼ򵥣���ǰ�����һ�� USART1->SR
*********************************************************************************************************
*/
void USART_OUT(USART_TypeDef* USARTx, uint8_t *Data,...)
{ 
	const char *s;
    int d;  
    char buf[32];
    
	va_list ap;
    __va_start(ap, Data);
	
	
	USART_GetFlagStatus(USARTx, USART_FLAG_TC);	//
	while(*Data != '\0')
	{				                         
		if(*Data==0x5c)
		{									
			switch (*++Data)
			{
				case 'r':							          
					USART_SendData(USARTx, 0x0d);	   
					Data++;
				break;
				case 'n':							          
					USART_SendData(USARTx, 0x0a);	
					Data++;
				break;
				
				default:
					Data++;
			    break;
			}						 
		}
		else if(*Data=='%')
		{									  //
			switch (*++Data){				
				case 's':										  
                	s = __va_arg(ap, const char *);
                	for ( ; *s; s++) 
					{
                    	USART_SendData(USARTx,*s);
						while(USART_GetFlagStatus(USARTx, USART_FLAG_TC)==RESET);
                	}
					Data++;
                	break;
            	case 'd':										 
                	d = __va_arg(ap, int);
					
					sprintf(buf, "%d", d);
                	for (s = buf; *s; s++) 
					{
                    	USART_SendData(USARTx,*s);
						while(USART_GetFlagStatus(USARTx, USART_FLAG_TC)==RESET);
                	}
					Data++;
                	break;
				default:
					Data++;
				    break;
			}		 
		}
		else 
			USART_SendData(USARTx, *Data++);
		while(USART_GetFlagStatus(USARTx, USART_FLAG_TC)==RESET);
	}
}




void usart_send(USART_TypeDef* USARTx, uint8_t *data, uint16_t data_size,...)
{ 
	const char *s;
    int d;  
    char buf[32];
    
	va_list ap;
    __va_start(ap, data);
	
	
	USART_GetFlagStatus(USARTx, USART_FLAG_TC);	//
	while(data_size--)
	{				                         
		if(*data==0x5c)
		{									
			switch (*++data)
			{
				case 'r':							          
					USART_SendData(USARTx, 0x0d);	   
					data++;
				break;
				case 'n':							          
					USART_SendData(USARTx, 0x0a);	
					data++;
				break;
				
				default:
					data++;
			    break;
			}						 
		}
		else if(*data=='%')
		{									  //
			switch (*++data){				
				case 's':										  
                	s = __va_arg(ap, const char *);
                	for ( ; *s; s++) 
					{
                    	USART_SendData(USARTx,*s);
						while(USART_GetFlagStatus(USARTx, USART_FLAG_TC)==RESET);
                	}
					data++;
                	break;
            	case 'd':										 
                	d = __va_arg(ap, int);
					
					sprintf(buf, "%d", d);
                	for (s = buf; *s; s++) 
					{
                    	USART_SendData(USARTx,*s);
						while(USART_GetFlagStatus(USARTx, USART_FLAG_TC)==RESET);
                	}
					data++;
                	break;
				default:
					data++;
				    break;
			}		 
		}
		else 
			USART_SendData(USARTx, *data++);
		while(USART_GetFlagStatus(USARTx, USART_FLAG_TC)==RESET);
	}
}














