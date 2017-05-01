#ifndef _WEBAPI_CONN_H_
#define _WEBAPI_CONN_H_

#include <string.h>
#include <stdio.h>

#include "comm/tcp_client/tcp_client.h"
#include "comm/log/pet_log.h"
#include "comm/util/pet_util.h"

namespace snslib
{

class CWebApiConn
{
public:
	const static int MAX_WEBAPI_HOST_NUM = 100;
	const static int MAX_WEBAPI_BUFF_LEN = 102400;

public:
	CWebApiConn();
	virtual ~CWebApiConn();

public:
    /**
     * @brief ��ʼ��
     * @param pszConfFile �����ļ�
     *
     * @return �ɹ�����0��������ʾʧ��
     */
	int Init(const char* pszConfFile);

	/**
	 * @brief ��������
	 * @param pszSendBuff ��Ҫ���͵����ݰ�
	 * @param iSendLen ��Ҫ�������ݰ��ĳ���
	 *
	 * @return �ɹ�����0��������ʾʧ��
	 */
	int Send(char *pszSendBuff, int iSendLen);

    /**
     * @brief ��������
     * @param ppszRecvBuff ���յ������ݰ�����ָ��ָ��Ŀռ䲻��Ҫ�ⲿ�ͷ�
     * @param piPkgBodyLen ���յ����ݰ��ĳ���
     * @param iExpireLen �ڴ��������ݵĳ��ȣ�0-��ʾ�����ƽ��ճ���
     *
     * @return �ɹ�����0��������ʾʧ��
     */
	int Recv(char *pszRecvBuff, int *piRecvLen, int iExpireLen = 0);

	/**
	 * @brief �������һ̨WebPCL
	 */
	int Reconnect();

    /**
     * @brief ��ȡ������Ϣ
     */
    const char *GetErrMsg()
    {
        return m_szErrMsg;
    }

private:
	int m_iHostNum;
	char m_aszIpList[MAX_WEBAPI_HOST_NUM][16];
	int m_aiPortList[MAX_WEBAPI_HOST_NUM];
	CTcpClient m_objTcpClient;

	char m_szRecvBuff[MAX_WEBAPI_BUFF_LEN];
	int m_iRecvLen;

	char m_szErrMsg[256];
};
}
#endif //_WEBAPI_BASE_H_
