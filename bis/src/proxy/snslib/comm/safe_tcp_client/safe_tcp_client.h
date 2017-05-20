#ifndef _SAFE_TCP_CLIENT_H_
#define _SAFE_TCP_CLIENT_H_

#include <vector>
#include "comm/share_mem/share_mem.h"

namespace snslib
{

const int STCP_CONN_STATUS_SHM_KEY = 0x00210001;    //�������״̬�Ĺ����ڴ�KEY
const int STCP_CONN_STATUS_SHM_SIZE = 16777216;     //�������״̬�Ĺ����ڴ��С 16MB�����Դ��10w�ڵ�

const int STCP_CONN_STATUS_OK = 1;      //����
const int STCP_CONN_STATUS_E_CONN = 2;  //����ʧ��
const int STCP_CONN_STATUS_E_RECV = 3;  //��������ʧ��
const int STCP_CONN_STATUS_E_SEND = 4;  //��������ʧ��
const int STCP_CONN_STATUS_E_FULL = 5;  //״̬�ڴ��Ѿ������ýڵ㲻�ڹ����ڴ���

const int STCP_MAX_CONN_STATUS = 2000;  //�����ڴ��б���Ľڵ㣬�������2000��
const int STCP_MAX_CONN_DATA   = 200;   //����API�������������200̨������

const int STCP_RETRY_TIME_INTER_DEFAULT = 600;      //Ĭ������ʱ�䣺10����

const int STCP_DEFAULT_TIMEOUT = 5000;       // TCP Client ��ʱĬ��ֵ

const int STCP_DEFAULT_PEERTIMEOUT = 180;       // TCP Client �Զ˳�ʱʱ��

const int STCP_STATUS_MEM_MAGICNUM = 0x19811103;
const int STCP_STATUS_MEM_VERSION = 0x0100;

typedef struct tagSTcpAddr
{
    unsigned int uiIPAddr;  //IP��ַ
    unsigned short ushPort; //�˿ں�
}STcpAddr;

typedef struct tagSTcpConnStatus
{
    unsigned int uiIPAddr;      //IP��ַ
    unsigned short ushPort;     //�˿ں�
    unsigned short ushStatus;   //״̬ �μ���STCP_CONN_STATUS_XX
    unsigned int uiTimeOut;     //��ʱʱ��
    time_t tLastRetryTime;      //�ϴγ���������ʱ��
    unsigned int uiRetryTimes;  //����ʧ�ܵĴ���
    unsigned int uiConnErrNum;  //����ʧ���ܴ���
    unsigned int uiRecvErrNum;  //����ʧ���ܴ���
    unsigned int uiSendErrNum;  //����ʧ���ܴ���
    unsigned int uiLastReqNum;  //���ϴμ����������������������
    char szNULL[88];            //��䲿��
}STcpConnStatus;

typedef struct tagSTcpConnStatusHeader
{
    unsigned int uiMagicNum;    //��ǣ�0x19811103
    unsigned short ushVersion;  //�汾�ţ�0x0100
    unsigned short ushUNUSE;
    time_t tCreateTime;         //�����ڴ洴��ʱ��
    unsigned int uiConnStatusNum;   //����״̬����Ч����
    unsigned int uiStatusFullNum;   //����״̬ʱ����FULL�Ĵ���
    time_t tLastCheckTime;      //�ϴμ��ʱ��
    char szNULL[104];           //��䲿��
    STcpConnStatus astSTcpConnStatus[0];
}STcpConnStatusHeader;

typedef struct tagSTcpConnData
{
    unsigned int uiIPAddr;
    unsigned short ushPort;
    int iSocket;
    time_t tLastUseTime;
}STcpConnData;

class CSafeTcpClient
{
public:
    CSafeTcpClient();
	~CSafeTcpClient();

	/**
	 * @brief ʹ�������ļ���ʼ��
	 * @param pszConfFile �����ļ�����
	 * @param pszSecName ��Ҫ��ȡ�����ļ��е�SEC����
	 *
	 * @note
	 * �����ļ���ʽ��
	 * [SAFE_TCP_CLI]
	 * ;;�����б�
	 * ;;���Ȳ��ø÷�ʽ���ã������������Ϊ�գ��Ż��ȡHostNum,HostX,PortXѡ��
	 * HostList=172.25.40.8,172.25.40.9
	 * ;;HostList��Ӧ�Ķ˿ڣ�ֻ�е�HostList��Ϊ��ʱ��Ч
	 * Port=2201
	 * ;;��ʱʱ�䣬��λ������
	 * TimeOut=5000
	 * ;;���Լ������λ����
	 * RetryInterval=600
	 * HostNum=2
	 * Host1=172.25.40.8
	 * Port1=2202
	 * Host2=172.25.40.9
	 * Port2=2203
	 *
	 */
	int Init(const char *pszConfFile, const char *pszSecName = "SAFE_TCP_CONF");

	/**
	 * @brief ʹ�ö��IP��ַ����ͬ�Ķ˿ڳ�ʼ��
	 * @param pszIPList IP��ַ�б����IP��ַ���ö���(,)�ո�( )�ֺ�(;)����
	 *
	 * @note һ������£�ͬһ�����Ķ˿���һ�µģ����Բ������ַ�ʽ��ʼ��Ҳ�ȽϷ���
	 */
	int Init(const char * pszIPList, int iPort, int iTimeOut = STCP_DEFAULT_TIMEOUT, int iRetryInterval = STCP_RETRY_TIME_INTER_DEFAULT, int iPeerTimeOut = STCP_DEFAULT_PEERTIMEOUT);

        /**
         * @brief ʹ�ö��IP��ַ����ͬ�Ķ˿ڳ�ʼ��
         * @param vstSTcpAddrList ��ַ/�˿��б�
         *
         * @note һ������£�ͬһ�����Ķ˿���һ�µģ����Բ������ַ�ʽ��ʼ��Ҳ�ȽϷ���
         */
        int Init(const std::vector<STcpAddr> &vstSTcpAddrList, int iTimeOut = STCP_DEFAULT_TIMEOUT, int iRetryInterval = STCP_RETRY_TIME_INTER_DEFAULT, int iPeerTimeOut = STCP_DEFAULT_PEERTIMEOUT);

	/**
	 * @brief ���ó�ʱʱ�䣬��λ����
	 */
	int SetTimeOut(int iTimeOut);

	/**
	 * @brief ��������
	 * @param pszBuff ��Ҫ�������ݵ�ͷָ��
	 * @param uiLen ��Ҫ�������ݵĳ���
	 */
    int Send(const void *pszBuff, unsigned int uiLen);

    /**
     * @brief ��������
     * @param pszBuff �����Ժ�������Buff��ͷָ��
     * @param puiLen �������������������ʱ��ָ����������Buff�ĳ��ȣ����ʱ����ʾ���յ������ݵĳ���
     * @param uiExpectLen �����ĳ��ȣ�����ﲻ�������ĳ��ȣ���ȴ��������գ�ֱ����ʱ����Ϊ0ʱ����ʾ���ص�һ��read�ĳ���
     */
    int Recv(void *pszBuff, unsigned int *puiLen, unsigned int uiExpectLen = 0);

    /**
     * @brief ���Ͳ���������
     * @param pszSendBuff ��Ҫ�������ݵ�ͷָ��
     * @param uiSendLen ��Ҫ�������ݵĳ���
     * @param pszRecvBuff �����Ժ�������Buff��ͷָ��
     * @param puiRecvLen �������������������ʱ��ָ����������Buff�ĳ��ȣ����ʱ����ʾ���յ������ݵĳ���
     * @param uiExpectLen �����ĳ��ȣ�����ﲻ�������ĳ��ȣ���ȴ��������գ�ֱ����ʱ����Ϊ0ʱ����ʾ���ص�һ��read�ĳ���
     */
    int SendAndRecv(const void * pszSendBuff, unsigned int uiSendLen, void * pszRecvBuff, unsigned int *puiRecvLen, unsigned int uiExpectLen = 0);

	/* *
	 * @brief �رյ�ǰ���ӣ�״̬��ع����ڴ�Ȳ�����
	 * @note ֻ�ǹر����һ�β����������ӣ��ر����Ӳ�Ӱ��״̬��Ϣ
	 *       һ�����ڳ�����ʱ���յ�һЩ�Ƿ������ݣ�Ϊ�˷�ֹ�Ժ�����������Ӱ�죬�رյ�ǰ������
	 */
	void CloseCurConn();

	/* *
	 * @brief �ر��������ӣ�״̬��ع����ڴ�Ȳ�����
	 * @note ֻ�ǹرո�API��ʼ���������ӣ��ر����Ӳ�Ӱ��״̬��Ϣ
	 */
	void CloseAllConn();

    /**
     * @brief �ر��������ӣ�����Detach�����ڴ�
     * @note ֻ�ǹرո�API��ʼ���������ӣ��ر����Ӳ�Ӱ��״̬��Ϣ
     */
	void Destory();

	/**
	 * @brief ��ȡ��������״̬
	 * @param stSTcpConnStatusHeader ״̬ͷ����һЩ��Ϣ��ע��uiConnStatusNum��astSTcpConnStatus����Ч��
	 * @note ���и������ϵ����ӣ��������Ǹ�APIʹ�õ�����
	 *       ���øýӿڿ��Բ���ʼ��
	 */
    int GetAllStatus(STcpConnStatusHeader &stSTcpConnStatusHeader, std::vector<STcpConnStatus> &vstSTcpConnStatus);

    /**
     * @brief ��ȡ��API�����ĸ�������״̬
     */
    int GetMyStatus(std::vector<STcpConnStatus> &vstSTcpConnStatus);

    /**
     * @brief ����ĳ��IP/�˿ڶ�Ӧ��״̬
     * @note ���øýӿڿ��Բ���ʼ��
     */
    int SetConnStatus(const STcpConnStatus &stSTcpConnStatus);

    /**
     * @brief ɾ��ĳ��IP/�˿ڶ�Ӧ��״̬
     * @note ���øýӿڿ��Բ���ʼ��
     */
    int DelConnStatus(const STcpConnStatus &stSTcpConnStatus);

    /**
     * @brief ��ȡ������Ϣ
     */
	const char *GetErrMsg()
	{
		return m_szErrMsg;
	}

private:
	int InitStatusShm();
	STcpConnStatus *GetSTcpConnStatus(unsigned int uiIPAddr, unsigned short ushPort);
	STcpConnStatus *AddSTcpConnStatus(unsigned int uiIPAddr, unsigned short ushPort);
	int DelConnStatusInternal(unsigned int uiIP, unsigned short ushPort);
    void SetSTcpConnStatusInternal(unsigned short ushConnStatus);

    int PollWait(int iFd, short shEvent, int iTimeOut);
	int RecvWait(int iFd, int iTimeOut);
	int SendWait(int iFd, int iTimeOut);

    int ConnectInternal(int iTestConnFlag = 0);
	int GetConnection();

    int SendInternal(const void *pszBuff, unsigned int uiLen);
    int RecvInternal(void *pszBuff, unsigned int *puiLen, unsigned int uiExpectLen);

private:
	STcpConnData m_astSTcpConnData[STCP_MAX_CONN_DATA];
	int m_iSTcpConnDataNum; //�洢��STcp�������ݵĸ���
	int m_iValidConnNum;    //��Ч��������
    int m_iTimeOut;
	char m_szErrMsg[1024];

	char *m_pStatusMem;
	STcpConnStatusHeader *m_pstSTcpConnStatus;
	STcpConnStatus *m_pstCurConnStatus;
	STcpConnData *m_pstCurConnData;

	int m_iRetryInterval;
    int m_iPeerTimeOut;

	CShareMem m_objStatusMem;

	STcpConnStatus m_stFullConnStatus;  //��������ڴ��е�״̬�ڵ����ˣ�����ʹ�øýڵ�����߼�����

};

}
#endif
