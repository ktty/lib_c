#ifndef _MSG_CENTRE_UTIL_H_
#define _MSG_CENTRE_UTIL_H_
#include <stdio.h>
#define _MSG_CENTRE_UTIL_H_
#include <stdio.h>
#include <stdarg.h>

#include <string.h>
#include <map>
#include <vector>

#include "comm/util/pet_util.h"
#include "api/include/msg_centre_protocol.h"

namespace snslib
{

//�ֶε�����
enum
{
    MC_TYPE_NOR = 1,    //NOR���ͣ���ʾ�����VALû������
    MC_TYPE_BYTE,       //BYTE���ͣ���ʾ�����VAL��һ��BYTE
    MC_TYPE_SHORT,      //SHORT���ͣ���ʾ�����VAL��һ��SHORT
    MC_TYPE_INT,        //INT���ͣ���ʾ�����VAL��һ��INT
    MC_TYPE_LL,         //LL���ͣ���ʾ�����VAL��һ��LONGLONG
    MC_TYPE_STR,        //STR���ͣ���ʾ�����VAL��һ���̶����ȵ�BUFF���洢�˿��Դ�ӡ���ַ�����������FieldMaxLen��ʾ
    MC_TYPE_BIN,        //STR���ͣ���ʾ�����VAL��һ���̶����ȵ�BUFF���洢�˲����Դ�ӡ�����ݣ�������FieldMaxLen��ʾ
};

typedef struct tagMsgCentreMetaData
{
    char szFieldName[64];
    unsigned short ushFieldID;      //TLV�е�T����
    unsigned short ushFieldType;    //�ֶε����ͣ�֧��NOR BYTE SHORT INT LL STR
    unsigned short ushFieldMaxLen;  //TLV��L����󳤶ȣ����ڹ̶����ȵ����ͣ���ʾ���ȣ�
}MsgCentreMetaData;

class CMCUtil
{
private:
    static int m_iInitFlag;
    static std::map<unsigned short, MsgCentreMetaData> m_mMsgCentreMetaData;

    static void Init();

public:

    //�ڲ����ݽṹ��ʽ��
    // �����ʽ��[20Byte:BusHeader][20Byte:PetHeader][2Byte:MsgCentrePkgNum][MsgCentrePkg_1][MsgCentrePkg_2]����[MsgCentrePkg_n]
    // ������Ϣ��[4Byte:MagicNum][2Byte:PkgType][2Byte:PkgLen][PkgLen Byte:PkgVal]
    //�����������������PetHeader�����������ݵĴ���ͽ������

    /**
     * @brief PackMCPkg ������Ķ��MsgCentreMsg�����ָ����Buff��
     * @param pvBuffer �ⲿ����Ĵ洢�ռ�ָ��
     * @param piPackLen[IN/OUT] �ⲿ����Ĵ洢�ռ䳤�ȣ����ʱ����������ó��ȣ�����ʧ���˳������������Buff�еĳ���
     * @param vstMsgCentreMsg ��Ҫ�������Ϣ����
     * @return 0-�ɹ� ����-ʧ��
     */
    static int PackMCPkg(void* pvBuffer, int *piPackLen, std::vector<MsgCentreMsg> vstMsgCentreMsg);

    /**
     * @brief UnpackMCPkg ��Buff�е����ݽ���������vstMsgCentreMsg��
     * @param pvBuffer �ⲿ����Ĵ洢�ռ�ָ��
     * @param piUnpackLen[IN/OUT] ��Ҫ����ĳ��ȣ�����ʵ�ʽ���ĳ���
     * @param vstMsgCentreMsg ����Ժ����Ϣ
     * @return 0-�ɹ� ����-ʧ��
     */
    static int UnpackMCPkg(void* pvBuffer, int *piUnpackLen, std::vector<MsgCentreMsg> &vstMsgCentreMsg);

    /**
     * @brief �����ݸ�ʽ��������ַ�����
     */
    static const char *FormatMCPkg(std::vector<MsgCentreMsg> &vstMsgCentreMsg);

    CMCUtil()
    {
    }

    ~CMCUtil()
    {
    }

};
}
#endif
