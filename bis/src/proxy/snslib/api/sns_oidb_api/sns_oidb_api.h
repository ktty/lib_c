#ifndef _SNS_OIDB_API_H_
#define _SNS_OIDB_API_H_

#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <limits.h>
#include "comm/safe_tcp_client/safe_tcp_client.h"
#include "api/sns_oidb_api/sns_oidb_def.h"

namespace snslib
{
namespace oidb2
{
const unsigned char OIDB_API_MAX_IP_LEN  = 15;

// MSSTYPE
// �����⼸������QQ��Ա���λ
const unsigned short OIDB_MSSTYPE_CLUB = 64;		//��Ա���
const unsigned short OIDB_MSSTYPE_CLUB_ANNUAL = 65;	//��ѻ�Ա
const unsigned short OIDB_MSSTYPE_163S = 65;		//��ѻ�Ա����OIDB��׼����
const unsigned short OIDB_MSSTYPE_163C = 66;		//163�������û�
const unsigned short OIDB_MSSTYPE_263U = 67;		//263�����û�
const unsigned short OIDB_MSSTYPE_MCLB = 68;		//�ƶ���Ա
const unsigned short OIDB_MSSTYPE_TCNT = 69;		//15Ԫ�ֻ������û�
const unsigned short OIDB_MSSTYPE_VNETCLB = 81;		//Vnet��Ա�û�

const unsigned short OIDB_MSSTYPE_PETVIP = 92;      //������ֵλ
const unsigned short OIDB_MSSTYPE_QZONEVIP = 78;    //������ֵλ

const unsigned short OIDB_MSSTYPE_PETVIP_YEAR = 103;//���������ֵλ

const unsigned short OIDB_PROXY_NUM_MAX = 10;   //����ܹ����е�OIDB_PROXY����������

const unsigned char OIDB_API_MAX_NICKNAME_LEN    = 64;
const unsigned short OIDB_API_MAX_PKG_LEN       = 8192;

const int OIDB_MAX_SESSIONKEY_LEN       = 256;      // session key��󳤶�
const int OIDB_MAX_IP_LEN = 16;	// ip ����
const char OIDB_SESSION_PTLOGIN = 1;
const char OIDB_SESSION_GAMESIG = 2;
const char OIDB_SESSION_SSO     = 3;
const char OIDB_SESSION_CLIENT  = 4;

typedef struct tagQQSimpleInfo
{
    unsigned int uiUin;
    unsigned short ushFace;
    unsigned char byAge;
    unsigned char byGender;
    char szNick[OIDB_API_MAX_NICKNAME_LEN + 1];

} SQQSimpleInfo;

typedef struct tagQQMssInfo
{
    unsigned int uiUin;
    unsigned short ushMssType;
    char cMssValue;
}QQMssInfo;

typedef struct tagQQAllMssInfo
{
    unsigned int uiUin;
    char szMssValue[20];    //����oidb�ṩ20*8����ֵλ������λ��ƫ������ȡ������ֵλ��ƫ����ΪOIDB_MSSTYPE_...����ϸ����ֵλ��Ϣ��ο�oidb��վ�ϵ�����
}QQAllMssInfo;

typedef struct tagSessionContext
{
    unsigned int uiAppID;
    char cKeyType;
    char szSessionKey[OIDB_MAX_SESSIONKEY_LEN];
    unsigned short ushSessionKeyLen;
    unsigned int uiClientIP;
    unsigned int uiConnSvrIP;

    tagSessionContext() { memset(this, 0x0, sizeof(*this)); }
} SessionContext;

typedef struct tagGroupInfo
{
	unsigned int uiGroupID;
	unsigned int uiSortID;
	std::string sGroupName; // group name is utf8 encoded
	tagGroupInfo() : uiGroupID(0), uiSortID(0)
	{
	}
} GroupInfo;

typedef struct tagGroupedFriendList
{
	unsigned int uiGroupID;
	unsigned int uiSortID;
	std::string sGroupName;
	std::vector<unsigned int> vGroupMemberList; // qqnick is gbk encoded
	tagGroupedFriendList() : uiGroupID(0), uiSortID(0)
	{
	}
} GroupedFriendList;

typedef struct tagProtoHeader
{
    unsigned short ushLength;
    unsigned int uiUin;
    unsigned short ushCommand;
    unsigned int uiServiceIP;
    char szServiceName[16];
    unsigned int uiClientIP;
    unsigned char byServiceType;
    unsigned char byResult;

    unsigned short ushExtraFlag; // �ֶβ������л�/�����л���ֻ����Ϊ��ʶ
    unsigned int uiAppID;
    char chKeyType; // ���ͼ�OIDB_SESSION_xxx
    unsigned short ushSessionKeyLen;
    char szSessionKey[OIDB_MAX_SESSIONKEY_LEN];
} SOIDBProtoHeader;

const unsigned short OIDB_API_PROTO_HEADER_LEN  = 34;
const unsigned short OIDB_API_PROTO_EXTRA_HEADER_MIN_LEN = 9;

typedef struct tagOIDBProxyConf
{
    char szHost[OIDB_API_MAX_IP_LEN + 1];
    int iPort;
    bool bInit;

    tagOIDBProxyConf()
    {
        memset(this, 0, sizeof(*this));
    }
} OIDBProxyConf;

class CSnsOIDBProxyAPI
{
public:
    CSnsOIDBProxyAPI();
    ~CSnsOIDBProxyAPI();

    static const int RET_OK = 0;
    static const int RET_FAIL = -1;
    static const int RET_IS_FRIEND = 1;
    static const int RET_NOT_FRIEND = 0;

    /**
     * ��ʼ��oidb_api�ӿ�
     * ����: pszFile �ӿ������ļ�, pszServiceName ���ýӿڵĳ���ҵ������
     * ����ֵ: RET_OK, RET_FAIL
     */
    int Init(const char *pszFile, const char *pszServiceName);

    /**
     * ��ȡCOIDBProxyAPI�ĵ���ʵ����
     *
     * @return COIDBProxyAPI�ĵ���ʵ��
     */
    static CSnsOIDBProxyAPI& Instance();

    /**
     * ��ȡ�����б�
     * ����: uiUin QQ����, vecFriends ���صĺ����б�
     * ����ֵ: RET_OK, RET_FAIL
     */
    int GetFriendList(unsigned int uiUin, std::vector<unsigned int> &vecFriends);
    int GetFriendList(unsigned int uiUin, std::vector<unsigned int> &vecFriends, const SessionContext& stContext);

    int GetGroupedFriendList(unsigned int uiUin, std::vector<GroupedFriendList> &vecGroupedFriends, const SessionContext& stContext);
    int GetGroupInfo(unsigned uiUin, std::map<unsigned, GroupInfo> &mstQQFriendGroups, const SessionContext& stContext);

    /**
     * ��ȡ�����ϣ������ǳơ��Ա��
     * ����: uiUin QQ����, stInfo ���ؼ�����
     * ����ֵ: RET_OK, RET_FAIL
     */
    int GetSimpleInfo(unsigned int uiUin, SQQSimpleInfo &stInfo);
    int GetSimpleInfo(unsigned int uiUin, SQQSimpleInfo &stInfo, const SessionContext& stContext);

    /**
     * ������ȡ������
     * ����: uiUin QQ����, vecUinList �����б�, vecInfo ���ص������б�
     * ����ֵ: RET_OK, RET_FAIL
     */
    int BatchGetSimpleInfo(const std::vector<unsigned int> &vecUinList, std::vector<SQQSimpleInfo> &vecInfo);
    int BatchGetSimpleInfo(const std::vector<unsigned int> &vuiUin, std::map<unsigned int, SQQSimpleInfo> &mstQQSimpInfo);
    int BatchGetSimpleInfo(unsigned int uiUin, const std::vector<unsigned int> &vecUinList, std::vector<SQQSimpleInfo> &vecInfo, const SessionContext& stContext);
    int BatchGetSimpleInfo(unsigned int uiUin, const std::vector<unsigned int> &vuiUin, std::map<unsigned int, SQQSimpleInfo> &mstQQSimpInfo, const SessionContext& stContext);

    /**
     * ��֤uiFriendUin�ǲ���uiUin�ĺ���
     * ����: uiUin QQ����, uiFriendUin ��Ҫ��֤�ĺ��Ѻ���
     * ����ֵ: RET_OK, RET_FAIL, RET_IS_FRIEND, RET_NOT_FRIEND
     */
    int CheckFriend(unsigned int uiUin, unsigned int uiFriendUin);
    int CheckFriend(unsigned int uiUin, unsigned int uiFriendUin, const SessionContext& stContext);

    /**
     * ��ȡ�����û�������ֵλ��ֵ
     * param uiUin: ��Ҫ��ȡ�û���UIN
     * param ushMssType: ��Ҫ��ȡ�û�����ֵλ OIDB_MSSTYPE_...
     * param cMssValue: ��ֵλ��ֵ 0-û�и���ֵλ 1-�и���ֵλ
     *
     * return: RET_OK, RET_FAIL
     */
    int GetMssFlag(unsigned int uiUin, unsigned short ushMssType, char &cMssValue);
    int GetMssFlag(unsigned int uiUin, unsigned short ushMssType, char &cMssValue, const SessionContext& stContext);

    /**
     * ������ȡ����û��ĵ�����ֵλ
     * param vuiUin: ��Ҫ��ȡ�û���UIN
     * param ushMssType: ��Ҫ��ȡ�û�����ֵλ OIDB_MSSTYPE_...
     * param vstQQMssInfo: ��ֵλ��Ϣ QQMssInfo.cMssValue  0-û�и���ֵλ 1-�и���ֵλ
     *
     * return: RET_OK, RET_FAIL
     */
    int BatchGetMssFlag(const std::vector<unsigned int> &vuiUin, unsigned short ushMssType, std::vector<QQMssInfo> &vstQQMssInfo);
    int BatchGetMssFlag(const std::vector<unsigned int> &vuiUin, unsigned short ushMssType, std::map<unsigned int, QQMssInfo> &mstQQMssInfo);
    int BatchGetMssFlag(unsigned int uiUin, const std::vector<unsigned int> &vuiUin, unsigned short ushMssType, std::vector<QQMssInfo> &vstQQMssInfo, const SessionContext& stContext);
    int BatchGetMssFlag(unsigned int uiUin, const std::vector<unsigned int> &vuiUin, unsigned short ushMssType, std::map<unsigned int, QQMssInfo> &mstQQMssInfo, const SessionContext& stContext);

    /**
     * ��ȡ�����û���������ֵλ
     * param uiUin: ��Ҫ��ȡ�û���UIN
     * param stQQAllMssInfo: ��ֵλ��Ϣ QQAllMssInfo.szMssValue ��λȡ��ƫ��������ֵλ�ı��
     *
     * return: RET_OK, RET_FAIL
     */
    //delete by jamieli�����治��ʹ�øýӿ�
    int GetMssFlagMulti(unsigned int uiUin, QQAllMssInfo &stQQAllMssInfo);

    int BatchGetMssFlagMulti(const std::vector<unsigned int> &vuiUin, std::map<unsigned int, QQAllMssInfo>& mstQQAllMssInfo);

    int GetMssFlagMulti(unsigned int uiUin, QQAllMssInfo &stQQAllMssInfo, const SessionContext& stContext);

    int BatchGetMssFlagMulti(unsigned uiUin, const std::vector<unsigned int> &vuiUin, std::map<unsigned int, QQAllMssInfo> &mstQQAllMssInfo, const SessionContext& stContext);
    int BatchGetMssFlagMulti(unsigned uiUin, const std::vector<unsigned int> &vuiUin, std::vector<QQAllMssInfo> &vstQQAllMssInfo, const SessionContext& stContext);

    inline const char *GetErrMsg()
    {
        return m_szErrMsg;
    }


    static int PackTransPkg(char * pMem, const STransPkg &stPkg);
    static int UnPackTransPkg(const char * pMem, unsigned uiLen, STransPkg& stPkg);

    static int PackHeader(char * pMem, const SOIDBProtoHeader &stHeader);
    static int UnPackHeader(const char * pMem, SOIDBProtoHeader &stHeader);
    static int UnPackExtraHeader(const char * pMem, SOIDBProtoHeader &stHeader);

private:
    int FillReqTransHeader(unsigned int uiUin, unsigned short ushCmdID, char cServiceType, const SessionContext *pstContext = NULL);
    int SendAndRecv();
    int SendAndRecvOnce();

private:
    bool m_bInit;
    char m_szErrMsg[256];
    int m_iTimeout;

    unsigned int m_uiServiceIP;
    char m_szServiceName[16];

    char m_abyRequest[OIDB_API_MAX_PKG_LEN];
    unsigned int m_uiRequestLen;
    char m_abyResponse[OIDB_API_MAX_PKG_LEN];
    unsigned int m_uiResponseLen;
    STransPkg m_stReqTransPkg;
    STransPkg m_stRspTransPkg;

    CSafeTcpClient m_oTcpClient;
    OIDBProxyConf m_astProxyConf[OIDB_PROXY_NUM_MAX];
    int m_iOIDBProxyNum;

    unsigned int m_uiReqSeq;

    //SOIDBProtoHeader m_stHeader;

};
}
}
#endif

