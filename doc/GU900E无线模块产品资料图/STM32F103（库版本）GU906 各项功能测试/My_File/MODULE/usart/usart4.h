#ifndef __USART4_H_
#define __USART4_H_
#include "sys.h"

#define USART4_BUFF     300  //�������ݻ���
#define USART4_485_EN   0    //1 ����485���ܣ�0 �ر�485����

void usart4_Configuration(u32 bound);
void usart4_Send(char *buf,int len); 
int  usart4_Receive(char *buf, int buflen);
void usart4_FreeBuff(void);

#endif


