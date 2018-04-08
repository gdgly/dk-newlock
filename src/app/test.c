




#include <stdarg.h>
#include <stdio.h>
#include <string.h>		
#include <stdlib.h>
#include "test.h"
#include "usart.h"
#include "timer.h"
#include "lock.h"
#include "gprs.h"



extern uint8_t protocol_buff[512];
extern uint8_t lock_self_status;
extern uint8_t lock_run_status;
extern uint8_t shake_flag;
extern uint8_t lock_open_time_flag;
extern uint8_t lock_close_time_flag;
extern void test_gprs(void);


void test_dev(void)
{
	uint8_t *p;
	uint8_t *ret;
	int tt =0;
	usart1_recv_data();
	
	
//	p = strstr((uint8_t*)protocol_buff, "self_checking=");
//	if(p != NULL)
//	{
//		memcpy(&lock_self_status, (char*)(p+sizeof("self_checking=")-1), 1);
//		lock_self_status = atoi(&lock_self_status);
//		lock_run_status = 0;
//		timer_is_timeout_1ms(timer_open_lock, 0);
//		USART_OUT(USART1, "lock_status=%s\r\n", &lock_self_status);
//		memset(protocol_buff, 0, 512);
//	}
//	lock_self_checking();
//	p = strstr((uint8_t*)protocol_buff, "open_lock=");
//	if(p != NULL)
//	{
//		memcpy(&lock_self_status, (char*)(p+sizeof("open_lock=")-1), 1);
//		lock_self_status = atoi(&lock_self_status);
//		timer_is_timeout_1ms(timer_open_lock, 0);

//		shake_flag = 1;
//		lock_open_time_flag = 0;
//		lock_close_time_flag = 0;
//		USART_OUT(USART1, "lock_status=%s\r\n", &lock_self_status);
//		memset(protocol_buff, 0, 512);
//	}

//	lock_close_deal();
//	lock_open_deal();

	p = strstr((uint8_t*)protocol_buff, "CSQ=1");
	if(p != NULL)
	{
		ret = gprs_send_at("AT+CSQ\r\n", "OK", 500, 10000);
		if (ret != NULL)
		{
			
		}
	}
	
	p = strstr((uint8_t*)protocol_buff, "gprs_open=1");
	if(p != NULL)
	{
		test_gprs();
	}
	
}




void test_gprs(void)
{
	int mqtt_rc = 0;

	uint8_t *ret;
	uint8_t buff[100] = {0};
	uint8_t cipstart[100] = {0};
	int gprs_status_test = 0;
	
	while(1)
	{
		switch(gprs_status_test)
		{
			case 0:
				gprs_power_on();
				
				USART_OUT(USART1, "gprs_power_on\r\n");
				gprs_status_test = 1;
	
				gprs_status_test++;
			break;
					
			case 1:
				ret = gprs_send_at("AT\r\n", "OK", 300,10000);
				if (ret != NULL)
				{
					gprs_status_test++;
				}
				
			break;
			
			case 2:
				ret = gprs_send_at("ATE0\r\n", "OK", 500,10000);
				if (ret != NULL)
				{
					gprs_status_test++;
				}
			
			break;
				
			case 3:
				ret = gprs_send_at("AT+CGSN\r\n", "OK", 500, 10000);
				if (ret != NULL)
				{
					gprs_status_test++;
				}
			break;
			
			case 4:			
				ret = gprs_send_at("AT+CPIN?\r\n", "OK", 500, 10000);
				if (ret != NULL)
				{
					gprs_status_test++;
				}
			
			break;
				
			case 5:
				ret = gprs_send_at("AT+CSQ\r\n", "OK", 500, 10000);
				if (ret != NULL)
				{
					
				}
			break;
		
		
			case 255:			
				
			
			break;
		
		}
	}	
		
}


