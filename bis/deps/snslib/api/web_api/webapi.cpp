#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "webapi.h"
using namespace snslib;

CWebApi::CWebApi()
{
    m_uiHostIP = 0;
}

CWebApi::~CWebApi()
{
}

int CWebApi::Init(const char* pszConfFile)
{
    int iRetVal = 0;

    iRetVal = m_objWebApiConn.Init(pszConfFile);
    if (iRetVal != 0)
    {
        snprintf(m_szErrMsg, sizeof(m_szErrMsg), "webapi_conn init failed, ret=%d, errmsg=%s", iRetVal, m_objWebApiConn.GetErrMsg());
        return iRetVal;
    }

    //��ȡ����IP��ַ
    //���Ȼ�ȡeth1��IP��ַ�������ȡ�������ٻ�ȡeth0��IP��ַ
    const char *pszHostAddr = NULL;
    pszHostAddr = CSysTool::GetNicAddr("eth1");
    if (pszHostAddr == NULL){
        pszHostAddr = CSysTool::GetNicAddr("eth0");
        if (pszHostAddr == NULL){
            m_uiHostIP = 0;
        }else{
            m_uiHostIP = inet_addr(pszHostAddr);
        }
    }else{
        m_uiHostIP = inet_addr(pszHostAddr);
    }


    //���еĹ��ܻ��࣬��Ҫ���������һ��BaseInit
    CWebApiFlag::BaseInit(&m_objWebApiConn, m_uiHostIP);
    CWebApiSnsInfo::BaseInit(&m_objWebApiConn, m_uiHostIP);
    CWebApiTest::BaseInit(&m_objWebApiConn, m_uiHostIP);
    CWebApiLock::BaseInit(&m_objWebApiConn, m_uiHostIP);
    CWebApiRandSelect::BaseInit(&m_objWebApiConn, m_uiHostIP);
    CWebApiQQInfo::BaseInit(&m_objWebApiConn, m_uiHostIP);
    CWebApiQQList::BaseInit(&m_objWebApiConn, m_uiHostIP);
    CWebApiKongfu::BaseInit(&m_objWebApiConn, m_uiHostIP);
    //����һЩWEBAPIҵ��ģ���ʼ��������������

    return 0;
}

