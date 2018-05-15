#ifndef __USART3_H_
#define __USART3_H_
#include "sys.h"

#define USART3_BUFF     200  //�������ݻ���
#define USART3_485_EN   0    //1 ����485���ܣ�0 �ر�485����
#define USART3_MAP_EN   0    //1 �����ض����ܣ�0�ر��ض�����

void usart3_Configuration(u32 bound);
void usart3_Send(char *buf,int len); 
int  usart3_Receive(char *buf, int buflen);
void usart3_FreeBuff(void);

#endif


