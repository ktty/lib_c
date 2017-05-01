/**
 * @file    load_tool_peter.h
 * @brief   ���ݿ���Ϣ���ع���,�����ݿ���Ϣ���ص������ڴ浱��ȥ
 * @author  peterfang@tencent.com
 * @date    2010-08-19
 */
#ifndef __SHMCONF_API_H__
#define __SHMCONF_API_H__

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include "comm/share_mem/share_mem.h"

namespace snslib
{

const static unsigned int SHMCONF_KEY = 0x550001;
const static unsigned int SHMCONF_HEAD_LENGTH = 64;
const static unsigned int SHMCONF_TABLE_LENGTH = 4*1024*1024;
const static unsigned int SHMCONF_DATA_LENGTH = 64*1024*1024;
const static unsigned int SHMCONF_BLOCK_SIZE =  SHMCONF_TABLE_LENGTH + SHMCONF_DATA_LENGTH;
const static unsigned int SHMCONF_DATA0_OFFSET = SHMCONF_HEAD_LENGTH ;
const static unsigned int SHMCONF_DATA1_OFFSET = SHMCONF_HEAD_LENGTH + SHMCONF_BLOCK_SIZE;
const static unsigned int SHMCONF_TOTAL_LENGTH = SHMCONF_HEAD_LENGTH + 2*(SHMCONF_BLOCK_SIZE);

//�ýṹ������ÿ����¼��������ŵ�λ�õ���Ϣ���Ա���õĶ�λ�ͷ���
typedef struct{
    unsigned int ID;          //ĳ�����ݶ�����Ӧ���ڴ�Id��
    unsigned int offset;      //�����ݶε�ƫ����
    unsigned int data_size;   //�����ݶεĴ�С
}SHMCONF_TABLE_INFO;

//����HEAD��ʽ
typedef struct{
    int iUseFlag;             //ʹ�ñ�־��0��ʾdata0����ʹ�ã�1��ʾdata1����ʹ��
}SHMCONF_MEM_HEAD;

//���ݿ����������Ϣ
typedef struct
{
    int  DB_Port;
    char DB_Host[64];
    char DB_User[64];
    char DB_Pass[64];
    char DB_Name[64];
    char DB_Table_Name[128];
}DB_INFO;

class CShmConfApi
{
public:
	CShmConfApi();
	~CShmConfApi();

public:
	int Init();
	int GetRocord(unsigned int uiID, char ** pszData, int * piLength );
	char * GetErrMsg(){
		return m_szErrMsg;
	}
private:
	char m_szErrMsg[255];
	char * m_pszShmConf;
};

}

#endif //__SHMCONF_API_H__
