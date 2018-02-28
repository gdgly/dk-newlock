#ifndef _GU906_H_
#define _GU906_H_


#include "bsp.h"  
#include <stdbool.h>

#define GU906GSM_EN   0    //�Ƿ������Ź���
#define GPRSCSQ       18   //�ź�ǿ�ȣ���ʹ��GPRS����ʱ�����Ҫ���ź�ǿ�Ȳ�����18

#define _ATOK          0  //ִ�гɹ�
#define _ATERROR      -1  //ִ�д���
#define _ATOTIME      -2  //ִ�г�ʱ
#define _LINKNOT      -3  //������




#define DEBUG_EN  0



#define MAXRECVBUFF  		512

#define AT                  "AT\r\n"                                        //��������
#define ATE(x)              ((x)?("ATE1\r\n"):("ATE0\r\n"))                 //�����ԡ��ػ���
#define ATESIM              "AT+ESIMS?\r\n"                                 //��鿨�Ƿ����
#define ATCNMI              "AT+CNMI=2,1\r\n"                               //�������������������Ϣ�洢���� 

#define ATCMGD              "AT+CMGD=1,4\r\n"                               //ɾ����ǰ�洢���е���Ϣ
#define ATCMGF              "AT+CMGF=1\r\n"                                 //0���ö���ϢΪPDUģʽ��1���ö���ϢΪ

#define ATCSMP              "AT+CSMP=17,167,2,25\r\n"                       //�����ı�ģʽ�Ĳ���
#define ATUCS2              "AT+CSCS=\"UCS2\"\r\n"                          //����ΪUCS2�����ַ���
#define ATGB2312            "AT+CSCS=\"GB2312\"\r\n"                        //����ΪGB2312����
#define ATATD               "ATD%s;\r\n"                                    //��ָ���ֻ�����
#define ATATH               "ATH\r\n"                                       //�һ�
#define ATGSM               "AT+CSCS=\"GSM\"\r\n"                           //����GSM�ַ���  
#define ATCPMS              "AT+CPMS=\"SM\",\"SM\",\"SM\"\r\n"              //���ö��Ŵ洢��ԪΪSIM�� 
#define ATCSQ               "AT+CSQ\r\n"                                    //��ȡ�ź�ǿ��
#define ATCREG              "AT+CREG?\r\n"                                  //ȷ��ģ��ע�ᵽGSM����
#define ATCIICR             "AT+CIICR\r\n"                                  //��ģ�鼤��GPRS���磬����Ҫ�������� TCP ���ӵĳ��Ͽ�����ٶ�    
#define ATCIPSTARTOK        "AT+CIPSTART?\r\n"                              //��ѯ��ǰģ���Ƿ����������� 
#define ATCIPCLOSE          "AT+CIPCLOSE=0\r\n"                             //�رյ�ǰ����
#define ATCIPSCONT(x)       ((x)?("AT+CIPSCONT=0,\"%s\",\"%s\",%d,2")\
                                :("AT+CIPSCONT\r\n"))                       //��������   
#define ATCIPSTART          "AT+CIPSTART=\"%s\",\"%s\",%d\r\n"              //����TCP���ӵ�IP�Ͷ˿ں�
#define ATCIPMUX            "AT+CIPMUX=0\r\n"                               //�ر���������
#define ATCIPMODE(x)        ((x)?("AT+CIPMODE=1,0\r\n")\
                                :("AT+CIPMODE=0,0\r\n"))                    //����͸��ģʽ����͸��
#define ATCIPCFG(x)         ((x)?("AT+CIPCFG=1,50,0\r\n")\
                                :("AT+CIPCFG=0,50,0\r\n"))                  //�Զ�������������                      
#define ATCIPPACK(x)        ((x)?("AT+CIPPACK=1,\"4C4F47494E3A31303031\"\r\n")\
                                :("AT+CIPPACK=0,\"0102A0\"\r\n"))           //����ע���������������
#define ATCIPSEND(x)        ((x)?("AT+CIPSEND=%d\r\n")\
                                :("AT+CIPSEND\r\n"))                        //���÷��͵����ݳ���
								
#define ATCGMR              "AT+CGMR\r\n"                                   //��ȡ��վ��Ϣ
#define ATCMGS              "AT+CMGS=\"%s\"\r\n"                            //������Ҫ���Ͷ��ŵ��ֻ���
#define ATCMGR              "AT+CMGR=%s\r\n"                                //����Ҫ��ȡ���ŵ�λ��
#define ATCSTT              "AT+CSTT=\"CMNET\"\r\n"                         //�˻�����
#define ATCIPSCONT_C        "AT+CIPSCONT?\r\n"                              //�鿴͸���������            
#define GPRSSEND            0x1A                                 
#define CLOSEDTU            "+++"                                            //�ر�͸��
#define OPENDTU             "ATO0\r\n"                                      //���½���͸��









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

s8  gu906_init(void);
s8  gu906_Module_State(void);
s8  gu906_TCP_Socket(struct Gprs_Config *GprsCon);
s8  gu906_DTU_Socket(struct Gprs_Config *GprsCon);
s8  gu906_GPRS_write(char* pdat, int len);
uint32_t gu906_GPRS_read(char *pout, int len);


#endif