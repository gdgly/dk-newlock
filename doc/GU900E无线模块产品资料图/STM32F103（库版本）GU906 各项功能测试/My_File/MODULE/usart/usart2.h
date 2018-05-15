#ifndef __USART2_H_
#define __USART2_H_
#include "sys.h"

#define USART2_BUFF     200  //�������ݻ���
#define USART2_485_EN   0    //1 ����485���ܣ�0 �ر�485����
#define USART2_MAP_EN   1    //1 �����ض����ܣ�0�ر��ض�����

void usart2_Configuration(u32 bound);
void usart2_Send(char *buf,int len); 
int  usart2_Receive(char *buf, int buflen);
void usart2_FreeBuff(void);

#endif


