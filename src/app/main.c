


#include <string.h>		
#include <stdlib.h>
#include "adc.h"
#include "bsp.h"
#include "timer.h"
#include "gprs.h"
#include "usart.h"
#include "aes.h"
#include "aes128.h"
#include "app_md5.h"
#include "transport.h"
#include "button.h"
#include "usart.h"
#include "flash.h"
#include "motor.h"
#include "main.h"
#include "lock.h"

extern usart_buff_t mqtt_buff;
extern uint16_t mqtt_publist_msgid;
extern uint8_t gprs_status;
extern uint8_t usart2_buff[512];
extern u16 usart2_cnt; 

//����
extern uint8_t bell_flag;

//��
extern uint8_t shake_flag;


extern uint8_t lock_open_err_flag;
extern uint8_t lock_close_err_flag;
extern uint8_t Lock_Open;
extern uint8_t Lock_Close;



uint8_t receiveText[24];
uint8_t expressText[512];  
uint8_t cipherText[512];
uint8_t aesKey[16];



//��ѹ
u16 Bat_V;
u16 Bat_Pre;


uint8_t *p1;
uint8_t *p2;



uint8_t protocol_buff[512] = {0};


uint8_t mqtt_keep_alive_flag = 0;
uint8_t mqtt_keep_alive_err_cnt = 0;


uint8_t lock_id[17] = {0};
uint8_t topic_buff[100] = {0};
uint8_t send_buff[100] = {0};




void heartbeat(uint32_t ms)
{
	int mqtt_pub = 0;
	u8 heartbeat_buff[2] = {0};
	
	if(timer_is_timeout_1ms(timer_heartbeat, ms) == 0)
	{
		memset(topic_buff, 0 ,100);
		memset(heartbeat_buff, 0, 2);
		
		sprintf((char *)topic_buff,"%s%s","lockdata/", lock_id);
		heartbeat_buff[0] = 0x30;
		heartbeat_buff[0] = '\0';
		
		USART_OUT(USART1, "heartbeat=%s\r\n", topic_buff);
		mqtt_pub = mqtt_publist(topic_buff, heartbeat_buff, 1, 2, mqtt_publist_msgid);
		if(mqtt_pub == 1)
		{
			USART_OUT(USART1, "heartbeat mqtt_publist ok\r\n");
		}	
		else
		{
			USART_OUT(USART1, "heartbeat mqtt_publist error\r\n");
		}
	}
}

void dev_to_srv_batt_voltage(uint32_t ms)
{
	int mqtt_pub;
	
	if(timer_is_timeout_1ms(timer_batt, ms) == 0)
	{
		Bat_V = adc_get_average(ADC_Channel_0, 10);
		Bat_V = Bat_V*3300/4096;
		Bat_V = Bat_V*88/20;
		Bat_Pre = (Bat_V-5000)*100/2400;
		USART_OUT(USART1, "Bat_Pre=%s\r\n", Bat_Pre);
		
		memset(topic_buff, 0, 100);	
		memset(expressText, 0, 512);
		memset(cipherText, 0 ,512);	
		
		sprintf((char *)topic_buff, "%s%s", "lockdata/", (char*)lock_id);	
		sprintf((char *)expressText, "{%c%s%c:%s}",'"',"battery",'"',"20");
		AES_Encrypt((char *)expressText, (char*)cipherText, (char*)aesKey);
		
		USART_OUT(USART1, "expressText=%s\r\n", expressText);
		USART_OUT(USART1, "aesKey=%s\r\n", aesKey);
		USART_OUT(USART1, "cipherText=%s\r\n", cipherText);
		
		mqtt_pub = mqtt_publist(topic_buff, expressText, 24, 2, mqtt_publist_msgid);
		if(mqtt_pub == 1)
		{
			USART_OUT(USART1, "dev_to_srv_batt mqtt_publist ok\r\n");
		}	
	}
}





static void test_encrypt_ecb(void)
{

    uint8_t key[] = { 0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c };
    uint8_t out[] = { 0x3a, 0xd7, 0x7b, 0xb4, 0x0d, 0x7a, 0x36, 0x60, 0xa8, 0x9e, 0xca, 0xf3, 0x24, 0x66, 0xef, 0x97 };


    uint8_t in[]  = { 0x6b, 0xc1, 0xbe, 0xe2, 0x2e, 0x40, 0x9f, 0x96, 0xe9, 0x3d, 0x7e, 0x11, 0x73, 0x93, 0x17, 0x2a };
   	struct AES_ctx ctx;
	
	
	sprintf((char *)expressText,"{%c%s%c:%s}",'"',"battery",'"',"20");
//	expressText[0] = 0x31;
	USART_OUT(USART1, "expressText66=%s\r\n", expressText);
	

	
//	USART_OUT(USART1, "in=%s\r\n", in);
//	USART_OUT(USART1, "out=%s\r\n", out);
//	AES_init_ctx(&ctx, key);
//	AES_ECB_encrypt(&ctx, in);
//	USART_OUT(USART1, "in=%s\r\n", in);
//	USART_OUT(USART1, "out=%s\r\n", out);
//    AES_init_ctx(&ctx, aesKey);
//    AES_ECB_encrypt(&ctx, expressText);

	USART_OUT(USART1, expressText);

    if (0 == memcmp((char*) out, (char*) in, 16))
    {
        USART_OUT(USART1, "SUCCESS!\n");
    }
    else
    {
        USART_OUT(USART1, "FAILURE!\n");
    }
}



int main(void)
{ 
	int rc = 0;

	uint8_t ret = 0;
	uint8_t ds_val = 0;
	uint8_t topic[50] = {0};
	uint8_t payload[100];
	int payloadlen = 0;
	int mqtt_pub;
	int mqtt_sub;
	GPRS_CONFIG gprs_info = GPRS_CONFIG_INIT;
	
	
	gprs_info.server_ip = "118.31.69.148";
	gprs_info.server_port = 1883;
	
	
	bsp_init();
                         
	USART_OUT(USART1, "uart1 is ok\r\n");
	
	USART_OUT(USART1, "server_port=%d\r\n", gprs_info.server_port);
	
	USART_OUT(USART1, "server_ip=%s\r\n", gprs_info.server_ip);
	
	while(1)
	{	
		ds_val = button_ds_get_value();
	
		if(ds_val == 0)
		{
			
			eeprom_read_data(EEPROM_LOCK_ID_ADDR, lock_id, 16);
			MakeFile_MD5_Checksum(lock_id, 16);
			USART_OUT(USART1, "lock_id=%s\r\n", lock_id);
			break;
		}
		else if(ds_val == 1)
		{
			
		}
		else if(ds_val == 2)
		{
		
		}
		else if(ds_val == 3)
		{
			usart1_recv_data();
			p1 = strstr((u8*)protocol_buff, "lockid=");
			if(p1 != NULL)
			{
				memcpy((char*)lock_id ,(char *)(p1+7), 16);		
				eeprom_write_data(EEPROM_LOCK_ID_ADDR, lock_id, 16);
				eeprom_read_data(EEPROM_LOCK_ID_ADDR, lock_id, 16);
				USART_OUT(USART1, "lock_id=%s\r\n", lock_id);
				break;
			}		
		}
	}
	
	while(1)
	{
		
		gprs_init_task(&gprs_info);

		usart1_recv_data();
		usart2_recv_data();
		
		mqtt_sub = mqtt_subscribe(topic, payload, &payloadlen);
		if(mqtt_sub == 1)
		{
			timer_is_timeout_1ms(timer_heartbeat, 0);
			USART_OUT(USART1, "AAAA=%s=%s\r\n", payload, topic);
			
		}
		
		if(strncmp((char*)topic, (char*)"lock/", 5)==0)
		{	
			USART_OUT(USART1, "topic==%s\r\n", topic);
					
			timer_is_timeout_1ms(timer_heartbeat, 0);

			memset(receiveText , 0, 24);
			memset(expressText , 0, 512);
			
			strncpy((char*)receiveText, (char*)payload, payloadlen);
			AES_Decrypt(expressText, receiveText, aesKey);
			
			USART_OUT(USART1, "receiveText=%s\r\n", receiveText);
			USART_OUT(USART1, "expressText=%s\r\n", expressText);	
			if(*expressText == 0x31)
			{
				timer_is_timeout_1ms(timer_open_lock, 0);
				shake_flag = 1;
				Lock_Open = 1;
				USART_OUT(USART1, "Lock_Open11111\r\n");
			
			}
			else if(*expressText == 0x32)
			{
				timer_is_timeout_1ms(timer_close_lock, 0);
				shake_flag = 1;
				Lock_Close = 1;
				USART_OUT(USART1, "Lock_Close11111\r\n");
			}
			else if(*expressText == 0x30)
			{
				motor_stop();	//ֹͣ����;
			}
								
			payloadlen = 0;
			memset(topic, 0, 50);
			memset(payload, 0, 100);	
		}
		
	
		if(strncmp((char*)topic,(char *)"bell/", 5)==0)
		{
			timer_is_timeout_1ms(timer_heartbeat, 0);
			USART_OUT(USART1, "bell\r\n");

			BEEP_ON();
			timer_delay_1ms(100);
			BEEP_OFF();
			
			payloadlen = 0;
			memset(topic, 0, 50);
			memset(payload, 0, 100);
		}
				
		//��������
		if((timer_is_timeout_1ms(timer_mqtt_keep_alive, 1000*120) == 0) || (mqtt_keep_alive_flag == 1))
		{
			USART_OUT(USART1, "mqtt_keep_alive\r\n");
			rc = mqtt_keep_alive(1);
			if(rc == 1)
			{
				mqtt_keep_alive_flag = 0;
				mqtt_keep_alive_err_cnt = 0;
				USART_OUT(USART1, "mqtt_keep_alive ok\r\n");
			}  
			else
			{
				mqtt_keep_alive_flag = 1;
				mqtt_keep_alive_err_cnt++;
				if(mqtt_keep_alive_err_cnt > 5)
				{
					gprs_status = 0;
					mqtt_keep_alive_flag = 0;
					mqtt_keep_alive_err_cnt = 0;
					
					USART_OUT(USART1, "mqtt_keep_aliv error\r\n");
				}
			}
		}
		
		
		lock_open_deal();
		lock_close_deal();
		
		lock_hand_close();
	
		dev_to_srv_batt_voltage(1000*60*60);	

		heartbeat(1000*60*5);
//		lock_shake_alarm();
		
	}

}
 





















