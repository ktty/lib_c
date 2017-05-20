#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/poll.h>
#include <arpa/inet.h>
#include <fcntl.h>

//petlogֻ������һЩ��������޸������Դ����������ⲿ�ӿڲ��񲻵���һЩ��Ϣ���������һЩ������Ϣ�����
//�ܹ�ͨ��ErrMsg�������Ϣ������ͨ��petlog���
#include "comm/log/pet_log.h"
#include "comm/ini_file/ini_file.h"
#include "comm/util/pet_util.h"

/**
 * �����ڴ�ṹ˵����
 * 1����̨���������е�������������"��ַ+�˿�"ȫ����¼��ͬһ�Ĺ����ڴ��У��ڴ��KEYд��
 * 2�����ǵ���̨�������ӵ���������"��ַ+�˿�"���������̫�࣬��ʹ�ø��ӵ����ݽṹ���������״̬�����ӵ����ݽṹһ����
 *    ���׳�����һ������ÿ�γ�ʼ����ʱ����ҪУ�飬���ڶ����ӵ��������һ���ļ�����
 * 3�����ڳ�ʱ��û�����������������nСʱ�������ڵ�ɾ�����´��������Ҫ�շ����ݣ��ٽ��в����OK
 * 4���������ݷ��ʲ�������Ϊ�˱�֤�����׼ȷ�ԣ������������¼��㣺
 *    a���м�ڵ�ɾ����ֻ�ǽ����һ���ڵ㿽������ɾ���Ľڵ��ϣ����һ������Ϊ�޸Ľڵ�����
 *    b���������ͬʱ����ڵ㣬����ֻ��า�ǵ����⣬�ᵼ�����������
 *       ��Ҫ��"IP/PORT"û�в��ȥ������������´�ѭ�����Զ��޸�
 *       ����Ҫ��"IP/PORT"�����ˣ������������һ��ʱ���Ժ�ɾ��
 *       ��Ӧ"IP/PORT"��״̬�����ͳ�����ݲ���ȷ�ˣ��������������һ��ѭ������������������Զ��޸�
 *    c���������ʱ����Ŀ��"IP/PORT"�鵽��Ӧ�Ľڵ㣬�κ��޸�ֻ���޸ĸýڵ���ص�״̬��ͳ�����ݣ������޸�IP�Ͷ˿�
 *    d��������ֲ����ڵ�"IP/PORT"����Ҫ���ýڵ���룬���밴�������ӽڵ���������д�ڵ��ԭ�򣬿��ܻᵼ�²���������ͬ�Ľڵ㣬�ڶ����ڵ�һ��ʱ���Ժ�ᱻɾ��
 * 5����ּ�������о�����֤���ݵ��ȶ���ҲҪ��֤������Ϊ�����ݵ����⵼�������շ����󣬲�����Ϊ�ù����ڴ�Ĵ��ڵ��³����ȶ����κεĴ�����֣�APIҪ��֤�пɿ������޸�����
 *
 * ����ṹ��
 * ----------------------------------------------------------------
 * |             STcpConnStatusHeader 136Byte (IA64)               |
 * ----------------------------------------------------------------
 * |                  STcpConnStatus1 136Byte                     |
 * |                  STcpConnStatus2 136Byte                     |
 * |                  STcpConnStatus3 136Byte                     |
 * |                           ....                               |
 * |                  STcpConnStatusn 136Byte                     |
 * ----------------------------------------------------------------
 *
 *
 * ����˵����
 * 1�������ж��ٸ����ӣ���һ�γ�ʼ��ʱ�������ӶԶˣ�ֻ�����շ����ݵ�ʱ��������ӣ��������õ��ĸ����ӣ������ĸ�����֤������Ӧ�õ�Ч�ʡ�
 *
 */

#include "comm/safe_tcp_client/safe_tcp_client.h"

using namespace snslib;

const int STCP_MAX_CONN_STATUS_KEEP_TIME = 10 * 3600;   //���û�����󾭹�ʱ������״̬��ౣ��10Сʱ��������������

CSafeTcpClient::CSafeTcpClient()
{
    memset(m_astSTcpConnData, 0x0, sizeof(m_astSTcpConnData));
    for(int i=0; i<STCP_MAX_CONN_DATA; i++)
    {
        m_astSTcpConnData[i].iSocket = -1;
    }

    m_iSTcpConnDataNum = 0;
    m_iValidConnNum = 0;
    m_iTimeOut = STCP_DEFAULT_TIMEOUT;
    m_iPeerTimeOut = STCP_DEFAULT_PEERTIMEOUT;
    memset(m_szErrMsg, 0x0, sizeof(m_szErrMsg));

    m_pstCurConnStatus = NULL;
    m_pstCurConnData = NULL;
    m_pStatusMem = NULL;
    m_pstSTcpConnStatus = NULL;

    m_iRetryInterval = STCP_RETRY_TIME_INTER_DEFAULT;
    memset(&m_stFullConnStatus, 0, sizeof(m_stFullConnStatus));
    m_stFullConnStatus.ushStatus = STCP_CONN_STATUS_E_FULL;
}

CSafeTcpClient::~CSafeTcpClient()
{
    Destory();
}

/**
 * @brief ʹ�������ļ���ʼ��
 */
int CSafeTcpClient::Init(const char *pszConfFile, const char *pszSecName/* = "SAFE_TCP_CLI"*/)
{
    CIniFile objIniFile(pszConfFile);

    if (objIniFile.IsValid())
    {
        int iRetryInterval = 0;
        int iTimeOut = 0;
        int iPeerTimeOut = 0;

        char szSTcpAddrList[1024] = {0};
        int iSTcpPort = 0;

        objIniFile.GetString(pszSecName, "HostList", "", szSTcpAddrList, sizeof(szSTcpAddrList));
        objIniFile.GetInt(pszSecName, "Port", 0, &iSTcpPort);
        objIniFile.GetInt(pszSecName, "RetryInterval", 0, &iRetryInterval);
        objIniFile.GetInt(pszSecName, "TimeOut", 0, &iTimeOut);
        objIniFile.GetInt(pszSecName, "PeerTimeOut", STCP_DEFAULT_PEERTIMEOUT, &iPeerTimeOut);

        if (szSTcpAddrList[0] == '\0')
        {
            std::vector<STcpAddr> vstSTcpAddrList;
            STcpAddr stSTcpAddr;
            int iSTcpAddrNum =0;

            //���ֶζ�ȡHostX��PortX
            objIniFile.GetInt(pszSecName, "HostNum", 0, &iSTcpAddrNum);
            if (iSTcpAddrNum <= 0 || iSTcpAddrNum > STCP_MAX_CONN_DATA)
            {
                snprintf(m_szErrMsg, sizeof(m_szErrMsg), "init fail: conf %s/HostNum [%d] is not valid", pszSecName, iSTcpAddrNum);
                return -1;
            }

            char szValName[64] = {0};
            char szIPAddr[64] = {0};
            for(int i=0; i<iSTcpAddrNum; i++)
            {
                snprintf(szValName, sizeof(szValName), "Host%d", i+1);
                objIniFile.GetString(pszSecName, szValName, "", szIPAddr, sizeof(szIPAddr));
                stSTcpAddr.uiIPAddr = inet_addr(szIPAddr);

                snprintf(szValName, sizeof(szValName), "Port%d", i+1);
                objIniFile.GetInt(pszSecName, szValName, 0, (int *)&stSTcpAddr.ushPort);

                if (stSTcpAddr.uiIPAddr == 0 || stSTcpAddr.ushPort == 0)
                {
                    snprintf(m_szErrMsg, sizeof(m_szErrMsg), "init fail: conf %s/Host%d [%s] or %s/Port%d [%d] is not valid", pszSecName, i+1, szIPAddr, pszSecName, i+1, stSTcpAddr.ushPort);
                    return -1;
                }

                vstSTcpAddrList.push_back(stSTcpAddr);
            }

            return Init(vstSTcpAddrList, iTimeOut, iRetryInterval, iPeerTimeOut);
        }
        else
        {
            return Init(szSTcpAddrList, iSTcpPort, iTimeOut, iRetryInterval, iPeerTimeOut);
        }
    }
    else
    {
        snprintf(m_szErrMsg, sizeof(m_szErrMsg), "init fail: conf file[%s] is not valid", pszSecName);
        return -1;
    }

    return 0;
}

/**
 * @brief ʹ�ö��IP��ַ����ͬ�Ķ˿ڳ�ʼ��
 */
int CSafeTcpClient::Init(const char * pszIPList, int iPort, int iTimeOut/* = STCP_DEFAULT_TIMEOUT*/, int iRetryInterval/* = STCP_RETRY_TIME_INTER_DEFAULT*/, int iPeerTimeOut/* =STCP_DEFAULT_PEERTIMEOUT */)
{
    std::vector<STcpAddr> vstSTcpAddrList;
    STcpAddr stSTcpAddr;

    char szAddrList[1024];
    snprintf(szAddrList, sizeof(szAddrList), "%s", pszIPList);
    char *pszOneIPAddr = NULL;
    int iCurAddrNum = 0;
    for(char *pszSecVal = szAddrList; (pszOneIPAddr = strtok(pszSecVal, ",")) != NULL; pszSecVal=NULL)
    {
        stSTcpAddr.uiIPAddr = inet_addr(pszOneIPAddr);
        stSTcpAddr.ushPort = iPort;
        if (stSTcpAddr.uiIPAddr == 0||stSTcpAddr.ushPort == 0)
        {
            snprintf(m_szErrMsg, sizeof(m_szErrMsg), "init fail: addr_list[%s] or port [%d] is not valid", szAddrList, stSTcpAddr.ushPort);
            return -1;
        }

        iCurAddrNum++;
        if (iCurAddrNum >= STCP_MAX_CONN_DATA)
        {
            snprintf(m_szErrMsg, sizeof(m_szErrMsg), "init fail: addr_list[%s] is not valid, addr_num[%d] is too big", szAddrList, iCurAddrNum);
            return -1;
        }

        vstSTcpAddrList.push_back(stSTcpAddr);
    }

    return Init(vstSTcpAddrList, iTimeOut, iRetryInterval, iPeerTimeOut);
}

/**
 * @brief ʹ�ö��IP��ַ����ͬ�Ķ˿ڳ�ʼ��
 */
int CSafeTcpClient::Init(const std::vector<STcpAddr> &vstSTcpAddrList, int iTimeOut/* = STCP_DEFAULT_TIMEOUT*/, int iRetryInterval/* = STCP_RETRY_TIME_INTER_DEFAULT*/, int iPeerTimeOut/* =STCP_DEFAULT_PEERTIMEOUT */)
{
    int iRetVal = 0;

    //��ֹ���Init�����й©
    Destory();

    if (vstSTcpAddrList.size() > (unsigned int)STCP_MAX_CONN_DATA)
    {
        snprintf(m_szErrMsg, sizeof(m_szErrMsg), "init fail: addr list is too big, max[%d], now[%d]", (int)vstSTcpAddrList.size(), STCP_MAX_CONN_DATA);
        return -1;
    }

    m_iSTcpConnDataNum = 0;
    std::vector<STcpAddr>::const_iterator pstSTcpAddr = vstSTcpAddrList.begin();
    for(; pstSTcpAddr != vstSTcpAddrList.end(); pstSTcpAddr++)
    {
        if ((pstSTcpAddr->uiIPAddr == 0) || (pstSTcpAddr->ushPort == 0))
        {
            snprintf(m_szErrMsg, sizeof(m_szErrMsg), "init fail: addr list is not valid, ip[%s], port[%d]", inet_ntoa(*(struct in_addr*)(&pstSTcpAddr->uiIPAddr)), pstSTcpAddr->ushPort);
            return -1;
        }

        m_astSTcpConnData[m_iSTcpConnDataNum].uiIPAddr = pstSTcpAddr->uiIPAddr;
        m_astSTcpConnData[m_iSTcpConnDataNum].ushPort = pstSTcpAddr->ushPort;
        m_astSTcpConnData[m_iSTcpConnDataNum].iSocket = -1;

        m_iSTcpConnDataNum++;
    }

    m_iTimeOut = iTimeOut;
    m_iRetryInterval = iRetryInterval;
    m_iPeerTimeOut = iPeerTimeOut;

    if (m_iRetryInterval > (STCP_MAX_CONN_STATUS_KEEP_TIME / 5))
    {
        m_iRetryInterval = (STCP_MAX_CONN_STATUS_KEEP_TIME / 60);    //��ֹ����״̬�����
    }

    iRetVal = InitStatusShm();

    if (iRetVal != 0)
    {
        return iRetVal;
    }

    return 0;
}

int CSafeTcpClient::InitStatusShm()
{
    int iRetVal = 0;

    //��ʼ�������ڴ�
    int iShmExistFlag = 0;  //�����ڴ���ڱ�־λ 0-������ 1-����
    iRetVal = m_objStatusMem.Create(STCP_CONN_STATUS_SHM_KEY, STCP_CONN_STATUS_SHM_SIZE, 0666);
    if (iRetVal == m_objStatusMem.SUCCESS)
    {
        //�ڴ治���ڣ����´�����
        iShmExistFlag = 0;
    }
    else if (iRetVal == m_objStatusMem.SHM_EXIST)
    {
        //�ڴ����
        iShmExistFlag = 1;
    }
    else
    {
        snprintf(m_szErrMsg, sizeof(m_szErrMsg), "init fail: status_shm create failed, ret=%d", iRetVal);
        return -1;
    }

    iRetVal = m_objStatusMem.Attach();
    if (iRetVal != m_objStatusMem.SUCCESS)
    {
        snprintf(m_szErrMsg, sizeof(m_szErrMsg), "init fail: status_shm attach failed, ret=%d", iRetVal);
        return -1;
    }

    m_pStatusMem = (char *)m_objStatusMem.GetMem();
    if (m_pStatusMem == (char *)-1)
    {
        snprintf(m_szErrMsg, sizeof(m_szErrMsg), "init fail: status_shm getmem failed");
        return -1;
    }

    m_pstSTcpConnStatus = (STcpConnStatusHeader *)m_pStatusMem;

    if (iShmExistFlag == 0||m_pstSTcpConnStatus->uiMagicNum != (unsigned int)STCP_STATUS_MEM_MAGICNUM )
    {
        PetLog(0, 0, PETLOG_WARN, "%s|%d stcp_status_mem cleared, exist_flag=%d, magic_num=0x%08X", __func__, __LINE__, iShmExistFlag, m_pstSTcpConnStatus->uiMagicNum);
        memset(m_pStatusMem, 0x0, STCP_CONN_STATUS_SHM_SIZE);
        m_pstSTcpConnStatus->ushVersion = STCP_STATUS_MEM_VERSION;
        m_pstSTcpConnStatus->uiMagicNum = STCP_STATUS_MEM_MAGICNUM;
        m_pstSTcpConnStatus->tCreateTime = time(NULL);
    }

    return 0;
}

/**
 * @brief ���ó�ʱʱ�䣬��λ����
 */
int CSafeTcpClient::SetTimeOut(int iTimeOut)
{
    m_iTimeOut = iTimeOut;
    return 0;
}

STcpConnStatus *CSafeTcpClient::GetSTcpConnStatus(unsigned int uiIPAddr, unsigned short ushPort)
{
    if (m_pstSTcpConnStatus->uiConnStatusNum > (unsigned int)STCP_MAX_CONN_STATUS)
    {
        m_pstSTcpConnStatus->uiConnStatusNum = STCP_MAX_CONN_STATUS;
    }

    for(unsigned int i=0; i<m_pstSTcpConnStatus->uiConnStatusNum; i++)
    {
        if ((m_pstSTcpConnStatus->astSTcpConnStatus[i].uiIPAddr == uiIPAddr) && (m_pstSTcpConnStatus->astSTcpConnStatus[i].ushPort == ushPort))
        {
            return &(m_pstSTcpConnStatus->astSTcpConnStatus[i]);
        }
    }

    return NULL;
}

STcpConnStatus *CSafeTcpClient::AddSTcpConnStatus(unsigned int uiIPAddr, unsigned short ushPort)
{
    //����Ѿ��������ݣ�ʹ��ԭ�е�����
    STcpConnStatus *pstRetConnStatus = GetSTcpConnStatus(uiIPAddr, ushPort);
    if (pstRetConnStatus != NULL)
    {
        return pstRetConnStatus;
    }

    if (m_pstSTcpConnStatus->uiConnStatusNum >= (unsigned int)STCP_MAX_CONN_STATUS)
    {
        PetLog(0, 0, PETLOG_WARN, "STCP:tcp_conn_data full, ip=%s, port=%d", inet_ntoa(*((struct in_addr*)&uiIPAddr)), ushPort);
        m_pstSTcpConnStatus->uiStatusFullNum++;
        m_stFullConnStatus.uiIPAddr = uiIPAddr;
        m_stFullConnStatus.ushPort = ushPort;
        m_stFullConnStatus.ushStatus = STCP_CONN_STATUS_E_FULL;
        return &m_stFullConnStatus;
    }

    int iConnStatusIDX = m_pstSTcpConnStatus->uiConnStatusNum;
    m_pstSTcpConnStatus->uiConnStatusNum++;

    memset(&m_pstSTcpConnStatus->astSTcpConnStatus[iConnStatusIDX], 0, sizeof(STcpConnStatus));
    m_pstSTcpConnStatus->astSTcpConnStatus[iConnStatusIDX].uiIPAddr = uiIPAddr;
    m_pstSTcpConnStatus->astSTcpConnStatus[iConnStatusIDX].ushPort = ushPort;
    m_pstSTcpConnStatus->astSTcpConnStatus[iConnStatusIDX].uiTimeOut = m_iTimeOut;
    m_pstSTcpConnStatus->astSTcpConnStatus[iConnStatusIDX].uiLastReqNum = 1;    //��ֹ��ɾ��

    return &(m_pstSTcpConnStatus->astSTcpConnStatus[iConnStatusIDX]);
}

int CSafeTcpClient::DelConnStatusInternal(unsigned int uiIPAddr, unsigned short ushPort)
{
    if (m_pstSTcpConnStatus->uiConnStatusNum > (unsigned int)STCP_MAX_CONN_STATUS)
    {
        m_pstSTcpConnStatus->uiConnStatusNum = STCP_MAX_CONN_STATUS;
    }

    for(unsigned int i=0; i<m_pstSTcpConnStatus->uiConnStatusNum; i++)
    {
        if ((m_pstSTcpConnStatus->astSTcpConnStatus[i].uiIPAddr == uiIPAddr) && (m_pstSTcpConnStatus->astSTcpConnStatus[i].ushPort == ushPort))
        {
            memcpy(&m_pstSTcpConnStatus->astSTcpConnStatus[i], &m_pstSTcpConnStatus->astSTcpConnStatus[m_pstSTcpConnStatus->uiConnStatusNum], sizeof(STcpConnStatus));
            m_pstSTcpConnStatus->uiConnStatusNum--;
            break;
        }
    }

    return 0;
}

//��m_pstCurConnDataָ������ӽ������ӻ�����
//iTestConnFlag�����ڱ�ʶ���������Ƿ�ֻ��̽�����ӵ���Ч�ԣ������̽�⣬�����ĳ�ʱʱ������ͨ��ʱʱ���1/5
int CSafeTcpClient::ConnectInternal(int iTestConnFlag/* = 0 */)
{
    int iRetVal = 0;
    if (m_pstCurConnData == NULL)
    {
        snprintf(m_szErrMsg, sizeof(m_szErrMsg), "internal err:%s: conn_data is null", __func__);
        return -1;
    }

    if (m_pstCurConnData->iSocket != -1)
    {
        close(m_pstCurConnData->iSocket);
        m_pstCurConnData->iSocket = -1;
    }

    m_pstCurConnData->iSocket = socket(PF_INET, SOCK_STREAM, 0);
    if (m_pstCurConnData->iSocket == -1)
    {
        snprintf(m_szErrMsg, sizeof(m_szErrMsg), "internal err: create socket err in ConnectInternal, errno=%d, errmsg=%s", errno, strerror(errno));
        return -1;
    }

    struct sockaddr_in stSockAddr;

    stSockAddr.sin_family = AF_INET;
    stSockAddr.sin_port = htons(m_pstCurConnData->ushPort);
    stSockAddr.sin_addr.s_addr = m_pstCurConnData->uiIPAddr;

    //����Ϊ������ģʽ
    fcntl( m_pstCurConnData->iSocket, F_SETFL, O_NONBLOCK);

    iRetVal = connect(m_pstCurConnData->iSocket, (struct sockaddr *)&stSockAddr, sizeof(stSockAddr));
    if ((iRetVal == -1) && (errno != EINPROGRESS))
    {
        snprintf(m_szErrMsg, sizeof(m_szErrMsg), "connect err: ip=%s, port=%d, errno=%d, errmsg=%s", inet_ntoa(*((struct in_addr*)&m_pstCurConnData->uiIPAddr)), m_pstCurConnData->ushPort, errno, strerror(errno));
        return -1;
    }

    int iConnTimeOut = m_iTimeOut;
    if (iTestConnFlag == 1)
    {
        //TODO ������Ը���ʵ��������е���
        iConnTimeOut = iConnTimeOut/5;
    }
    iRetVal = SendWait(m_pstCurConnData->iSocket, iConnTimeOut);
    if (iRetVal != 0)
    {
        return -1;
    }

    return 0;
}

int CSafeTcpClient::GetConnection()
{
    if (m_pstSTcpConnStatus == NULL)
    {
        snprintf(m_szErrMsg, sizeof(m_szErrMsg), "internal err:%s: conn_status is null,maybe not init or destoried", __func__);
        return -1;
    }

    int iRetVal = 0;
    int iRandomConnIDX = CRandomTool::Instance()->Get(0, m_iSTcpConnDataNum);
    time_t tTimeNow = time(NULL);

    //TODO ��Ҫ����������״̬�����ݽ��м�飬Ƶ�ʴ��10Сʱ���һ�Σ���ʮСʱ��û���շ����ݵ�����״̬�����
    if ((m_pstSTcpConnStatus->tLastCheckTime > (tTimeNow + 600)) || (m_pstSTcpConnStatus->tLastCheckTime < (tTimeNow - STCP_MAX_CONN_STATUS_KEEP_TIME)))
    {
        m_pstSTcpConnStatus->tLastCheckTime = tTimeNow;

        if (m_pstSTcpConnStatus->uiConnStatusNum > (unsigned int)STCP_MAX_CONN_STATUS)
        {
            m_pstSTcpConnStatus->uiConnStatusNum = STCP_MAX_CONN_STATUS;
        }

        for(unsigned int i=0; i<m_pstSTcpConnStatus->uiConnStatusNum; i++)
        {
            if (m_pstSTcpConnStatus->astSTcpConnStatus[i].uiLastReqNum == 0)
            {
                //���ϴμ�鵽���ڵ���������Ϊ0����Ҫ��������״̬ɾ���������ӻ�����һ�α���⵽
                memcpy(&m_pstSTcpConnStatus->astSTcpConnStatus[i], &m_pstSTcpConnStatus->astSTcpConnStatus[m_pstSTcpConnStatus->uiConnStatusNum], sizeof(STcpConnStatus));
                m_pstSTcpConnStatus->uiConnStatusNum--;
            }
            else
            {
                m_pstSTcpConnStatus->astSTcpConnStatus[i].uiLastReqNum = 0;
            }
        }
    }

    int iRetryTimes = 0;
    //TODO �����������ʧЧm_iSTcpConnDataNum/2 + 1�����ӣ�����������������ж��������ʱ��ֱ������
    int iMaxRetryTimes = m_iSTcpConnDataNum/2 + 1;
    for (; iRetryTimes < iMaxRetryTimes; iRetryTimes++)
    {
        m_pstCurConnData = &(m_astSTcpConnData[iRandomConnIDX]);
        m_pstCurConnStatus = AddSTcpConnStatus(m_pstCurConnData->uiIPAddr, m_pstCurConnData->ushPort);

        if (m_pstCurConnData->iSocket == -1)
        {
            if (m_pstCurConnStatus->ushStatus == STCP_CONN_STATUS_E_CONN
                ||m_pstCurConnStatus->ushStatus == STCP_CONN_STATUS_E_SEND)
            {
                //��������ʧ�ܣ����߷���ʧ�ܣ���Ҫ����״̬���г���
                if ((tTimeNow - m_pstCurConnStatus->tLastRetryTime) < 0||(tTimeNow - m_pstCurConnStatus->tLastRetryTime) > m_iRetryInterval)
                {
                    iRetVal = ConnectInternal(1);
                    if (iRetVal == 0)
                    {
                        //���ӳɹ�����������ں������ݲ�������Ժ����״̬����
                        PetLog(0, 0, PETLOG_INFO, "STCP:try reconnect succ, ip=%s, port=%d", inet_ntoa(*((struct in_addr*)&m_pstCurConnData->uiIPAddr)), m_pstCurConnData->ushPort)
                        break;
                    }
                    else
                    {
                        //�޸�״̬����
                        PetLog(0, 0, PETLOG_WARN, "STCP:try reconnect fail, errmsg=%s", m_szErrMsg)
                        PetLog(0, 0, PETLOG_WARN, "STCP:try reconnect fail, conn_err_num=%d, retry_times=%d, ip=%s, port=%d", m_pstCurConnStatus->uiConnErrNum, m_pstCurConnStatus->uiRetryTimes, inet_ntoa(*((struct in_addr*)&m_pstCurConnData->uiIPAddr)), m_pstCurConnData->ushPort)
                        SetSTcpConnStatusInternal(STCP_CONN_STATUS_E_CONN);
                    }
                }
            }
            else
            {
                //�״ν������ӣ����߲�ͨ��Status�����ڴ���ƣ���ʱʱ������
                iRetVal = ConnectInternal();
                if (iRetVal == 0)
                {
                    PetLog(0, 0, PETLOG_INFO, "STCP:connect succ, ip=%s, port=%d", inet_ntoa(*((struct in_addr*)&m_pstCurConnData->uiIPAddr)), m_pstCurConnData->ushPort);
                    break;
                }
                else
                {
                    //���ش���
                    //�޸�״̬����
                    PetLog(0, 0, PETLOG_WARN, "STCP:connect fail, errmsg=%s", m_szErrMsg)
                    PetLog(0, 0, PETLOG_WARN, "STCP:connect fail, conn_err_num=%d, retry_times=%d, ip=%s, port=%d", m_pstCurConnStatus->uiConnErrNum, m_pstCurConnStatus->uiRetryTimes, inet_ntoa(*((struct in_addr*)&m_pstCurConnData->uiIPAddr)), m_pstCurConnData->ushPort)
                    SetSTcpConnStatusInternal(STCP_CONN_STATUS_E_CONN);
                    return -1;
                }
            }
        }
        else if ((tTimeNow - m_pstCurConnData->tLastUseTime) < 0 || tTimeNow - m_pstCurConnData->tLastUseTime > m_iPeerTimeOut) 
        {
                iRetVal = ConnectInternal(1);
                if (iRetVal == 0)
                {
                        //���ӳɹ�����������ں������ݲ�������Ժ����״̬����
                        PetLog(0, 0, PETLOG_INFO, "I thought peer tcp connection has closed, so reconnect");
                        PetLog(0, 0, PETLOG_INFO, "STCP:try reconnect succ, ip=%s, port=%d", inet_ntoa(*((struct in_addr*)&m_pstCurConnData->uiIPAddr)), m_pstCurConnData->ushPort)
                                break;
                }
                else
                {
                        //�޸�״̬����
                        PetLog(0, 0, PETLOG_WARN, "STCP:try reconnect fail, errmsg=%s", m_szErrMsg)
                                PetLog(0, 0, PETLOG_WARN, "STCP:try reconnect fail, conn_err_num=%d, retry_times=%d, ip=%s, port=%d", m_pstCurConnStatus->uiConnErrNum, m_pstCurConnStatus->uiRetryTimes, inet_ntoa(*((struct in_addr*)&m_pstCurConnData->uiIPAddr)), m_pstCurConnData->ushPort)
                                SetSTcpConnStatusInternal(STCP_CONN_STATUS_E_CONN);
                }
        }
        else
        {
            //ֻҪ�����������������Ƿ�״̬�����⣬�����շ�����
            break;
        }

        //������ָ����Ӳ����ã�Ҳ������������ʹ���¸�����
        iRandomConnIDX = (iRandomConnIDX+1)%m_iSTcpConnDataNum;
    }

    if (iRetryTimes >= iMaxRetryTimes)
    {
        //ǿ�ƶ�������Ч�����������һ����������
        iRandomConnIDX = CRandomTool::Instance()->Get(0, iMaxRetryTimes);
        int iForceConnIdx = ((m_pstCurConnData - m_astSTcpConnData) + m_iSTcpConnDataNum - iRandomConnIDX) % m_iSTcpConnDataNum;
        m_pstCurConnData = &(m_astSTcpConnData[iForceConnIdx]);
        m_pstCurConnStatus = AddSTcpConnStatus(m_pstCurConnData->uiIPAddr, m_pstCurConnData->ushPort);

        PetLog(0, 0, PETLOG_WARN, "STCP:force reconnect svr, ip=%s, port=%d", inet_ntoa(*((struct in_addr*)&m_pstCurConnData->uiIPAddr)), m_pstCurConnData->ushPort)

        iRetVal = ConnectInternal();
        if (iRetVal == 0)
        {
            PetLog(0, 0, PETLOG_INFO, "STCP:force reconnect succ, ip=%s, port=%d", inet_ntoa(*((struct in_addr*)&m_pstCurConnData->uiIPAddr)), m_pstCurConnData->ushPort);
        }
        else
        {
            //���ش���
            //�޸�״̬����
            PetLog(0, 0, PETLOG_WARN, "STCP:force reconnect fail, errmsg=%s", m_szErrMsg)
            PetLog(0, 0, PETLOG_WARN, "STCP:force reconnect fail, conn_err_num=%d, retry_times=%d, ip=%s, port=%d", m_pstCurConnStatus->uiConnErrNum, m_pstCurConnStatus->uiRetryTimes, inet_ntoa(*((struct in_addr*)&m_pstCurConnData->uiIPAddr)), m_pstCurConnData->ushPort)
            SetSTcpConnStatusInternal(STCP_CONN_STATUS_E_CONN);
            return -1;
        }
    }

    return 0;
}


int CSafeTcpClient::PollWait(int iFd, short shEvent, int iTimeOut)
{
    struct pollfd stPoll;

    stPoll.fd = iFd;
    stPoll.events = shEvent;
    stPoll.revents = 0;

    while(true)
    {
        switch (poll(&stPoll, 1, iTimeOut))
        {
        case -1:
            if (errno != EINTR)
            {
                snprintf(m_szErrMsg, sizeof(m_szErrMsg), "pool fail: errno=%d, errmsg=%s, timeout[%d], host=%s, port=%d", errno, strerror(errno), iTimeOut, inet_ntoa(*((struct in_addr*)&m_pstCurConnData->uiIPAddr)), m_pstCurConnData->ushPort);
                return -1;
            }
            continue;

        case 0:
            snprintf(m_szErrMsg, sizeof(m_szErrMsg), "poll fail: timeout[%d], host=%s, port=%d", iTimeOut, inet_ntoa(*((struct in_addr*)&m_pstCurConnData->uiIPAddr)), m_pstCurConnData->ushPort);
            return -1;

        default:
            if (stPoll.revents & POLLHUP)
            {
                snprintf(m_szErrMsg, sizeof(m_szErrMsg), "poll fail: revents POLLHUP, timeout[%d], host=%s, port=%d", iTimeOut, inet_ntoa(*((struct in_addr*)&m_pstCurConnData->uiIPAddr)), m_pstCurConnData->ushPort);
                return -1;
            }
            if (stPoll.revents & POLLERR)
            {
                snprintf(m_szErrMsg, sizeof(m_szErrMsg), "poll fail: revents POLLERR, timeout[%d], host=%s, port=%d", iTimeOut, inet_ntoa(*((struct in_addr*)&m_pstCurConnData->uiIPAddr)), m_pstCurConnData->ushPort);
                return -1;
            }
            if (stPoll.revents & POLLNVAL)
            {
                snprintf(m_szErrMsg, sizeof(m_szErrMsg), "poll fail: revents POLLNVAL, timeout[%d], host=%s, port=%d", iTimeOut, inet_ntoa(*((struct in_addr*)&m_pstCurConnData->uiIPAddr)), m_pstCurConnData->ushPort);
                return -1;
            }
            if (stPoll.revents & shEvent)
            {
                return 0;
            }
            else
            {
                snprintf(m_szErrMsg, sizeof(m_szErrMsg), "poll fail: revents other error, timeout[%d], host=%s, port=%d", iTimeOut, inet_ntoa(*((struct in_addr*)&m_pstCurConnData->uiIPAddr)), m_pstCurConnData->ushPort);
                return -1;
            }
        }
    }
}

int CSafeTcpClient::RecvWait(int iFd, int iTimeOut)
{
    return PollWait(iFd, POLLIN, iTimeOut);
}

int CSafeTcpClient::SendWait(int iFd, int iTimeOut)
{
    return PollWait(iFd, POLLOUT, iTimeOut);
}

/**
 * @brief �����ݷ��͵�m_pstCurConnDataָ���socket��
 */
int CSafeTcpClient::SendInternal(const void *pszBuff, unsigned int uiLen)
{
    if ((m_pstCurConnData == NULL) || (m_pstCurConnStatus == NULL))
    {
        snprintf(m_szErrMsg, sizeof(m_szErrMsg), "internal err:%s: conn_data or conn_status is null", __func__);
        return -1;
    }

    unsigned int uiBytesSent = 0;
    int iWriteBytes = 0;

    while(true)
    {
        if (SendWait(m_pstCurConnData->iSocket, m_iTimeOut) != 0)
        {
            //�޸�����״̬����
            SetSTcpConnStatusInternal(STCP_CONN_STATUS_E_SEND);
            return -1;
        }

        iWriteBytes = write(m_pstCurConnData->iSocket, (unsigned char *)pszBuff + uiBytesSent, uiLen - uiBytesSent);
        if (iWriteBytes > 0)
        {
            uiBytesSent += iWriteBytes;
            if (uiBytesSent < uiLen) continue;
            else break;
        }
        else if (errno == EINTR)
        {
            continue;
        }
        else
        {
            break;
        }
    }

    if (iWriteBytes <= 0)
    {
        snprintf(m_szErrMsg, sizeof(m_szErrMsg), "send err: ip=%s, port=%d, ret=%d, errno=%d, errmsg=%s", inet_ntoa(*((struct in_addr*)&m_pstCurConnData->uiIPAddr)), m_pstCurConnData->ushPort, iWriteBytes, errno, strerror(errno));
        SetSTcpConnStatusInternal(STCP_CONN_STATUS_E_SEND);
        return -1;
    }

    return 0;
}


/**
 * @brief �����ݷ��͵�m_pstCurConnDataָ���socket��
 */
int CSafeTcpClient::RecvInternal(void *pszBuff, unsigned int *puiLen, unsigned int uiExpectLen)
{
    if ((m_pstCurConnData == NULL) || (m_pstCurConnStatus == NULL))
    {
        snprintf(m_szErrMsg, sizeof(m_szErrMsg), "internal err:%s: conn_data or conn_status is null", __func__);
        return -1;
    }

    unsigned int uiBytesReceived = 0;
    int iReadBytes = 0;

    while (true)
    {
        if (0 != RecvWait(m_pstCurConnData->iSocket, m_iTimeOut))
        {
            //�޸�����״̬����
            SetSTcpConnStatusInternal(STCP_CONN_STATUS_E_RECV);
            return -1;
        }

        if (uiExpectLen > 0)
        {
            iReadBytes = read(m_pstCurConnData->iSocket, (unsigned char *)pszBuff + uiBytesReceived, uiExpectLen - uiBytesReceived);
            if (iReadBytes > 0)
            {
                *puiLen = (uiBytesReceived += iReadBytes);
                if (uiBytesReceived < uiExpectLen) continue;
                else break;
            }
            else if (errno == EINTR)
            {
                continue;
            }
            else
            {
                break;
            }
        }
        else
        {
            iReadBytes = read(m_pstCurConnData->iSocket, pszBuff, *puiLen);
            if (iReadBytes > 0)
            {
                *puiLen = iReadBytes;
                return 0;
            }
            else if (errno == EINTR)
            {
                continue;
            }
            else
            {
                break;
            }
        }
    }

    if (iReadBytes <= 0)
    {
        snprintf(m_szErrMsg, sizeof(m_szErrMsg), "recv err: ip=%s, port=%d, ret=%d, errno=%d, errmsg=%s", inet_ntoa(*((struct in_addr*)&m_pstCurConnData->uiIPAddr)), m_pstCurConnData->ushPort, iReadBytes, errno, strerror(errno));
        SetSTcpConnStatusInternal(STCP_CONN_STATUS_E_RECV);
        return -1;
    }

    return 0;
}

void CSafeTcpClient::SetSTcpConnStatusInternal(unsigned short ushConnStatus)
{
    if ((m_pstCurConnStatus == NULL) || (m_pstCurConnData == NULL))
    {
        snprintf(m_szErrMsg, sizeof(m_szErrMsg), "internal err:%s: conn_data or conn_status is null", __func__);
        return;
    }

    m_pstCurConnStatus->ushStatus = ushConnStatus;

    switch(ushConnStatus)
    {
        case STCP_CONN_STATUS_OK:
        {
            m_pstCurConnStatus->uiLastReqNum++;
            break;
        }
        case STCP_CONN_STATUS_E_CONN:
        {
            m_pstCurConnStatus->uiRetryTimes++;
            m_pstCurConnStatus->uiConnErrNum++;
            m_pstCurConnStatus->tLastRetryTime = time(NULL);
            m_pstCurConnStatus->uiLastReqNum++;

            if (m_pstCurConnData->iSocket != -1)
            {
                close(m_pstCurConnData->iSocket);
                m_pstCurConnData->iSocket = -1;
            }
            break;
        }
        case STCP_CONN_STATUS_E_RECV:
        {
            m_pstCurConnStatus->uiRecvErrNum++;
            if (m_pstCurConnData->iSocket != -1)
            {
                close(m_pstCurConnData->iSocket);
                m_pstCurConnData->iSocket = -1;
            }
            break;
        }
        case STCP_CONN_STATUS_E_SEND:
        {
            m_pstCurConnStatus->uiSendErrNum++;
            if (m_pstCurConnData->iSocket != -1)
            {
                close(m_pstCurConnData->iSocket);
                m_pstCurConnData->iSocket = -1;
            }
            break;
        }
        default:
        {
            break;
        }
    }
    m_pstCurConnData->tLastUseTime = time(NULL);

    return;
}


/**
 * @brief ��������
 */
int CSafeTcpClient::Send(const void *pszBuff, unsigned int uiLen)
{
    int iRetVal = 0;

    if (NULL == pszBuff)
    {
        snprintf(m_szErrMsg, sizeof(m_szErrMsg), "send fail: send buffer is NULL");
        return -1;
    }

    if (uiLen <= 0)
    {
        snprintf(m_szErrMsg, sizeof(m_szErrMsg), "send fail: send_len[%d] is not valid", uiLen);
        return -1;
    }

    //1.��ȡһ����Ч���ӣ�����ConnData��ConnStatusָ�룬�ֱ�洢����Ա��������
    iRetVal = GetConnection();
    if (iRetVal != 0)
    {
        return -1;
    }

    //2.��������
    iRetVal = SendInternal(pszBuff, uiLen);
    if (iRetVal != 0)
    {
        return -1;
    }

    //3.��������״̬
    SetSTcpConnStatusInternal(STCP_CONN_STATUS_OK);

    return 0;
}

/**
 * @brief ��������
 */
int CSafeTcpClient::Recv(void *pszBuff, unsigned int *puiLen, unsigned int uiExpectLen/* = 0*/)
{
    int iRetVal = 0;

    if (NULL == pszBuff)
    {
        snprintf(m_szErrMsg, sizeof(m_szErrMsg), "recv fail: recv buffer is NULL");
        return -1;
    }

    if ((*puiLen <= 0)||((uiExpectLen > 0)&&(uiExpectLen > *puiLen)))
    {
        snprintf(m_szErrMsg, sizeof(m_szErrMsg), "recv fail: recv_buffer_len[%d] expect_len[%d] is not valid", *puiLen, uiExpectLen);
        return -1;
    }

    //1.��ȡһ����Ч���ӣ�����ConnData��ConnStatusָ�룬�ֱ�洢����Ա��������
    //iRetVal = GetConnection();
    //if (iRetVal != 0)
    //{
    //    return -1;
    //}
    //RECV���������һ�����������ݣ��������ϴη������ݵ��Ǹ�����

    //2.��������
    iRetVal = RecvInternal(pszBuff, puiLen, uiExpectLen);
    if (iRetVal != 0)
    {
        return -1;
    }

    //3.��������״̬
    SetSTcpConnStatusInternal(STCP_CONN_STATUS_OK);

    return 0;

}

/**
 * @brief ���Ͳ���������
 */
int CSafeTcpClient::SendAndRecv(const void * pszSendBuff, unsigned int uiSendLen, void * pszRecvBuff, unsigned int *puiRecvLen, unsigned int uiExpectLen /*= 0*/)
{
    int iRetVal = 0;

    if ((NULL == pszSendBuff)||(NULL == pszRecvBuff))
    {
        snprintf(m_szErrMsg, sizeof(m_szErrMsg), "sendrecv fail: send buff or recv buffer is NULL");
        return -1;
    }

    if ((uiSendLen <= 0)||(*puiRecvLen <= 0)||((uiExpectLen > 0)&&(uiExpectLen > *puiRecvLen)))
    {
        snprintf(m_szErrMsg, sizeof(m_szErrMsg), "sendrecv fail: sendlen[%d] recv_buffer_len[%d] expect_len[%d] is not valid", uiSendLen, *puiRecvLen, uiExpectLen);
        return -1;
    }

    //1.��ȡһ����Ч���ӣ�����ConnData��ConnStatusָ�룬�ֱ�洢����Ա��������
    iRetVal = GetConnection();
    if (iRetVal != 0)
    {
        return -1;
    }

    //2.��������
    iRetVal = SendInternal(pszSendBuff, uiSendLen);
    if (iRetVal != 0)
    {
        return -1;
    }

    //3.��������
    iRetVal = RecvInternal(pszRecvBuff, puiRecvLen, uiExpectLen);
    if (iRetVal != 0)
    {
        return -1;
    }

    //4.��������״̬
    SetSTcpConnStatusInternal(STCP_CONN_STATUS_OK);

    return 0;

}

/* *
 * @brief �رյ�ǰ����
 */
void CSafeTcpClient::CloseCurConn()
{
	if (m_pstCurConnData == NULL)
	{
		snprintf(m_szErrMsg, sizeof(m_szErrMsg), "internal err:%s: conn_data is null", __func__);
	}
	else if (m_pstCurConnData->iSocket != -1)
	{
		close(m_pstCurConnData->iSocket);
		m_pstCurConnData->iSocket = -1;
	}
}

/* *
 * @brief �ر���������
 */
void CSafeTcpClient::CloseAllConn()
{
	//�ر���������
	for(int i=0; i<m_iSTcpConnDataNum; i++)
	{
		if (m_astSTcpConnData[i].iSocket != -1)
		{
			close(m_astSTcpConnData[i].iSocket);
			m_astSTcpConnData[i].iSocket = -1;
		}
	}
}

/**
 * @brief �ر��������ӣ�����Detach�����ڴ�
 */
void CSafeTcpClient::Destory()
{
    //�ر���������
	CloseAllConn();

    //�ͷ��ڴ�
    if (m_pstSTcpConnStatus != NULL)
    {
        m_objStatusMem.Detach();
        m_pstSTcpConnStatus = NULL;
        m_pStatusMem = NULL;
    }

    return;
}

/**
 * @brief ��ȡ��������״̬
 */
int CSafeTcpClient::GetAllStatus(STcpConnStatusHeader &stSTcpConnStatusHeader, std::vector<STcpConnStatus> &vstSTcpConnStatus)
{
    int iRetVal = 0;
    if (m_pstSTcpConnStatus == NULL)
    {
        iRetVal = InitStatusShm();
        if (iRetVal != 0)
        {
            return -1;
        }
    }

    memcpy(&stSTcpConnStatusHeader, m_pstSTcpConnStatus, sizeof(STcpConnStatusHeader));

    if (m_pstSTcpConnStatus->uiConnStatusNum > (unsigned int)STCP_MAX_CONN_STATUS)
    {
        m_pstSTcpConnStatus->uiConnStatusNum = STCP_MAX_CONN_STATUS;
    }

    vstSTcpConnStatus.clear();
    for (unsigned int i=0; i<m_pstSTcpConnStatus->uiConnStatusNum; i++)
    {
        vstSTcpConnStatus.push_back(m_pstSTcpConnStatus->astSTcpConnStatus[i]);
    }

    return 0;
}

/**
 * @brief ��ȡ��API�����ĸ�������״̬
 */
int CSafeTcpClient::GetMyStatus(std::vector<STcpConnStatus> &vstSTcpConnStatus)
{
    if (m_pstSTcpConnStatus == NULL)
    {
        snprintf(m_szErrMsg, sizeof(m_szErrMsg), "internal err:%s: conn_status is null", __func__);
        return -1;
    }

    if (m_pstSTcpConnStatus->uiConnStatusNum > (unsigned int)STCP_MAX_CONN_STATUS)
    {
        m_pstSTcpConnStatus->uiConnStatusNum = STCP_MAX_CONN_STATUS;
    }

    vstSTcpConnStatus.clear();
    for (int i=0; i<m_iSTcpConnDataNum; i++)
    {
        for (unsigned int j=0; j<m_pstSTcpConnStatus->uiConnStatusNum; j++)
        {
            if ((m_pstSTcpConnStatus->astSTcpConnStatus[j].uiIPAddr == m_astSTcpConnData[i].uiIPAddr)
                &&(m_pstSTcpConnStatus->astSTcpConnStatus[j].ushPort == m_astSTcpConnData[i].ushPort))
            {
                vstSTcpConnStatus.push_back(m_pstSTcpConnStatus->astSTcpConnStatus[j]);
            }
        }
    }

    return 0;
}

/**
 * @brief ����ĳ��IP/�˿ڶ�Ӧ��״̬
 */
int CSafeTcpClient::SetConnStatus(const STcpConnStatus &stSTcpConnStatus)
{
    int iRetVal = 0;
    if (m_pstSTcpConnStatus == NULL)
    {
        iRetVal = InitStatusShm();
        if (iRetVal != 0)
        {
            return -1;
        }
    }

    m_pstCurConnStatus = AddSTcpConnStatus(stSTcpConnStatus.uiIPAddr, stSTcpConnStatus.ushPort);

    SetSTcpConnStatusInternal(stSTcpConnStatus.ushStatus);

    return 0;
}


/**
 * @brief ɾ��ĳ��IP/�˿ڶ�Ӧ��״̬
 */
int CSafeTcpClient::DelConnStatus(const STcpConnStatus &stSTcpConnStatus)
{
    int iRetVal = 0;
    if (m_pstSTcpConnStatus == NULL)
    {
        iRetVal = InitStatusShm();
        if (iRetVal != 0)
        {
            return -1;
        }
    }

    DelConnStatusInternal(stSTcpConnStatus.uiIPAddr, stSTcpConnStatus.ushPort);

    return 0;
}

