#ifndef _APP_MD5_H
#define _APP_MD5_H
#include "bsp.h"

typedef unsigned long UINT4;   
typedef unsigned char *POINTER;   

typedef enum
{
	MD5_FALSE = 0,
	MD5_TRUE,
}MD5_CHECK;
#define R_memset(x, y, z) memset(x, y, z)   
#define R_memcpy(x, y, z) memcpy(x, y, z)   
#define R_memcmp(x, y, z) memcmp(x, y, z)   

#define MD5_CODE_LEN	8

/* MD5 context. */   
typedef struct {   
	/* state (ABCD) */    
	/*�ĸ�32bits�������ڴ�����ռ���õ�����ϢժҪ������Ϣ���ȡ�512bitsʱ��Ҳ���ڴ��ÿ��512bits���м���*/    
	UINT4 state[4];    
	   
	/* number of bits, modulo 2^64 (lsb first) */    
	/*�洢ԭʼ��Ϣ��bits������,����������bits���Ϊ 2^64 bits����Ϊ2^64��һ��64λ�������ֵ*/   
	UINT4 count[2];   
	   
	/* input buffer */    
	/*����������Ϣ�Ļ�������512bits*/   
	unsigned char buffer[64];    
} MD5_CTX;   
void MD5Init(MD5_CTX *);   
void MD5Update(MD5_CTX *, unsigned char *, unsigned int);   
void MD5Final(unsigned char [16], MD5_CTX *);   
MD5_CHECK MakeFile_MD5_Checksum(uint8_t *pPacket, uint32_t pPacket_len);
#endif//_APP_MD5_H
