#ifndef _SHM_QUEUE_H_
#define _SHM_QUEUE_H_

#include    "comm/sem_lock/sem_lock.h"

namespace snslib
{

//������β������ʶ���룬������������ʱ�����ʶ����һ����¼
const char TAIL_FLAG = 0xAA;

class CShmQueue
{
public:
    const static int SUCCESS = 0;
    const static int ERROR = -1;

    const static int E_SHM_QUEUE_SEMLOCK = -601;
    const static int E_SHM_QUEUE_AT_SHM = -602;
    const static int E_SHM_QUEUE_GET_SHM = -603;
    const static int E_SHM_QUEUE_FULL = -604;
    const static int E_SHM_QUEUE_EMPTY = -605;
    const static int E_SHM_QUEUE_BUFFLIMIT = -606;

    typedef struct tagQueueHead
    {
        int iLen;  //Queue���ܳ���
        int iHead; //����ͷָ��
        int iTail; //����βָ��
        int iNum;  //���ݿ����
    } QueueHead;

    typedef struct tagBlockHead
    {
        int iIndex;
        // 2013��12��12�� 21:44:26
        // shimmeryang: short���ֻ��32K�������ð�������Ϊint�Ͱ�
        int iLen;
    } BlockHead;

public:
    CShmQueue();
    virtual ~CShmQueue();

    int Init(const int aiShmKey, const int aiSize);

    // ��������
    int InQueue(const char * pData, int iDataLen);

    // ��ȡ���ݣ����ݳ��Ȼᱣ����iBufLen
    int OutQueue(char * pBuf, int * iBufLen);

    // ͷ����������
    int HeaderInQueue(const char * pData, int iDataLen);

    // ��ȡQueue�м�¼����
    int GetNum();

    const QueueHead * GetHead();

    const char *GetErrMsg()
    {
        return m_szErrMsg;
    }

protected:
    char *m_pMem;
    QueueHead * m_pHead;
    CSemLock * m_pobjSemLock;
    unsigned int m_iQueueSize;

    char m_szErrMsg[256];
};

}
#endif

