#ifndef _WEBAPI_BASE_H_
#define _WEBAPI_BASE_H_

#include <string.h>
#include <stdio.h>

#include "webapi_conn.h"
#include "api/include/sns_protocol.h"
#include "comm/log/pet_log.h"
#include "comm/util/pet_util.h"

namespace snslib
{

class CWebApiBase
{
public:
	const static int MAX_WEBAPI_HOST_NUM = 100;
	const static int MAX_WEBAPI_BUFF_LEN = 102400;

public:
	CWebApiBase();
	virtual ~CWebApiBase();

public:
	int BaseInit(CWebApiConn *pobjWebApiConn, unsigned int uiHostIP);

	static const char *GetErrMsg()
	{
	    return m_szErrMsg;
	}

protected:
	/**
	 * @brief ��������
	 * @param stAppHeader ��Ҫ�������ݵ�AppHeader����
	 * @param pszPkgBody ��Ҫ�������ݰ��岿��ͷָ��
	 * @param iPkgBodyLen ��Ҫ�������ݵİ��峤��
	 *
	 * @return �ɹ�����0��������ʾʧ��
	 */
	int Send(const AppHeader &stAppHeader, const char *pszPkgBody, int iPkgBodyLen);

    /**
     * @brief ��������
     * @param stAppHeader ���յ����ݵ�AppHeader����
     * @param ppszPkgBody ���յ����ݰ��岿��ͷָ�룬��ָ��ָ��Ŀռ䲻��Ҫ�ⲿ�ͷ�
     * @param piPkgBodyLen ���յ����ݵİ��峤��
     *
     * @return �ɹ�����0��������ʾʧ��
     */
	int Recv(AppHeader &stAppHeader, char **ppszPkgBody, int *piPkgBodyLen);

	/**
	 * @brief ���Ͳ��ҽ�������
	 * @param stSendAppHeader ��Ҫ�������ݵ�AppHeader
	 * @param pszSendPkgBody ��Ҫ�������ݵİ���
	 * @param iSendPkgBodyLen ��Ҫ�������ݵİ��峤��
	 * @param stRecvAppHeader ���յ������ݶ�Ӧ��AppHeader
	 * @param ppszRecvPkgBody ���յ������ݰ���ͷָ�룬��ָ��ָ��Ŀռ䲻��Ҫ�ⲿ�ͷ�
	 * @param piRecvPkgBodyLen �յ����ݵĳ���
	 *
	 * @return �ɹ�����0��������ʾʧ��
	 * @note ppszRecvPkgBodyָ������ݣ�ֻ�Ǹ����Ա�����е�һ��BUFF������Ҫ�����ⲿ�ͷ�
	 */
	int SendAndRecv(const AppHeader &stSendAppHeader, const char *pszSendPkgBody, int iSendPkgBodyLen, AppHeader &stRecvAppHeader, char **ppszRecvPkgBody, int *piRecvPkgBodyLen);

	/**
	 * @brief ���Ͳ���������
	 */
	template<class REQUEST_TYPE, class RESPONSE_TYPE>
	int SendAndRecv(unsigned int uiUin, unsigned short ushCmdID, unsigned short ushDstAppID, unsigned short ushDstSvrID, REQUEST_TYPE &objRequest, RESPONSE_TYPE &objResponse)
	{
	    int iRetVal = 0;

        char szSendBuff[MAX_WEBAPI_BUFF_LEN];
        int iSendLen;

	    AppHeader stSendAppHeader, stRecvAppHeader;
        memset(&stSendAppHeader, 0x0, sizeof(stSendAppHeader));
        stSendAppHeader.uiUin = uiUin;
        stSendAppHeader.ushCmdID = ushCmdID;
        stSendAppHeader.ushDestSvrID = ushDstSvrID;

        if(!objRequest.SerializeToArray(szSendBuff, sizeof(szSendBuff)))
        {
            snprintf(m_szErrMsg, sizeof(m_szErrMsg), "serialize req to buff failed");
            return -1;
        }

        iSendLen = objRequest.ByteSize();

        char *pszRecvBuff = NULL;
        int iRecvLen = 0;
        iRetVal = SendAndRecv(stSendAppHeader, szSendBuff, iSendLen, stRecvAppHeader, &pszRecvBuff, &iRecvLen);
        if (iRetVal != 0)
        {
            return -2;
        }

        if (iRecvLen <= 0)
        {
            snprintf(m_szErrMsg, sizeof(m_szErrMsg), "recv len is not valid, recv_len=%d", iRecvLen);
            return -3;
        }

        if (stRecvAppHeader.iRet != 0)
        {
            snprintf(m_szErrMsg, sizeof(m_szErrMsg), "ret val in header is %d", stRecvAppHeader.iRet);
            return stRecvAppHeader.iRet;
        }

        if(!objResponse.ParseFromArray(pszRecvBuff, iRecvLen))
        {
            snprintf(m_szErrMsg, sizeof(m_szErrMsg), "parse rsp from buff failed");
            return -4;
        }

        return 0;
	}

    /**
     * @brief ���Ͳ��������ݣ����յ�����û�а��岿��
     */
    template<class REQUEST_TYPE>
    int SendAndRecv(unsigned int uiUin, unsigned short ushCmdID, unsigned short ushDstAppID, unsigned short ushDstSvrID, REQUEST_TYPE &objRequest)
    {
        int iRetVal = 0;

        char szSendBuff[MAX_WEBAPI_BUFF_LEN];
        int iSendLen;

        AppHeader stSendAppHeader, stRecvAppHeader;
        memset(&stSendAppHeader, 0x0, sizeof(stSendAppHeader));
        stSendAppHeader.uiUin = uiUin;
        stSendAppHeader.ushCmdID = ushCmdID;
        stSendAppHeader.ushDestSvrID = ushDstSvrID;

        if(!objRequest.SerializeToArray(szSendBuff, sizeof(szSendBuff)))
        {
            snprintf(m_szErrMsg, sizeof(m_szErrMsg), "serialize req to buff failed");
            return -1;
        }

        iSendLen = objRequest.ByteSize();

        PetLog(0, uiUin, PETLOG_TRACE, "PROTO_SERIAL|%d|%s", iSendLen, CStrTool::Str2Hex(szSendBuff, iSendLen));

        char *pszRecvBuff = NULL;
        int iRecvLen = 0;
        iRetVal = SendAndRecv(stSendAppHeader, szSendBuff, iSendLen, stRecvAppHeader, &pszRecvBuff, &iRecvLen);
        if (iRetVal != 0)
        {
            return -2;
        }

        if (stRecvAppHeader.iRet != 0)
        {
            snprintf(m_szErrMsg, sizeof(m_szErrMsg), "ret val in header is %d", stRecvAppHeader.iRet);
            return stRecvAppHeader.iRet;
        }

        return 0;
    }

protected:
	static char m_szErrMsg[256];
    unsigned int m_uiHostIP;

private:

	CWebApiConn *m_pobjWebApiConn;

    //���涨���ȫ�ֱ������ᵼ���̲߳���ȫ�����ǲ������ö��߳̾�����ν
	static char m_szSendBuff[MAX_WEBAPI_BUFF_LEN];
	static char m_szRecvBuff[MAX_WEBAPI_BUFF_LEN];
	static int m_iSendLen;
	static int m_iRecvLen;
};
}
#endif //_WEBAPI_BASE_H_
