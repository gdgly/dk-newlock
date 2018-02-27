#ifndef _GU906_H_
#define _GU906_H_
//#include "sys.h"
#include "bsp.h"  


#define GU906GSM_EN   1    //�Ƿ������Ź���
#define GPRSCSQ       18   //�ź�ǿ�ȣ���ʹ��GPRS����ʱ�����Ҫ���ź�ǿ�Ȳ�����18

#define _ATOK          0  //ִ�гɹ�
#define _ATERROR      -1  //ִ�д���
#define _ATOTIME      -2  //ִ�г�ʱ
#define _LINKNOT      -3  //������

struct Gprs_Config{
	uint8_t *server_ip;     //������ip
	uint32_t server_port;   //�������˿ں�
};

#if GU906GSM_EN
//����ʵ���ڴ��������
struct user_simdata{
	char phone[15];  //�û��ֻ���
	char dev[50];    //�û�ʹ�õ��豸
	char date[50];   //����ʱ��
	char data[200];  //���յ�����
};
extern struct user_simdata sim;
s8 GU906_Read_UserSMS(void);
s8 GU906_Chinese_text(char *phone,char* pmsg);
#endif

s8  GU906_init(void);
s8  GU906_Module_State(void);
s8  GU906_TCP_Socket(struct Gprs_Config *GprsCon);
s8  GU906_DTU_Socket(struct Gprs_Config *GprsCon);
s8  GU906_GPRS_write(char* pdat, int len);
uint32_t GU906_GPRS_read(char *pout, int len);

s8  GU906_make_phone(char *phone);
s8  GU906_Answer_Phone(uint32_t Delay);
s8  GU906_end_phone(void);
s8  GU906_DtuOrAT(uint8_t type);


#endif