

#include <stdio.h>
#include <string.h>
#include "protocol.h"
#include "usart.h"
#include "common.h"
#include "timer.h"
#include "transport.h"
#include "aes.h"
#include "motor.h"



extern uint8_t lock_id[17];
extern uint8_t receiveText[24];
extern uint8_t expressText[128];  
extern uint8_t cipherText[128];
extern uint8_t aesKey[16];

extern uint8_t lock_open_err_flag;
extern uint8_t lock_close_err_flag;
extern uint8_t lock_status;
extern uint8_t lock_open_time_flag;
extern uint8_t lock_close_time_flag;

extern uint8_t lock_run_status;

//����
extern uint8_t lock_bell_flag;

//��
extern uint8_t lock_shake_flag;


void protocol_analyze(void)
{
	uint8_t local_topic[50] = {0};
	uint8_t recv_topic[50] = {0};
	uint8_t payload[100];
	int payloadlen = 0;
	int mqtt_sub;
	
	mqtt_sub = mqtt_subscribe(recv_topic, payload, &payloadlen);
	if(mqtt_sub == 1)
	{
//		timer_is_timeout_1ms(timer_heartbeat, 0);
		USART_OUT(USART1, "AAAA=%s=%s\r\n", payload, recv_topic);
		
		sprintf((char*)local_topic, "%s%s", "lock/", lock_id);
		if(strncmp((char*)recv_topic, (char*)"lock/", 5)==0)
		{	
			USART_OUT(USART1, "topic==%s\r\n", recv_topic);
					
			timer_is_timeout_1ms(timer_heartbeat, 0);

			memset(receiveText , 0, 24);
			memset(expressText , 0, 128);
			
			strncpy((char*)receiveText, (char*)payload, payloadlen);
			AES_Decrypt(expressText, receiveText, aesKey);
			
			USART_OUT(USART1, "receiveText=%s\r\n", receiveText);
			USART_OUT(USART1, "expressText=%s\r\n", expressText);	
			if(*expressText == 0x31)
			{
//				timer_is_timeout_1ms(timer_open_lock, 0);
				lock_shake_flag = 1;
				lock_status = 1;
				lock_open_time_flag = 0;
				lock_run_status = 0;
				USART_OUT(USART1, "Lock_Open11111\r\n");
			
			}
			else if(*expressText == 0x32)
			{
//				timer_is_timeout_1ms(timer_close_lock, 0);
				lock_shake_flag = 1;
				lock_status = 0;
				lock_run_status = 0;
				lock_close_time_flag = 0;
				USART_OUT(USART1, "Lock_Close11111\r\n");
			}
			else if(*expressText == 0x30)
			{
				motor_stop();	//ֹͣ����;
			}
								
			payloadlen = 0;
			memset(recv_topic, 0, 50);
			memset(payload, 0, 100);	
		}
		
		sprintf((char*)local_topic, "%s%s", "lock/", lock_id);
		if(strncmp((char*)recv_topic,(char *)"bell/", 5)==0)
		{
			timer_is_timeout_1ms(timer_heartbeat, 0);
			USART_OUT(USART1, "bell===========================================================================\r\n");

			lock_bell_flag = 1;
			
			payloadlen = 0;
			memset(recv_topic, 0, 50);
			memset(payload, 0, 100);
		}

	}

}






















