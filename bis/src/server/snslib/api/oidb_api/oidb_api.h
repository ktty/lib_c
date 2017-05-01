#ifndef _OIDB_API_H_
#define _OIDB_API_H_

#include <vector>
#include <map>
#include <string>
#include <string.h>
#include "comm/safe_tcp_client/safe_tcp_client.h"
#include "api/oidb_api/oidb_def.h"

namespace snslib
{

// RichFlag
const unsigned char OIDB_API_SERVICE_TYPE_PET   = 4;    //RICHFLAG��ͷ��־λ
const unsigned char OIDB_API_SERVICE_TYPE_CFACE = 12;   //RICHFLAG�Զ���ͷ���־λ

// RichFlag2
const unsigned char OIDB_API_SERVICE_TYPE_BEAR = 53;	//����ͼ��

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
const unsigned short OIDB_MSSTYPE_QQGAMEVIP = 77;   //������ֵλ
const unsigned short OIDB_MSSTYPE_QZONEVIP = 78;    //������ֵλ
const unsigned short OIDB_MSSTYPE_LVZUAN_1 = 82;    //������ֵλ1
const unsigned short OIDB_MSSTYPE_LVZUAN_2 = 91;    //������ֵλ2

const unsigned short OIDB_MSSTYPE_PETVIP_YEAR = 103;//���������ֵλ
const unsigned short OIDB_MSSTYPE_QQGAMEVIP_YEAR = 102; //���������ֵλ
const unsigned short OIDB_MSSTYPE_QZONEVIP_YEAR = 104;  //���������ֵλ
const unsigned short OIDB_MSSTYPE_LVZUAN_YEAR = 136;  //���������ֵλ

// RichFlagLevel
const unsigned char OIDB_RICHFLAGLEVEL_FZ = 49;		//����ȼ�
const unsigned char OIDB_RICHFLAGLEVEL_PET = 59;	//����ּ�
const unsigned char OIDB_RICHFLAGLEVEL_HZ = 2;      //����ȼ�
const unsigned char OIDB_RICHFLAGLEVEL_LZ = 16;     //����ȼ�
const unsigned char OIDB_RICHFLAGLEVEL_LVZ = 14;     //����ȼ�

const unsigned short OIDB_PROXY_NUM_MAX = 10;   //����ܹ����е�OIDB_PROXY����������

const unsigned char OIDB_API_MAX_NICKNAME_LEN    = 63;
// shimmeryang�� �޸�Ϊ81920
const unsigned int OIDB_API_MAX_PKG_LEN       = 81920;

const int OIDB_MAX_SESSIONKEY_LEN       = 256;      // session key��󳤶�
const int OIDB_MAX_IP_LEN = 16;	// ip ����

const char OIDB_SESSION_PTLOGIN = 1;
const char OIDB_SESSION_GAMESIG = 2;
const char OIDB_SESSION_SSO     = 3;
const char OIDB_SESSION_CLIENT  = 4;

const int OIDB_PET_SSO_ST_APPID = 0x200E01;
const int OIDB_PET_CLIENT_ST_APPID = 0x1000201;

typedef struct tagQQSimpleInfo
{
    unsigned int uiUin;
    // 2013��12��02�� 18:20:09 ͷ���Ѿ�������oidb��ȡ�ˣ���edzhong��
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

#define IsBitSet(pBuf,bit) (((unsigned char*)(pBuf))[(bit)/8]&(0x80>>((bit)%8)))

typedef struct tagQQAllMssInfo
{
    unsigned int uiUin;
    char szMssValue[20]; //20���ֽڱ�־λ, ��ϸ����ֵλ˵����ο���OIDB�����ĵ��б��еġ���ֵ����bitmap���塷

    bool IsHZ()
    {
        return IsBitSet(szMssValue, OIDB_MSSTYPE_QZONEVIP);
    }
    bool IsHZYear()
    {
        return IsBitSet(szMssValue, OIDB_MSSTYPE_QZONEVIP_YEAR);
    }

    bool IsLZ()
    {
        return IsBitSet(szMssValue, OIDB_MSSTYPE_QQGAMEVIP);
    }
    bool IsLZYear()
    {
        return IsBitSet(szMssValue, OIDB_MSSTYPE_QQGAMEVIP_YEAR);
    }

    bool IsFZ()
    {
        return IsBitSet(szMssValue, OIDB_MSSTYPE_PETVIP);
    }
    bool IsFZYear()
    {
        return IsBitSet(szMssValue, OIDB_MSSTYPE_PETVIP_YEAR);
    }

    bool IsLvZ()
    {
        return (IsBitSet(szMssValue, OIDB_MSSTYPE_LVZUAN_1) || IsBitSet(szMssValue, OIDB_MSSTYPE_LVZUAN_2));
    }
    bool IsLvZYear()
    {
        return IsBitSet(szMssValue, OIDB_MSSTYPE_LVZUAN_YEAR);
    }
}QQAllMssInfo;

typedef struct tagSessionContext
{
    //���������SSO_ST����д��0x200E01������ǵ�session_key����дҵ����ptlogin��Ӧ��appid
    unsigned int uiAppID;
    //PET_SSO_STΪ3��ptloginΪ1��client_keyΪ4
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
	std::string sGroupName;
	tagGroupInfo() : uiGroupID(0)
	{
	}
} GroupInfo;

typedef struct tagGroupedFriendList
{
	unsigned int uiGroupID;
	unsigned int uiSortID;
	std::string sGroupName;
	std::vector<unsigned int> vuiGroupMemberList;
	tagGroupedFriendList() : uiGroupID(0)
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
    char szHost[20];
    int iPort;
    bool bInit;

    tagOIDBProxyConf()
    {
        memset(this, 0, sizeof(*this));
    }
} OIDBProxyConf;

class COIDBProxyAPI
{
public:
    COIDBProxyAPI();
    ~COIDBProxyAPI();

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
    static COIDBProxyAPI& Instance();

    /**
     * ��ȡ�����б�
     * ����: uiUin QQ����, vecFriends ���صĺ����б�
     * ����ֵ: RET_OK, RET_FAIL
     */
    // shimmeryang: ��������Э��ֹͣʹ��
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
     * ��ȡRichFlag
     * ����: uiUin QQ����, byFlag ���ص�RichFlag, byServiceType ҵ��������Ϣ
     * ����ֵ: RET_OK, RET_FAIL
     */
    int GetRichFlag(unsigned int uiUin, unsigned char &byFlag, unsigned char byServiceType);
    int GetRichFlag(unsigned int uiUin, unsigned char &byFlag, unsigned char byServiceType, const SessionContext& stContext);

    /**
     * ����RichFlag
     * ����: uiUin QQ����, byFlag ���õ�RichFlag, byServiceType ҵ��������Ϣ��Ĭ��Ϊ����ҵ��
     * ����ֵ: RET_OK, RET_FAIL
     */
    int SetRichFlag(unsigned int uiUin, unsigned char byFlag, unsigned char byServiceType);
    int SetRichFlag(unsigned int uiUin, unsigned char byFlag, unsigned char byServiceType, const SessionContext& stContext);

    /**
     * ������ȡRichFlag
     * ����: uiUin QQ����, vecUinList �����б�, mapFlags ���ص�RichFlag, byServiceType ҵ��������Ϣ��Ĭ��Ϊ����ҵ��
     * ����ֵ: RET_OK, RET_FAIL
     */
    int GetFriendsRichFlag(unsigned int uiUin, const std::vector<unsigned int> &vecUinList, std::map<unsigned int, unsigned char> &mapFlags, unsigned char byServiceType)
    {
        return BatchGetRichFlag(uiUin, vecUinList, mapFlags, byServiceType);
    }
    int GetFriendsRichFlag(unsigned int uiUin, const std::vector<unsigned int> &vecUinList, std::map<unsigned int, unsigned char> &mapFlags, unsigned char byServiceType, const SessionContext& stContext)
    {
        return BatchGetRichFlag(uiUin, vecUinList, mapFlags, byServiceType, stContext);
    }
    int BatchGetRichFlag(unsigned int uiUin, const std::vector<unsigned int> &vecUinList, std::map<unsigned int, unsigned char> &mapFlags, unsigned char byServiceType);
    int BatchGetRichFlag(unsigned int uiUin, const std::vector<unsigned int> &vecUinList, std::map<unsigned int, unsigned char> &mapFlags, unsigned char byServiceType, const SessionContext& stContext);

    /**
     * ������ȡ������
     * ����: uiUin QQ����, vecUinList �����б�, vecInfo ���ص������б�
     * ����ֵ: RET_OK, RET_FAIL
     */
    // ��������BatchGetSimpleInfo��û�д�session���Ѿ�����ʹ����
    int BatchGetSimpleInfo(const std::vector<unsigned int> &vecUinList, std::vector<SQQSimpleInfo> &vecInfo);
    int BatchGetSimpleInfo(const std::vector<unsigned int> &vuiUin, std::map<unsigned int, SQQSimpleInfo> &mstQQSimpInfo);
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
     * ���õ����û�������ֵλ��ֵ
     * param uiUin: ��Ҫ��ȡ�û���UIN
     * param ushMssType: ��Ҫ��ȡ�û�����ֵλ OIDB_MSSTYPE_...
     * param cMssValue: ��ֵλ��ֵ 0-û�и���ֵλ 1-�и���ֵλ
     *
     * return: RET_OK, RET_FAIL
     */
    int SetMssFlag(unsigned int uiUin, unsigned short ushMssType, char cMssValue);
    int SetMssFlag(unsigned int uiUin, unsigned short ushMssType, char cMssValue, const SessionContext& stContext);

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
    int BatchGetMssFlag(unsigned int uiUin, const std::vector<unsigned int> &vuiUin, unsigned short ushMssType, std::map<unsigned int, QQMssInfo> &mstQQMssInfo, const SessionContext& stContext);

    /**
     * ������ȡ����û��Ķ����ֵλ
     * param vuiUin: ��Ҫ��ȡ�û���UIN
     *
     * return: RET_OK, RET_FAIL
     */
    // add by tofuli 2013.01.28
    int BatchGetAllMssFlag(unsigned int uiUin, const std::vector<unsigned int> &vuiUin, std::map<unsigned int, QQAllMssInfo> &mstAllQQMssInfo, const SessionContext& stContext);

    /**
     * ��ȡ�����û���������ֵλ
     * param uiUin: ��Ҫ��ȡ�û���UIN
     * param stQQAllMssInfo: ��ֵλ��Ϣ QQAllMssInfo.szMssValue ��λȡ��ƫ��������ֵλ�ı��
     *
     * return: RET_OK, RET_FAIL
     */
    //delete by jamieli�����治��ʹ�øýӿ�
    //int GetMssFlagMulti(unsigned int uiUin, QQAllMssInfo &stQQAllMssInfo);

	/**
	 * ��ȡ/����ҵ��ȼ�
	 * param uiUin: ��Ҫ�������û�UIN
	 * param ushType: ҵ������
	 * param cValue: ҵ��ֵ
	 *
	 * return: RET_OK, RET_FAIL
	 */
	int GetRichFlagLevel(unsigned int uiUin, unsigned char byServiceType, unsigned char &byLevel);
	int GetRichFlagLevel(unsigned int uiUin, unsigned char byServiceType, unsigned char &byLevel, const SessionContext& stContext);
	int SetRichFlagLevel(unsigned int uiUin, unsigned char byServiceType, unsigned char byLevel);
    int BatchGetRichFlagLevel(unsigned int uiUin, const std::vector<unsigned int> &vuiUin, unsigned char byServiceType, std::map<unsigned int, unsigned char> &mRichFlagLevel, const SessionContext& stContext);

    /**
     * ��ȡRichFlag
     * ����: uiUin QQ����, byFlag ���ص�RichFlag, byServiceType ҵ��������Ϣ
     * ����ֵ: RET_OK, RET_FAIL
     */
    int GetRichFlag2(unsigned int uiUin, unsigned char &byFlag, unsigned char byServiceType);

    /**
     * ����RichFlag2
     * ����: uiUin QQ����, byFlag ���õ�RichFlag, byServiceType ҵ��������Ϣ��Ĭ��Ϊ����ҵ��
     * ����ֵ: RET_OK, RET_FAIL
     */
    int SetRichFlag2(unsigned int uiUin, unsigned char byFlag, unsigned char byServiceType);

    /**
     * ��ȡQQ��ע����
     * ����: uiUin QQ���룬mapRemarkName ���صı�ע���ƣ�stContext ��½̬��Ϣ
     * ����ֵ: RET_OK, RET_FAIL
     */
    // 2013��11��28�� 21:43:22 shimmeryang: ��pet�Ǳ���ֲ����
    int GetRemarkName(unsigned int uUin, std::map<unsigned int, std::string> &mapRemarkName, const SessionContext& stContext);

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
    //int SendAndRecvOld();

private:
    static COIDBProxyAPI* m_pSingleton;

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
#endif

