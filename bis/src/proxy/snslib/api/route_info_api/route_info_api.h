/**
 * @file    route_info_api.h
 * @brief   ��ȡ·����Ϣ�Ľӿ�
 * @author  jamieli@tencent.com
 * @date    2009-03-19
 *
 * @note    �ýӿ�ʵ��ͨ��ProcID��DstBusID��ȡNextHopBusID
 *          �ýӿڵ�Reload���ⲿģ����Ҫʵ�ֲ���USER1�źţ����øýӿ�
 */

#ifndef _ROUTE_INFO_API_H_
#define _ROUTE_INFO_API_H_

#include <map>
#include <vector>

namespace snslib
{

class CRouteInfo
{
public:
    typedef struct tagRouteInfo
    {
        unsigned int uiProcID;
        unsigned int uiDstID;
        unsigned int uiNextHop;
    }RouteInfo;

    const static int SUCCESS = 0;
    const static int ERROR = -1;

    const static int ERR_BASE = -200;
    const static int CONF_FILE_INVALID = ERR_BASE - 1;
    const static int ERR_NO_ROUTE = ERR_BASE - 2;

public:
    CRouteInfo();

    ~CRouteInfo();

    /**
     * @brief ��ʼ��Route��Ϣ
     */
    int Init(const char *pszConfFile);

    /**
     * @brief ���¼���Route��Ϣ
     */
    int Reload(const char *pszConfFile);

    /**
     * @brief ��ȡ��һ���ڵ��BUSID
     */
    int GetNextHop(unsigned int uiProcID, unsigned int uiDstBusID, unsigned int *puiNextHop);

    /**
     * @brief ��ȡ������һ����Ϣ
     */
    int GetAllNextHop(unsigned int uiProcID, std::vector<unsigned int> &vuiAllNextHop);

    /**
     * @brief ��ȡ���е�·����Ϣ
     */
    int GetAll(std::vector<RouteInfo> &vstRouteInfo);

    const char *GetErrMsg()
    {
        return m_szErrMsg;
    }

private:
    std::map<unsigned long long, unsigned int> m_mRouteInfo;
    char m_szErrMsg[256];
};

}
#endif
