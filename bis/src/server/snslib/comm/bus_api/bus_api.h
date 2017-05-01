#ifndef _BUS_API_H_
#define _BUS_API_H_

#include "comm/util/pet_util.h"
#include "api/include/sns_protocol.h"

namespace snslib
{

const unsigned int ROUTER_ID_FOR_HEART_BEAT = 0xFFFFFFFF;

class CBusApi
{
public:
    const static int SUCCESS = 0;
    const static int ERROR = -1;

    const static int ERR_BASE = -200;
    const static int CONF_FILE_NOT_VALID = ERR_BASE - 1;
    const static int CONFIG_ITEM_ERR = ERR_BASE - 2;
    const static int TBUS_INIT_FAILED = ERR_BASE - 3;
    const static int TBUS_SEND_FAILED = ERR_BASE - 4;
    const static int TBUS_RECV_FAILED = ERR_BASE - 5;
    const static int BUS_FULL = ERR_BASE - 6;
    const static int BUS_EMPTY = ERR_BASE - 7;
    const static int RECV_BUFF_TOO_SMALL = ERR_BASE - 8;
    const static int SEND_PKG_TOO_LARGE = ERR_BASE - 9;
    const static int TBUS_NO_CHANNEL_MATCHED = ERR_BASE - 10;
    const static int TBUS_PEEK_FAILED = ERR_BASE - 11;
    const static int TBUS_POP_FAILED = ERR_BASE - 12;

    const static int CLUSTER_BUS_MAX = 20;  //�����ȱ���BUS����
    const static int ONE_CLUSTER_BUS_NUM_MAX = 20;  //ÿ���ȱ�BUS��BusID���������

    typedef struct tagClusterBusInfo
    {
        char szModuleName[64];
        unsigned int uiBusDst;
        int iBusIDNum;
        unsigned int auiBusID[ONE_CLUSTER_BUS_NUM_MAX];
        unsigned int uiLastActiveBusID;
    }ClusterBusInfo;

public:
    CBusApi();
    ~CBusApi();

    /**
     * @brief ��ʼ��BUS�ӿڣ�ͨ�������ļ���ʼ��
     */
    int Init(const char *pszConfFile, unsigned int uiProcID);

    /**
     * @brief ͨ��BUS��������
     * @param:uiDstBusID Ŀ��BUSID������Ƿ��͵�Router�����ǲ�֪�����͵���һ̨Router��������4.0.0.0
     * @param:pvData ��Ҫ�������ݵ�Buff
     * @param:iDataLen ��Ҫ�������ݵĳ���
     * @param:iFlag ��ʱ��ʹ��
     *
     * @note:
     * 1)������֪�����͵���һ̨Router������Bus�ӿڵ������ļ���������Router�ӿ�ʹ���ȱ���ģʽ�������ýӿ�Ҳ�ὫuiDstBusID�滻Ϊ���һ���յ����ݵ��Ǹ�Router
     */
    int Send(unsigned int uiDstBusID, const void *pvData, int iDataLen, int iFlag = 0);

    /**
     * @brief ͨ��BUS��������
     * @param:puiSrcBusID ԴBUSID
     * @param:pvData ��Ҫ�������ݵĻ���������Ҫ���ⲿ����
     * @param:piDataLen �洢���ĳ��ȣ�ͬʱ��Ϊ���յ����ݰ����ȵķ���ֵ
     * @param:iFlag ��ʱ��ʹ��
     *
     * @note:
     * 1)�ýӿ��߼��н���Router�ļ������д���Router�ļ������᷵�ظ��û�
     */
    int Recv(unsigned int *puiSrcBusID, void *pvData, int *piDataLen, int iFlag = 0);

    /**
     * @brief ͨ��BUS��������,������Ȼ������bus�У���Ҫ����Pop�ӿ��������
     * @param:puiSrcBusID ԴBUSID
     * @param:puiDstBUsID Ŀ��BusID
     * @param:pvData ��Ҫ�������ݵĻ���������Ҫ���ⲿ����
     * @param:piDataLen �洢���ĳ��ȣ�ͬʱ��Ϊ���յ����ݰ����ȵķ���ֵ
     * @param:iFlag ��ʱ��ʹ��
     *
     * @note:
     * 1)�ýӿ��߼��н���Router�ļ������д���Router�ļ������᷵�ظ��û�
     */
    int Peek(unsigned int *puiSrcBusID, unsigned int *puiDstBusID, void **ppvData, int *piDataLen, int iFlag = 0);

    /**
     * @brief �Ƴ�Bus�еĵ�һ�����ݰ���
     * @param:uiSrcBusID ԴBUSID
     * @param:uiDstBusID Ŀ��BusID
     */
    int Pop(unsigned int uiSrcBusID, unsigned uiDstBusID);

    /**
         * @brief �Զ�ѡ��BUSͨ���������ݡ�����dst id�жϣ����dst idΪ0����router��bus id�򷢸�router�����������ʵdst idת��
         * @param:pvData ��Ҫ�������ݵ�Buff
         * @param:iDataLen ��Ҫ�������ݵĳ���
         */
    int AutoSend(void *pvData, int iDataLen);

    /**
     * @brief ���������ݰ����͵����е�Router
     * @param:pvData ��Ҫ�������ݵ�Buff
     * @param:iDataLen ��Ҫ�������ݵĳ���
     * @param:iFlag ��ʱ��ʹ��
     *
     * @note BUSHEADER�е�RouterID�ᱻ�޸�Ϊ�ض�Router��BUSID
     */
    int SendToAllRouter(void *pvData, int iDataLen, int iFlag = 0);

    /**
     * @brief ���������ݰ�ֱ�ӷ��͵����е�Router
     * @param:pvData ��Ҫ�������ݵ�Buff
     * @param:iDataLen ��Ҫ�������ݵĳ���
     * @param:iFlag ��ʱ��ʹ��
     *
     * @note BUSHEADER�е�RouterID���ᱻ�޸ģ��������������͵�ʱ����ROUTERIDΪ�����־���͵�
     */
    int SendToAllRouterDirect(void *pvData, int iDataLen, int iFlag = 0);

    unsigned int GetProcID()
    {
        return m_uiProcID;
    }

    const char *GetErrMsg()
    {
        return m_szErrMsg;
    }

private:
    int m_iGCIMKey;
    unsigned int m_uiProcID;
    unsigned int m_uiRouterBusID;
    unsigned int m_auiAllRouterBusID[ONE_CLUSTER_BUS_NUM_MAX];
    int m_iRouterBusNum;

    int m_iClusterBusNum;
    ClusterBusInfo m_astClusterBusInfo[CLUSTER_BUS_MAX];

    int m_iBusHandle;
    char m_szErrMsg[256];
    int m_iInitFlag;

    BusHeader *m_pstBusHeader;
};

}
#endif


