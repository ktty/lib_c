/*
 * @   Filename��  shm_config_api_ex.h
 * @        dsc��  �û�����ʹ��getBuffById(),���õ������ڴ�����Ӧ�����ݣ��û�ֻ������Mem�׵�ַ�� ������ID�ţ����ɵõ�ID����Ӧ�Ĺ����ڴ�����
 * @ Created on: 2010-8-20
 * @     Author: peterfang@tencent.com
 */

#ifndef SHM_CONFIG_API_EX_H_
#define SHM_CONFIG_API_EX_H_

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include "comm/util/pet_util.h"
#include "comm/ini_file/ini_file.h"
#include "comm/share_mem/share_mem.h"
//#include "../parse_speed_test.pb.h"
//#include "../hf_db_user_info.pb.h"
#include <google/protobuf/message_lite.h>

using namespace std;

namespace snslib
{
#define O2P(offset) ((char*)g_pvMem+offset)  //����õ���Ӧƫ����λ�õ�ָ��

//�����������Թ����ڴ��СΪ120*1024*1024�����
const static unsigned int HEAD_LENGTH =  100;
const static unsigned int TABLE_LENGTH = 2*1024*1024;
const static unsigned int DATA1_OFFSET = 60*1024*1024;

//�ýṹ������ÿ����¼��������ŵ�λ�õ���Ϣ���Ա���õĶ�λ�ͷ���
typedef struct
{
    unsigned int ID;          //ĳ�����ݶ�����Ӧ���ڴ�Id��
    unsigned int offset;      //�����ݶε�ƫ����
    unsigned int data_size;   //�����ݶεĴ�С
    char Message_name[50];    //�����ݶε����֣���Ӧһ����Ϣ����
}TABLE_INFO;

//���ݿ����������Ϣ
typedef struct
{
    int  DB_Id;
    int  DB_Port;
    char DB_Host[50];
    char DB_User[50];
    char DB_Pass[50];
    char DB_Name[50];
    char DB_Table_Name[50];
}DB_INFO;

class CShmConfigApiEx 
{
public:
    CShmConfigApiEx();

    int initShmMem(const char* pszConf);               //��ʼ�������ڴ�

    template <typename T>
    int GetRecordList(uint32_t uiMessageID, T& objRecords)
    {
        g_pHeadMem = (TABLE_INFO*) Head();

        uint32_t uiIdNumber = 0;
        uint32_t uiFlag = *((uint32_t*) Head());

        TABLE_INFO* g_pTOfDataMem;

        if (uiFlag == 0)
        {
            printf("uiFlag == 0\n");
            g_pTOfDataMem = (TABLE_INFO*) TOfData0();
            uiIdNumber = *(int*) Data0();
        }
        else
        {
            printf("uiFlag == 1\n");
            g_pTOfDataMem = (TABLE_INFO*) TOfData1();
            uiIdNumber = *(int*) Data1();
        }

        for (unsigned int i = 0; i < uiIdNumber; i++)
        {
            if ((g_pTOfDataMem + i)->ID == uiMessageID)
            {
                //printf("ID = %d\n", (g_pTOfDataMem + i)->ID);
                //printf("Name = %s\n", (g_pTOfDataMem + i)->Message_name);
                //printf("offset = %d\n", (g_pTOfDataMem + i)->offset);
                //printf("size = %d\n", (g_pTOfDataMem + i)->data_size);
                //printf("<<<buff=%s>>>\n\n", CStrTool::Str2Hex( (void*)O2P((g_pTOfDataMem + i)->offset), (g_pTOfDataMem + i)->data_size));
                objRecords.ParseFromArray((void*)O2P((g_pTOfDataMem + i)->offset), (g_pTOfDataMem + i)->data_size);
                return 0;
            }
        }

        //�������ˣ�˵��û�ҵ�
        printf("ERROR:Your Message ID is:%d,cannot find this Message.\n", uiMessageID);
        return -1;
    }

    void getBuffById(unsigned int inputID, char *buff, int *buff_size); //�����ṩ�Ľӿڣ��û�ֻ������Mem�׵�ַ�� ������ID�ţ����ɵõ�ID����Ӧ�Ĺ����ڴ���Ϣ

    virtual ~CShmConfigApiEx();

private:
    void *g_pvMem;                                     //���湲���ڴ��׵�ַ��Ϣ

    TABLE_INFO* g_pHeadMem;       //ָ��ͷָ���׵�ַ
    char *g_pDataMem;                  //ָ������������Ӧ���׵�ַ

    inline char* Head() {return O2P(0);}

    inline char* Data0()  {return O2P(HEAD_LENGTH);}

    inline char* TOfData0()  {return O2P(HEAD_LENGTH+8);}        //ע��table�е��ʼ4���ֽڣ�����table��ID������

    inline char* BuffOfData0()  {return O2P(HEAD_LENGTH+8+TABLE_LENGTH);}  //buff����ʼλ�ô�2M�ĵط���ʼ

    inline char* Data1()  {return O2P(DATA1_OFFSET);}

    inline char* TOfData1()  {return O2P(DATA1_OFFSET+8);}       //ע��table�е��ʼ4���ֽڣ�����table��ID������

    inline char* BuffOfData1()  {return O2P(DATA1_OFFSET+8+TABLE_LENGTH);}
};
}
#endif /* SHM_CONFIG_API_EX_H_ */
