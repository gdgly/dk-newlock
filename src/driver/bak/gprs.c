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
    2. ...
*************************************************/

/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/
#include <stdio.h>
#include <string.h>
#include "gprs.h"
#include "bsp.h"
#include "usart.h"
#include "timer.h"




extern usart_buff_t  *usart2_rx_buff;


extern uint8_t usart2_buff[512];
extern uint16_t usart2_cnt;

extern uint8_t usart2_rx_status;





uint8_t gprs_err_cnt = 0; 		//GPRS错误计数器
uint8_t gprs_status = 0;		//GPRS的状态

uint8_t gprs_rx_buf[512];
uint16_t gprs_rx_cnt = 0;

uint8_t gprs_send_at_flag = 0;	
uint8_t gprs_rx_flag = 0;



uint8_t lock_id[17] = {0};

uint8_t topic_id = 0;







void gprs_gpio_init(void)
{
	GPIO_InitTypeDef gpio_init_structure;
	
	gpio_init_structure.GPIO_Pin = GPIO_Pin_6;
  	gpio_init_structure.GPIO_Speed = GPIO_Speed_10MHz;
	gpio_init_structure.GPIO_Mode = GPIO_Mode_OUT;          
  	GPIO_Init(GPIOA, &gpio_init_structure);
	
	gpio_init_structure.GPIO_Pin = GPIO_Pin_7;
  	gpio_init_structure.GPIO_Speed = GPIO_Speed_10MHz;
	gpio_init_structure.GPIO_Mode = GPIO_Mode_OUT;          
  	GPIO_Init(GPIOA, &gpio_init_structure);

}


/*
*********************************************************************************************************
*                                          gprs_power_on()
*
* Description : Create application tasks.
*
* Argument(s) : none
*
* Return(s)   : none
*
* Caller(s)   : gprs_init_task_fun()
*
* Note(s)     : none.
*********************************************************************************************************
*/
void gprs_power_on(void)
{

	GPIO_SetBits(GPIOA, GPIO_Pin_6);				//GPRS_PW
	
	GPIO_ResetBits(GPIOA, GPIO_Pin_7);				//GPRS_PW
	timer_delay_1ms(1200);
}






/*
*********************************************************************************************************
*                                          gprs_check_cmd()
*
* Description : 检测字符串中是否存在子字符串.
*
* Argument(s) : p_str ： 要检测的子字符串
*
* Return(s)   : 返回子字符串的位置指针
*
* Caller(s)   : gprs_send_at()
*
* Note(s)     : 
*********************************************************************************************************
*/
uint8_t *gprs_check_cmd(uint8_t *p_str)
{
	char *str = NULL;
	
	str = strstr((const char*)usart2_buff, (const char*)p_str);

	return (uint8_t*)str;
}


/*
*********************************************************************************************************
*                                          gprs_send_at()
*
* Description : Create application tasks.
*
* Argument(s) : none
*
* Return(s)   : none
*
* Caller(s)   : gprs_init_task_fun()
*
* Note(s)     : none.
*********************************************************************************************************
*/
uint8_t* gprs_send_at(uint8_t *cmd, uint8_t *ack, uint16_t waittime, uint16_t timeout)
{
	uint8_t res = 1;
	uint8_t buff[512] = {0};
	
	timer_is_timeout_1ms(timer_at, 0);	//开始定时器timer_at
	while (res)
	{	
		memset(usart2_buff, 0, 512);
		usart2_cnt = 0;
		
		usart2_rx_status = 0;
		USART_OUT(USART2, cmd);							
		timer_delay_1ms(waittime);				//AT指令延时
	
		usart2_rx_status = 1;	//数据未处理 不在接收数据
		USART_OUT(USART1, "usart2_buff=%s", usart2_buff);

		if (gprs_check_cmd(ack))	
		{
			res = 0;				//监测到正确的应答数据
			usart2_rx_status = 0;	//数据处理完 开始接收数据
			memcpy(buff, usart2_buff, 512);
			USART_OUT(USART1,  buff);
			memset(usart2_buff, 0, 512); 	//清理usart接收缓冲区
			usart2_cnt = 0;		
			return buff;
		}
			
		if (timer_is_timeout_1ms(timer_at, timeout) == 0)	//定时器timer_at结束
		{
			res = 0;
			usart2_rx_status = 0;	//数据处理完 开始接收数据
			return NULL;
		}
	}	
}





void gprs_config(void)
{
	uint8_t *ret;
	uint8_t buff[512] = {0};
	char *str = 0;
	switch(gprs_status)
	{
		
		case 0:
			gprs_power_on();
			timer_delay_1ms(1000);
			gprs_status++;
		break;
		
		case 1:
			ret = gprs_send_at("AT\r\n", "OK", 800,10000);
			if (ret != NULL)
			{
				gprs_status++;
				gprs_err_cnt = 0;
			}
			else
			{
				gprs_err_cnt++;
				if (gprs_err_cnt > 5)
				{
					gprs_status = 0;
				}
			}
		break;
			
		case 2:
			ret = gprs_send_at("AT+STATUS\r\n", "OK", 800,10000);
			if (ret != NULL)
			{
				if(strstr((char*)ret, "MQTT READY") != NULL)
				{
					gprs_status++;
					gprs_err_cnt = 0;
					USART_OUT(USART1, "STATUS=%d\r\n", gprs_status);
				}
				else if(strstr((char*)ret, "MQTT CONNECT OK") != NULL)
				{
					gprs_status = 4;
					gprs_err_cnt = 0;
				}
				else if(strstr((char*)ret, "MQTT CLOSE") != NULL)
				{
					gprs_status = 0;
				}
			}
			else
			{
				gprs_err_cnt++;
				if (gprs_err_cnt > 5)
				{
					gprs_status = 0;
				}
			}
		break;	
		
		case 3:
			ret = gprs_send_at("AT+START\r\n", "OK", 800,5000);
			if (ret != NULL)
			{
				gprs_status++;
				gprs_err_cnt = 0;
			}
			else
			{
				gprs_err_cnt++;
				if (gprs_err_cnt > 5)
				{
					gprs_status = 0;
				}
			}
		break;	
		
			
		case 4:
			ret = gprs_send_at("AT+STATUS\r\n", "MQTT CONNECT OK", 800,5000);
			if (ret != NULL)
			{
				gprs_status++;
				gprs_err_cnt = 0;
			}
			else
			{
				gprs_err_cnt++;
				if (gprs_err_cnt > 5)
				{
					gprs_status = 0;
				}
			}
		break;		
		
		case 5:
			ret = gprs_send_at("AT+CLIENTID?\r\n", "OK", 50, 1000);
			if (ret != NULL)
			{
				USART_OUT(USART1, ret);
				str = strstr((const char*)ret, (const char*)"+CLIENTID: ");
				memcpy(lock_id, str+12, 16);	//读出锁id	
				USART_OUT(USART1, "lock_id=%s\r\n", lock_id);
				gprs_status++;
				gprs_err_cnt = 0;
			}
			else
			{
				gprs_err_cnt++;
				if (gprs_err_cnt > 5)
				{
					gprs_status = 0;
				}
			}
		break;
			
		case 6:
			ret = gprs_send_at("AT+STATUS\r\n", "MQTT CONNECT OK", 800,5000);
			if (ret != NULL)
			{
				gprs_status++;
				gprs_err_cnt = 0;
			}
			else
			{
				gprs_err_cnt++;
				if (gprs_err_cnt > 5)
				{
					gprs_status = 0;
				}
			}
		break;
			
		case 7:
			sprintf((char*)buff, "%s%s%s", "AT+SUBSCRIBE=bell/", lock_id, ",2\r\n");
			USART_OUT(USART1, "lock_id=%s\r\n", buff);
			ret = gprs_send_at(buff, "OK", 800, 2000);
			if (ret != NULL)
			{
				gprs_status++;
				gprs_err_cnt = 0;
			}
			else
			{
				gprs_err_cnt++;
				if (gprs_err_cnt > 5)
				{
					gprs_status = 0;
				}
			}
		break;
			
		case 8:
			sprintf((char*)buff, "%s%s%s", "AT+SUBSCRIBE=lock/", lock_id, ",2\r\n");
			USART_OUT(USART1, "lock=%s\r\n", buff);
			ret = gprs_send_at(buff, "OK", 800, 2000);
			if (ret != NULL)
			{
				gprs_status++;
				gprs_err_cnt = 0;
			}
			else
			{
				gprs_err_cnt++;
				if (gprs_err_cnt > 5)
				{
					gprs_status = 0;
				}
			}
		break;
			
		case 9:
			gprs_status = 255;
		break;
			
		
		default:
		break;				
				
	}
	
	
}







/*
*********************************************************************************************************
*                                          gprs_init_task_fun()
*
* Description : GPRS初始化任务函数.
*
* Argument(s) : p_arg :该任务要传的参数
*
* Return(s)   : none
*
* Caller(s)   : 
*
* Note(s)     : none.
*********************************************************************************************************
*/
static void gprs_init_task_fun(void *p_arg)
{

	uint8_t *msg;
	uint8_t size1;
	uint8_t *ret;
	char *p;
	static uint8_t gprs_init_flag = true;		//
		
	while(1)
	{
		
		switch(gprs_status)
		{
			case 0:
				gprs_power_on();
				gprs_status = 1;
				gprs_err_cnt = 0;
			break;
					
			case 1:	//串口测试指令
				ret = gprs_send_at("\r\nAT\r\n", "OK", 800,10000);
				if (ret != NULL)
				{
					gprs_status++;
					gprs_err_cnt = 0;
				}
				else
				{
					gprs_err_cnt++;
					if (gprs_err_cnt > 5)
					{
						gprs_status = 0;
					}
				}
			break;
			
			case 2://查询卡是否存在
				ret = gprs_send_at("\r\AT+ESIMS?\r\n\r\n", "OK", 800, 10000);
				if (ret != NULL)
				{
					if((p = strstr(ret, "+ESIMS: ")) != 0)
					{
						p += 8;
						if(1 == (*p -'0'))
						{
							gprs_status++;
							gprs_err_cnt = 0;
						}
					}
				}
				else
				{
					gprs_err_cnt++;
					if (gprs_err_cnt > 5)
					{
						gprs_status = 0;
					}
				}
			break;
			
			case 3:	//查询PIN	
				ret = gprs_send_at("\r\nAT+CPIN?\r\n", "OK", 800, 10000);
				if (ret != NULL)
				{
					gprs_status++;
					gprs_err_cnt = 0;
				}
				else
				{
					gprs_err_cnt++;
					if (gprs_err_cnt > 5)
					{
						gprs_status = 0;
					}
				}
			break;
			
			case 4://查询信号质量
				ret = gprs_send_at("\r\nAT+CSQ\r\n", "OK", 800, 10000);
				if (ret == 0)
				{
					gprs_status++;
					gprs_err_cnt = 0;
				}
				else
				{
					if (gprs_err_cnt > 5)
					{
						gprs_status = 0;
					}
				}
			break;
			
			case 5://设置网络注册信息
				gprs_status++;
//				ret = gprs_send_at("\r\nAT+CREG=1\r\n", "OK", 800, 10000);
//				if (ret != NULL)
//				{
//					gprs_status++;
//					gprs_err_cnt = 0;
//				}
//				else
//				{
//					gprs_err_cnt++;
//					if (gprs_err_cnt > 5)
//					{
//						gprs_status = 0;
//					}
//				}
			break;
			
			case 6://查询网络注册信息
				ret = gprs_send_at("\r\nAT+CREG?\r\n", "OK", 800, 10000);
				if (ret != NULL)
				{
					gprs_status++;
					gprs_err_cnt = 0;
				}
				else
				{
					gprs_err_cnt++;
					if (gprs_err_cnt > 5)
					{
						gprs_status = 0;
					}
				}
			break;
			
			case 7:	//激活无线网络
				ret = gprs_send_at("\r\nAT+CIICR\r\n\r\n", "OK", 800, 10000);//
				if (ret == 0)
				{
					gprs_status++;
					gprs_err_cnt = 0;
				}
				else
				{
					gprs_err_cnt++;
					if (gprs_err_cnt > 5)
					{
						gprs_status = 0;
					}
				}
			break;

			case 8:	//透传参数设置
				ret = gprs_send_at("\r\nAT+CIPCFG=1,50,0\r\n", "OK", 800, 10000);//
				if (ret == 0)
				{
					gprs_status++;
					gprs_err_cnt = 0;
				}
				else
				{
					gprs_err_cnt++;
					if (gprs_err_cnt > 5)
					{
						gprs_status = 0;
					}
				}
			break;
				
			case 9:	//心跳包
				ret = gprs_send_at("\r\nAT+CIPPACK=1,\"4C4F47494E3A31303031\"\r\n", "OK", 800, 10000);//
				if (ret == 0)
				{
					gprs_status++;
					gprs_err_cnt = 0;
				}
				else
				{
					gprs_err_cnt++;
					if (gprs_err_cnt > 5)
					{
						gprs_status = 0;
					}
				}
			break;	
				
			case 10://注册包
				ret = gprs_send_at("\r\nAT+CIPPACK=0,\"0102A0\\r\n", "OK", 800, 10000);//
				if (ret == 0)
				{
					gprs_status++;
					gprs_err_cnt = 0;
				}
				else
				{
					gprs_err_cnt++;
					if (gprs_err_cnt > 5)
					{
						gprs_status = 0;
					}
				}
			break;
				
			case 11://账户配置
				ret = gprs_send_at("\r\AT+CSTT=\"CMNET\"\r\n", "OK", 800, 10000);//
				if (ret == 0)
				{
					gprs_status++;
					gprs_err_cnt = 0;
				}
				else
				{
					gprs_err_cnt++;
					if (gprs_err_cnt > 5)
					{
						gprs_status = 0;
					}
				}
			break;
				
			case 12:	//单链接
				ret = gprs_send_at("\r\nAT+CIPMUX=0\r\n", "OK", 800, 10000);//
				if (ret == 0)
				{
					gprs_status++;
					gprs_err_cnt = 0;
				}
				else
				{
					gprs_err_cnt++;
					if (gprs_err_cnt > 5)
					{
						gprs_status = 0;
					}
				}
			break;
			
			case 13:	//设置透传模式
				ret = gprs_send_at("\r\nAT+CIPMODE=1, 0\r\n", "OK", 800, 10000);//
				if (ret == 0)
				{
					gprs_status++;
					gprs_err_cnt = 0;
				}
				else
				{
					gprs_err_cnt++;
					if (gprs_err_cnt > 5)
					{
						gprs_status = 0;
					}
				}
			break;
			
			case 14: //
				ret = gprs_send_at("\r\nAT+CIPSTART=\"TCP\",\"118.31.69.148\",1883\r\n", "OK", 800, 10000);//
				if (ret == 0)
				{
					gprs_status = 255;
					gprs_err_cnt = 0;
				}
				else
				{ 
					gprs_err_cnt++;
					if (gprs_err_cnt > 5)
					{
						gprs_status = 0;
					}
				}
			break;
					
			
		
				
			case 255:	//gprs 初始化完成后进行数据传输
				
//				gprs_recv_task_create();
			
			break;
				
				
			default:
			break;		
		}	//switch end			
	} // while end
	
		
}


















