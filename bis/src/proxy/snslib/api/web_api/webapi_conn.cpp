#include <string.h>
#include <stdio.h>

#include "comm/util/pet_util.h"
#include "webapi_conn.h"
#include "comm/ini_file/ini_file.h"
#include "comm/log/pet_log.h"

using namespace snslib;

CWebApiConn::CWebApiConn()
{
}
CWebApiConn::~CWebApiConn()
{
}

int CWebApiConn::Init(const char* pszConfFile)
{
    int iRetVal = 0;

    char szTemp[256];
    char szSection[256];

    CIniFile objIniFile(pszConfFile);
    if (objIniFile.IsValid())
    {
        objIniFile.GetInt("WEBAPI", "HostNum", 0, &m_iHostNum);
    }
    else
    {
        snprintf(m_szErrMsg, sizeof(m_szErrMsg), "conf file is not valid, conf_file=%s", pszConfFile);
        return -1;
    }

    if (m_iHostNum <= 0 || m_iHostNum > MAX_WEBAPI_HOST_NUM)
    {
        snprintf(m_szErrMsg, sizeof(m_szErrMsg), "conf WEBAPI/HostNum[%d] is not valid, conf_file=%s", m_iHostNum, pszConfFile);
        return -2;
    }

    for (int i = 0; i < m_iHostNum; i++)
    {
        snprintf(szSection, sizeof(szSection), "HOST_%d", i + 1);
        memset(szTemp, 0, sizeof(szTemp));
        objIniFile.GetString(szSection, "IP", "", szTemp, sizeof(szTemp));
        snprintf(m_aszIpList[i], sizeof(m_aszIpList[i]), "%s", szTemp);
        objIniFile.GetInt(szSection, "Port", 0, &m_aiPortList[i]);

        if (m_aszIpList[i][0] == '\0')
        {
            snprintf(m_szErrMsg, sizeof(m_szErrMsg), "conf %s/IP is not valid, conf_file=%s", szSection, pszConfFile);
            return -3;
        }

        if (m_aiPortList[i] == 0)
        {
            snprintf(m_szErrMsg, sizeof(m_szErrMsg), "conf %s/Port is not valid, conf_file=%s", szSection, pszConfFile);
            return -4;
        }
    }

    //�������һ��WEBAPI
    int iRandom = CRandomTool::Instance()->Get(0, m_iHostNum);

    iRetVal = m_objTcpClient.Init(m_aszIpList[iRandom], m_aiPortList[iRandom]);
    if (iRetVal != 0)
    {
        snprintf(m_szErrMsg, sizeof(m_szErrMsg), "Connect to web_pcl host[%s:%d] failed, ret=%d, errmsg=%s", m_aszIpList[iRandom], m_aiPortList[iRandom], iRetVal, m_objTcpClient.GetErrMsg());
        return -5;
    }

    return 0;
}

/**
 * @brief ��������
 * @param pszSendBuff ��Ҫ���͵����ݰ�
 * @param iSendLen ��Ҫ�������ݰ��ĳ���
 *
 * @return �ɹ�����0��������ʾʧ��
 */
int CWebApiConn::Send(char *pszSendBuff, int iSendLen)
{
    int iRetVal = 0;
    iRetVal = m_objTcpClient.Send(pszSendBuff, iSendLen);
    if (iRetVal != 0)
    {
        snprintf(m_szErrMsg, sizeof(m_szErrMsg), "send pkg failed, ret=%d, errmsg=%s", iRetVal, m_objTcpClient.GetErrMsg());
        return -1;
    }

    return 0;
}

/**
 * @brief ��������
 * @param ppszRecvBuff ���յ������ݰ�����ָ��ָ��Ŀռ䲻��Ҫ�ⲿ�ͷ�
 * @param piPkgBodyLen ���յ����ݰ��ĳ���
 * @param iExpireLen �ڴ��������ݵĳ��ȣ�0-��ʾ�����ƽ��ճ���
 *
 * @return �ɹ�����0��������ʾʧ��
 */
int CWebApiConn::Recv(char *pszRecvBuff, int *piRecvLen, int iExpireLen /*= 0*/)
{
    int iRetVal = 0;

    if (iExpireLen > *piRecvLen)
    {
        snprintf(m_szErrMsg, sizeof(m_szErrMsg), "exp_len [%d] is larger than buff_len [%d]", iExpireLen, *piRecvLen);
        return -1;
    }

    int iRealRecvLen = *piRecvLen;
    iRetVal = m_objTcpClient.Recv(pszRecvBuff, iRealRecvLen, iExpireLen);
    if(iRetVal != 0)
    {
        snprintf(m_szErrMsg, sizeof(m_szErrMsg), "recv pkg failed, ret=%d, errmsg=%s", iRetVal, m_objTcpClient.GetErrMsg());
        return -1;
    }

    *piRecvLen = iRealRecvLen;

    return 0;
}

/**
 * @brief �������һ̨WebPCL
 */
int CWebApiConn::Reconnect()
{
    int iRetVal = 0;

    m_objTcpClient.Close();

    //�������һ��WEBAPI
    int iRandom = CRandomTool::Instance()->Get(0, m_iHostNum);

    iRetVal = m_objTcpClient.Init(m_aszIpList[iRandom], m_aiPortList[iRandom]);
    if (iRetVal != 0)
    {
        snprintf(m_szErrMsg, sizeof(m_szErrMsg), "Connect to web_pcl host[%s:%d] failed, ret=%d, errmsg=%s", m_aszIpList[iRandom], m_aiPortList[iRandom], iRetVal, m_objTcpClient.GetErrMsg());
        return -1;
    }

    return 0;
}
