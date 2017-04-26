#ifndef _OIDB_PROXY_DEF_H_
#define _OIDB_PROXY_DEF_H_

#define OIDB_TRANS_PKG_MAGIC "MGCOIDBTRS"
// 2013��11��28�� 21:38:07 
// shimmeryang: ���ڵĺ��ѷ�����Ϣ�Ѿ�����8192���ֽ���
const int MAX_TRANSPORT_PKG_LEN         = 81920;
const int MAX_TRANSPORT_PKG_BODY_LEN    = MAX_TRANSPORT_PKG_LEN - 5;
const int SSO_PET_ST_APPID = 0x200e01;

typedef struct tagAssistInfo
{
    char abyVersion[2];         //������Ϣ�ṹ�汾�� 		unsigned short
    char szUserName[11];        //���ýӿڵ��û���
    char szPasswd[11];          //���ýӿڵ��û�����
    char abyServiceIP[4];       //ǰ��Ӧ������IP			unsigned long
    char szServiceName[16];     //ǰ��Ӧ�÷�������
    char abyServiceTime[4];     //ǰ�˷��ʽӿ�ʱ��			time_t
    char abyServiceSeq[4];      //ǰ�˷��ʽӿ���ˮ��		unsigned long
    char cServiceType;          //���ýӿڵķ�������
    char abyClientIP[4];        //����ǰ�˷�����û�IP		unsigned long
    char szClientName[21];      //����ǰ�˷�����û���
    char abyClientUin[4];       //����ǰ�˷�����û�UIN		unsigned long
    char abyFlag[4];            //���ݱ�־					long
    char szDesc[30];            //��ע˵��

} SAssistInfo;

//����½̬��չ��ͷ���Ǳ䳤��ͷ(�������˵��)
typedef struct
{
    unsigned short   ushExLen;                //��չ��ͷ����(����shExtLen����)������
    short   shExVer;                //��չ���汾�ţ���ʼ�汾Ϊ600������
    unsigned int   uiAppID;                //��ǩ���ɷ����ƶ�����ptlogin��db�ȣ�����
    char   chKeyType;               //key���ͣ���½key���ͣ���������� ����
    unsigned short   ushSessionKeyLen;         //sessionkey�ĳ��ȣ���½key ����
    char   szBuffSessionKey[128];       //sessionkey(�128�ֽ�)����½key ����
    unsigned short   ushReservedLen;                           //�������ȣ�ĿǰΪ0
    char   szBuffReserved[64];        //�����ֽ�((�64�ֽ�)
    unsigned short   ushContextDataLen;                        //�����ĳ��ȣ�����Ҫ����0
    char   szContextData[64];      //����������((�64�ֽ�)��oidbԭ������

} TransPkgHeadExt; //oidb��½̬��չ��ͷ

typedef struct tagTransPkgHeader
{
    char    abyLength[2];
    char    abyVersion[2];
    char    abyCommand[2];
    char    abyUin[4];
    char    cResult;    //11
    SAssistInfo stAssistInfo;
} STransPkgHeader;
//127

typedef struct tagTransPkg
{
    char    cStx;
    STransPkgHeader stHeader;
    unsigned char byExtraFlag; // not serialized
    TransPkgHeadExt stExtraInfo;
    unsigned short ushBodyLen; // not serialized
    char    abyBody[MAX_TRANSPORT_PKG_LEN - 5];
    char    cEtx;
} STransPkg;

#endif

