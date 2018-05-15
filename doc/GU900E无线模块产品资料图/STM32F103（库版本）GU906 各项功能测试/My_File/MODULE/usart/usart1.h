#ifndef __USART1_H_
#define __USART1_H_
#include "sys.h"

#define USART1_BUFF     200  //�������ݻ���
#define USART1_485_EN   0    //1 ����485���ܣ�0 �ر�485����
#define USART1_MAP_EN   0    //1 �����ض����ܣ�0�ر��ض�����

void usart1_Configuration(u32 bound);
void usart1_Send(char *buf,int len); 
int  usart1_Receive(char *buf, int buflen);
void usart1_FreeBuff(void);

#endif


