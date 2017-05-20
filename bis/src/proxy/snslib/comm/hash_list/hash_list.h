/**
 * @file    hash_list.h
 * @brief   HashList(��ϣ����)��ģ����
 * @author  jamieli@tencent.com
 * @date    2008-07-02
 */
#ifndef _HASH_LIST_H_
#define _HASH_LIST_H_

#include <string.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>

namespace snslib
{

//�����ڲ�ָ�붼ʹ��ƫ����
template<class KEYTYPE, class DATATYPE>
class CHashList
{

#pragma pack(1)
	typedef struct tagHead
	{
		int iSize;
		int iTotalDataNodeNum;
		int iUsedDataNodeNum;
		int iIndexNodeNum;
		int ipFreeDataNodeList;
	}Head;

	typedef struct tagIndexNode
	{
		int iHashVal;
		int iDataNodeNum;
		int ipDataNodeList;
	}IndexNode;

	typedef struct tagDataNode
	{
		KEYTYPE KeyVal;
		int ipNextDataNode;
		DATATYPE Data;
	}DataNode;
#pragma pack()

public:
    const static int SUCCESS = 0;
    const static int ERROR = -1;

    const static int E_HASH_LIST_MEM_SIZE = -101;
    const static int E_HASH_LIST_INDEX_NUM = -102;
    const static int E_HASH_LIST_DATA_NUM = -103;
    const static int E_HASH_LIST_NO_SPACE = -104;
    const static int E_HASH_LIST_NO_NODE = -105;
    const static int E_HASH_LIST_NODE_EXIST = -106;
    const static int E_HASH_LIST_INVALID_KEY = -107;
    const static int E_HASH_LIST_VERIFY_INDEX_HASHVAL = -108;
    const static int E_HASH_LIST_VERIFY_INDEX_DATANUM = -109;
    const static int E_HASH_LIST_VERIFY_DATA_PTR = -110;
    const static int E_HASH_LIST_VERIFY_HEAD_USEDDATANUM = -111;
    const static int E_HASH_LIST_VERIFY_DATA_FREEDATAPTR = -112;
    const static int E_HASH_LIST_VERIFY_HEAD_TOTALDATANUM = -113;

public:
	const static int HEAD_SIZE = sizeof(Head);
	const static int INDEX_NODE_SIZE = sizeof(IndexNode);
	const static int DATA_NODE_SIZE = sizeof(DataNode);

	const static int HASH_LIST_LOCK_READ = 1;
	const static int HASH_LIST_LOCK_WRITE = 2;

protected:
    unsigned char *m_pvMem;
    int m_iIndexNodeNum;
    int m_iDataNodeNum;
    Head *m_pstHead;
    IndexNode *m_pstIndex;
    DataNode *m_pstData;

public:
	CHashList(void)
	{
		m_pvMem = NULL;
		m_pstHead = NULL;
		m_pstIndex = NULL;
		m_pstData = NULL;
	}

	virtual ~CHashList(void)
	{
	}

	virtual int Init(void *pvMem, int iMemSize, int iIndexNum, int iDataNum)
	{
		m_pvMem = (unsigned char*)pvMem;
		m_pstHead = (Head *)m_pvMem;
		m_pstIndex = (IndexNode *)(m_pvMem + HEAD_SIZE);
		m_pstData = (DataNode *)(m_pvMem + HEAD_SIZE + (iIndexNum * INDEX_NODE_SIZE) );

		int iTotalMemSize = HEAD_SIZE + (iIndexNum * INDEX_NODE_SIZE) + (iDataNum * DATA_NODE_SIZE);
		if (iTotalMemSize > iMemSize )
		{
			return E_HASH_LIST_MEM_SIZE;
		}

		if (iIndexNum <1)
		{
			return E_HASH_LIST_INDEX_NUM;
		}

		if (iDataNum < 2)
		{
			//��һ���ڵ㲻��ʹ��
			return E_HASH_LIST_DATA_NUM;
		}

		LockHead(HASH_LIST_LOCK_WRITE);
		m_pstHead->iSize = iMemSize;
		m_pstHead->iIndexNodeNum = iIndexNum;
		m_pstHead->iTotalDataNodeNum = iDataNum;
		UnLockHead();

		m_iIndexNodeNum = iIndexNum;
		m_iDataNodeNum = iDataNum;

		srand(getpid()+time(NULL));
		return SUCCESS;
	}

	virtual void Destory()
	{
	    return;
	}

	int Verify()
	{
		//TODO У���ڴ��е������Ƿ���ڴ���
        LockAll(HASH_LIST_LOCK_READ);

        int iUsedDataNodeNum = m_pstHead->iUsedDataNodeNum;
        int iRealUsedDataNodeNum = 1;
        int ipData = 0;

        for(int i=0; i<m_pstHead->iIndexNodeNum; i++)
        {
            if(m_pstIndex[i].iHashVal != i)
            {
                return E_HASH_LIST_VERIFY_INDEX_HASHVAL;
            }

            int iDataNodeNum = m_pstIndex[i].iDataNodeNum;
            int iRealDataNodeNum = 0;

            ipData = m_pstIndex[i].ipDataNodeList;

            while (ipData != 0)
            {
                iRealDataNodeNum++;
                if (ipData > m_pstHead->iTotalDataNodeNum)
                {
                    return E_HASH_LIST_VERIFY_DATA_PTR;
                }
                ipData = m_pstData[ipData].ipNextDataNode;
            }

            if (iRealDataNodeNum != iDataNodeNum)
            {
                return E_HASH_LIST_VERIFY_INDEX_DATANUM;
            }

            iRealUsedDataNodeNum += iRealDataNodeNum;
        }

        if (iRealUsedDataNodeNum != iUsedDataNodeNum)
        {
            return E_HASH_LIST_VERIFY_HEAD_USEDDATANUM;
        }

        ipData = m_pstHead->ipFreeDataNodeList;
        int iFreeDataNodeNum = 0;
        while (ipData != 0)
        {
            iFreeDataNodeNum++;
            if (ipData > m_pstHead->iTotalDataNodeNum)
            {
                return E_HASH_LIST_VERIFY_DATA_PTR;
            }
            ipData = m_pstData[ipData].ipNextDataNode;
        }

        if ((iFreeDataNodeNum + iUsedDataNodeNum) != m_pstHead->iTotalDataNodeNum)
        {
            return E_HASH_LIST_VERIFY_HEAD_TOTALDATANUM;
        }

        UnLockAll();
		return 0;
	}

	int Clear()
	{
        LockAll(HASH_LIST_LOCK_WRITE);

		int iMemSize = m_pstHead->iSize;
		int iIndexNodeNum = m_pstHead->iIndexNodeNum;
		int iDataNodeNum = m_pstHead->iTotalDataNodeNum;

		int iUsedMemSize=HEAD_SIZE + (m_iIndexNodeNum * INDEX_NODE_SIZE) + (m_iDataNodeNum * DATA_NODE_SIZE);


		memset(m_pvMem, 0x0, iUsedMemSize);

		m_pstHead->iSize = iMemSize;
		m_pstHead->iIndexNodeNum = iIndexNodeNum;
		m_pstHead->iTotalDataNodeNum = iDataNodeNum;
		m_pstHead->iUsedDataNodeNum = 1;
		m_pstHead->ipFreeDataNodeList = 1;	//��0���ڵ��ʾ��ָ��

		for (int i=0; i<m_pstHead->iIndexNodeNum; i++)
		{
			m_pstIndex[i].iHashVal = i;
			m_pstIndex[i].iDataNodeNum = 0;
			m_pstIndex[i].ipDataNodeList = 0;
		}

        for (int i=1; i<m_pstHead->iTotalDataNodeNum; i++)
        {
            m_pstData[i].ipNextDataNode = i+1;
        }

        m_pstData[m_pstHead->iTotalDataNodeNum-1].ipNextDataNode = 0;

		UnLockAll();

		return 0;
	}

	//����ָ����Data
	int Insert(const KEYTYPE &Key, const DATATYPE &Data)
	{
		int iHashVal = Hash(Key);
		int ipData = 0;
		int iRetVal = 0;

        LockIndex(HASH_LIST_LOCK_WRITE, iHashVal);
        ipData = m_pstIndex[iHashVal].ipDataNodeList;

        while ((ipData != 0)&&(m_pstData[ipData].KeyVal != Key))
        {
            int ipNextData = m_pstData[ipData].ipNextDataNode;
            ipData = ipNextData;
        }

        if (ipData > 0)
        {
            iRetVal = E_HASH_LIST_NODE_EXIST;

        }
        else
        {
            int ipFreeNode=GetFreeNode();
            if(ipFreeNode > 0)
            {
                m_pstData[ipFreeNode].KeyVal = Key;
                m_pstData[ipFreeNode].ipNextDataNode = m_pstIndex[iHashVal].ipDataNodeList;
                memcpy(&m_pstData[ipFreeNode].Data, &Data, sizeof(DATATYPE));
                m_pstIndex[iHashVal].ipDataNodeList = ipFreeNode;
                m_pstIndex[iHashVal].iDataNodeNum ++;

                iRetVal = SUCCESS;
            }
            else
            {

                iRetVal = ipFreeNode;
            }
        }

        UnLockIndex(iHashVal);
        return iRetVal;

	}

	int Update(const KEYTYPE &Key, const DATATYPE &Data)
	{
	    int iRetVal = SUCCESS;

	    int iHashVal = Hash(Key);
        int ipData = 0;

        LockIndex(HASH_LIST_LOCK_WRITE, iHashVal);
        ipData = m_pstIndex[iHashVal].ipDataNodeList;

        while ((ipData != 0)&&(m_pstData[ipData].KeyVal != Key))
        {
            int ipNextData = m_pstData[ipData].ipNextDataNode;
            ipData = ipNextData;
        }

        if (ipData > 0)
        {
            memcpy(&m_pstData[ipData].Data, &Data, sizeof(DATATYPE));
        }
        else
        {
            iRetVal = E_HASH_LIST_NO_NODE;
        }

        UnLockIndex(iHashVal);

        return iRetVal;
	}

	//ɾ��ָ��Key�Ľڵ�
	int Remove(const KEYTYPE &Key)
	{
	    int iRetVal = SUCCESS;
		int iHashVal = Hash(Key);
		int ipData = 0;
		int ipLastData = 0;

		LockIndex(HASH_LIST_LOCK_WRITE, iHashVal);

		ipData = m_pstIndex[iHashVal].ipDataNodeList;

		while ((ipData != 0)&&(m_pstData[ipData].KeyVal != Key))
		{
			ipLastData = ipData;
			ipData = m_pstData[ipData].ipNextDataNode;
		}

		if (ipData > 0)
		{
			//��������ȥ�������ݽڵ�
			if (0 == ipLastData)
			{
				m_pstIndex[iHashVal].ipDataNodeList = m_pstData[ipData].ipNextDataNode;
			}
			else
			{
				m_pstData[ipLastData].ipNextDataNode = m_pstData[ipData].ipNextDataNode;
			}
			m_pstIndex[iHashVal].iDataNodeNum--;

			iRetVal = RecycleNode(ipData);
		}
		else
		{
			iRetVal = E_HASH_LIST_NO_NODE;
		}

		UnLockIndex(iHashVal);

		return iRetVal;
	}

    //��ȡָ��Key�Ľڵ�
    int Get(const KEYTYPE &Key, DATATYPE &Data)
    {
        int iRetVal = SUCCESS;
        int iHashVal = Hash(Key);
        int ipData = 0;

        LockIndex(HASH_LIST_LOCK_READ, iHashVal);
        ipData = m_pstIndex[iHashVal].ipDataNodeList;

        while ((ipData != 0)&&(m_pstData[ipData].KeyVal != Key))
        {
            int ipNextData = m_pstData[ipData].ipNextDataNode;
            ipData = ipNextData;
        }

        if (ipData > 0)
        {
            memcpy(&Data, &m_pstData[ipData].Data, sizeof(DATATYPE));
            iRetVal = SUCCESS;
        }
        else
        {
            iRetVal = E_HASH_LIST_NO_NODE;
        }

        UnLockIndex(iHashVal);

        return iRetVal;
    }

	//����ָ��Key�Ľڵ㣬���лص�����
	int Process(const KEYTYPE &Key, void (*CallBackFunc)(DATATYPE &Data))
	{
        int iRetVal = SUCCESS;
        int iHashVal = Hash(Key);
        int ipData = 0;

        LockIndex(HASH_LIST_LOCK_WRITE, iHashVal);
        ipData = m_pstIndex[iHashVal].ipDataNodeList;

        while ((ipData != 0)&&(m_pstData[ipData].KeyVal != Key))
        {
            int ipNextData = m_pstData[ipData].ipNextDataNode;
            ipData = ipNextData;
        }

        if (ipData > 0)
        {
            //�ص�������
            CallBackFunc(m_pstData[ipData].Data);
            iRetVal = SUCCESS;
        }
        else
        {
            iRetVal = E_HASH_LIST_NO_NODE;
        }

        UnLockIndex(iHashVal);

        return iRetVal;
	}

    //�����нڵ���лص�����
    int ProcessAll(void (*CallBackFunc)(DATATYPE &Data))
    {
        int iRetVal = SUCCESS;
        int ipData = 0;

        for(int i=0; i<m_iIndexNodeNum; i++)
        {
            LockIndex(HASH_LIST_LOCK_WRITE, i);

            if (m_pstIndex[i].iDataNodeNum > 0)
            {
                ipData = m_pstIndex[i].ipDataNodeList;
                while (ipData != 0)
                {
                    //�ص�������
                    CallBackFunc(m_pstData[ipData].Data);
                    ipData = m_pstData[ipData].ipNextDataNode;
                }
            }

            UnLockIndex(i);
        }

        return iRetVal;
    }

    //�����нڵ���лص������ص�ʱ�ᴫ�ݲ���Key
    int ProcessAllWithKey(void (*CallBackFunc)(const KEYTYPE &Key, DATATYPE &Data))
    {
        int iRetVal = SUCCESS;
        int ipData = 0;

        for(int i=0; i<m_iIndexNodeNum; i++)
        {
            LockIndex(HASH_LIST_LOCK_WRITE, i);

            if (m_pstIndex[i].iDataNodeNum > 0)
            {
                ipData = m_pstIndex[i].ipDataNodeList;
                while (ipData != 0)
                {
                    //�ص�������
                    CallBackFunc(m_pstData[ipData].KeyVal, m_pstData[ipData].Data);
                    ipData = m_pstData[ipData].ipNextDataNode;
                }
            }

            UnLockIndex(i);
        }

        return iRetVal;
    }

    int GetRandomNode(KEYTYPE &Key, DATATYPE &Data)
    {
        int iRandom = random()%m_iIndexNodeNum;
        int ipData = 0;
        //����ֻ�������100����
        for (int i=iRandom; i<(iRandom+100); i++)
        {
            LockIndex(HASH_LIST_LOCK_READ, (i%m_iIndexNodeNum));
            if(m_pstIndex[i%m_iIndexNodeNum].iDataNodeNum > 0)
            {
                ipData = m_pstIndex[i%m_iIndexNodeNum].ipDataNodeList;

                int iRandom2 = (random()%m_pstIndex[i%m_iIndexNodeNum].iDataNodeNum);
                for(int j=0;((j<iRandom2)&&(ipData != 0));j++)
                {
                    ipData = m_pstData[ipData].ipNextDataNode;
                }

                if (ipData != 0)
                {
                    UnLockIndex(i%m_iIndexNodeNum);
                    break;
                }
            }
            UnLockIndex(i%m_iIndexNodeNum);
        }

        if (ipData != 0)
        {
            Key = m_pstData[ipData].KeyVal;
            Data=m_pstData[ipData].Data;
        }
        else
        {
            return E_HASH_LIST_NO_NODE;
        }

        return SUCCESS;
    }

    //��ȡIndex��Ŀ
	int GetIndexNum()
	{
	    return m_iIndexNodeNum;
	}

    //��ȡ�Ѿ�ʹ�õĽڵ���
    int GetUsedDataNum()
    {
        //�����ݻ�ȡʱ��������������΢С��ƫ��
        //���ڸ����ݲ�����ΪHashList�Ƿ������ж����ݣ�����ƫ�Ӱ��
        //���ﲻ���м���
        return m_pstHead->iUsedDataNodeNum;
    }

	//���HashListͷ��Ϣ
	virtual void ShowHeadInfo()
	{
	    LockHead(HASH_LIST_LOCK_READ);
		printf("HashList Header Info\n");
		printf("BuffSize=%d ", m_pstHead->iSize);
		printf("NodeNum=%d ", m_pstHead->iTotalDataNodeNum);
		printf("UsedNodeNum=%d ", m_pstHead->iUsedDataNodeNum);
		printf("IndexNum=%d\n", m_pstHead->iIndexNodeNum);
		UnLockHead();
	}

	//���HashList��Index��Ϣ
	virtual void ShowIndexInfo()
	{
		for (int i=0; i<m_iIndexNodeNum; i++)
		{
		    LockIndex(HASH_LIST_LOCK_READ, i);
			printf("Hash[%d],NodeNum=%d\n", m_pstIndex[i].iHashVal, m_pstIndex[i].iDataNodeNum);
			UnLockIndex(i);
		}
	}

	//������е�������Ϣ
	virtual void ShowDataInfo()
	{
		char szKeyStr[1024] = { 0 };
		char szDataStr[1024] = { 0 };
		int ipData = 0;
		for (int i=0; i<m_iIndexNodeNum; i++)
		{
		    LockIndex(HASH_LIST_LOCK_READ, i);
		    if(m_pstIndex[i].iDataNodeNum > 0)
		    {
    			printf("Hash[%d],NodeNum=%d\n", m_pstIndex[i].iHashVal, m_pstIndex[i].iDataNodeNum);
    			ipData = m_pstIndex[i].ipDataNodeList;

    	        while(ipData != 0)
    			{
    				printf("  Data[%s]=%s\n",
    					FormatKey(m_pstData[ipData].KeyVal, szKeyStr, sizeof(szKeyStr)),
    					FormatData(m_pstData[ipData].Data, szDataStr, sizeof(szDataStr)));
                    int ipNextData = m_pstData[ipData].ipNextDataNode;
    				ipData = ipNextData;
    			}
		    }
	        UnLockIndex(i);
		}

	}
	//��ȡĳ��Key��Hashֵ
	virtual int Hash(const KEYTYPE &Key)
	{
	    return Key % m_pstHead->iIndexNodeNum;
	}

	//����д��
	//summary:���Ҫʵ�ַ���HashList���⣬��Ҫ��������������
	//        ����ͨ����������ʵ�ֲ��������д����Ҳ����ͨ���ź���ʵ��
	//        �����洢������
	//param:iType��ʾ�������� 1-ReadLock 2-WriteLock
	//param:iOffSet��ʾ��ʼ����λ��
	//param:iSize��ʾ��Ҫ��������
	//return: 0-�ɹ� !=0-ʧ��
	virtual int Lock(int iType, int iOffSet, int iSize)
	{
		return 0;
	}

	//�ͷ���
	virtual int UnLock(int iPos, int iSize)
	{
		return 0;
	}

	//Key����Ļ��������ڴ�ӡ������Ϣ
	virtual const char *FormatKey(const KEYTYPE &Key, char *pBuff, int iBuffSize)
	{
	    pBuff[0] = '\0';
	    return pBuff;
	}

	//Data����Ļ��������ڴ�ӡ������Ϣ
	virtual const char *FormatData(const DATATYPE &Data, char *pBuff, int iBuffSize)
    {
        pBuff[0] = '\0';
        return pBuff;
    }

	//����IndexNum��NodeNum������������ڴ��С
	int CalcSize(int iIndexNum, int iNodeNum)
	{
	    return HEAD_SIZE + (iIndexNum * INDEX_NODE_SIZE) + (iNodeNum * DATA_NODE_SIZE);
	}

private:
    int GetFreeNode()
    {
        int iFreeNode = 0;
        int iFreeNodeNum = 0;

        LockHead(HASH_LIST_LOCK_WRITE);
        iFreeNodeNum = m_pstHead->iTotalDataNodeNum - m_pstHead->iUsedDataNodeNum;


        iFreeNode = m_pstHead->ipFreeDataNodeList;

        if ((iFreeNodeNum <= 0)||(iFreeNode <= 0))
        {
            iFreeNode = E_HASH_LIST_NO_SPACE;
        }
        else
        {
            m_pstHead->iUsedDataNodeNum++;
            m_pstHead->ipFreeDataNodeList = m_pstData[iFreeNode].ipNextDataNode;
        }

		UnLockHead(); //��֤�ڵ��������Ժ����ͷ�ͷ�ڵ����

		return iFreeNode;
	}

    int RecycleNode(int iNode)
    {
        int iRetVal = SUCCESS;

        LockHead(HASH_LIST_LOCK_WRITE);
        if ((0 == iNode) || (iNode > m_pstHead->iTotalDataNodeNum))
        {
            iRetVal = ERROR;
        }
        else
        {
            m_pstHead->iUsedDataNodeNum--;
            m_pstData[iNode].ipNextDataNode = m_pstHead->ipFreeDataNodeList;
            m_pstHead->ipFreeDataNodeList = iNode;
        }
        UnLockHead();

        return iRetVal;
    }

	int LockHead(int iType)
	{
		return Lock(iType, 0, HEAD_SIZE);
	}

	int UnLockHead()
	{
		return UnLock(0, HEAD_SIZE);
	}

	int LockIndex(int iType, int iOffSet)
	{
	    //LockIndex������������Index��Ӧ���������������е����ݽڵ�
		return Lock(iType, HEAD_SIZE+(iOffSet*INDEX_NODE_SIZE), INDEX_NODE_SIZE);
	}

	int UnLockIndex(int iOffSet)
	{
		return UnLock(HEAD_SIZE+(iOffSet*INDEX_NODE_SIZE), INDEX_NODE_SIZE);
	}

	/*
	int LockData(int iType, int iOffSet)
	{
		return Lock(iType, HEAD_SIZE+(m_iIndexNodeNum*INDEX_NODE_SIZE)+(iOffSet*DATA_NODE_SIZE), DATA_NODE_SIZE);
	}

	int UnLockData(int iOffSet)
	{
		return UnLock(HEAD_SIZE+(m_iIndexNodeNum*INDEX_NODE_SIZE)+(iOffSet*DATA_NODE_SIZE), DATA_NODE_SIZE);
	}
	*/

	int LockAll(int iType)
	{
		return Lock(iType, 0, HEAD_SIZE+(m_iIndexNodeNum*INDEX_NODE_SIZE)+(m_iDataNodeNum*DATA_NODE_SIZE));
	}

	int UnLockAll()
	{
		return UnLock(0, HEAD_SIZE+(m_iIndexNodeNum*INDEX_NODE_SIZE)+(m_iDataNodeNum*DATA_NODE_SIZE));
	}
};

}
#endif
