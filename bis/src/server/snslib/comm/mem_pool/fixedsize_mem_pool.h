/*
 * @file     fixedsize_mem_pool.h
 * @brief    �̶��������ݿ��ڴ�ع�����
 * @author   jamieli@tencent.com
 * @date     2008-9-23
 *
 * @note     ��ģ����Ĵ洢�ռ�������ⲿ���䣬�ڲ������������͵Ĳ�ͬ�����ڴ滮��ΪС����й���
 *           ��ģ�����ڲ�û�����Ĺ�����Ҫ�ⲿ���й���
 *           �����������������ͣ�
 *           1-�ڴ��ͷ��
 *           2-�ڴ�ؿ���������
 *           3-�ڴ��ʹ�ñ��λ��
 *           4-�ڴ�����ݽڵ���
 *           �����ݽ��в���ʱ���漰���������£�
 *           1-���䡢���սڵ㣺��Ҫ���ڴ��ͷ�����ڴ�ؿ������������ڴ��ʹ�ñ��λ��ȫ����д��
 *           2-�ⲿ�������ݣ��ⲿ����������ֱ��ͨ��ָ����з��ʵģ��ⲿҪ�跨ʹ�����̷�������ʱ����ͻ
 *
 */
#ifndef _FIXEDSIZE_MEM_POOL_H_
#define _FIXEDSIZE_MEM_POOL_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

namespace snslib
{
template<class DATATYPE>
class CFixedsizeMemPool
{
    typedef struct tagHead
    {
        int iMemSize;
        int iNodeNum;
        int iUsedNodeNum;
        int iFreeHead;
        int iFreeTail;
    }Head;

public:
    const static int SUCCESS = 0;
    const static int ERROR = -1;

    const static int E_NODE_NUM = -101; //���������ȷ
    const static int E_MEM_SIZE = -102; //�ⲿ������ڴ�ռ䲻��ʹ��
    const static int E_DATAP_ERR = -103;    //�ⲿ���������ָ��Ƿ�
    const static int E_VERIFY_CYCLE_LIST_ERR = -104;    //У��ʱ�����սڵ�ѭ������ָ��ʱ����
    const static int E_VERIFY_USEDNODE_NUM = -105;      //У��ʱ������Ѿ�ʹ�ù��Ľڵ���ʱ����
    const static int E_VERIFY_USEDBIT_FLAG = -106;      //У��ʱ�����ʹ�ñ��BITλ����

private:
    Head *m_pstHead;        //���ڼ�¼ͷ�ڵ�
    int *m_piFreeList;      //���ڼ�¼���ýڵ��ѭ���б�
    unsigned char *m_pucUsedBit;      //���ڼ�¼�ڴ���Ƿ�ʹ�õı��,ÿ���ڴ��ʹ��һ��bitλ
    DATATYPE *m_pDataBuff;  //���ڴ�����ݵ��ڴ��

    void *m_pvMem;          //���ڼ�¼MemPool���ڴ��ַ

    int m_iGetNextPos;  //���ڼ�¼GetNextNode()�������õ�λ��

public:

    CFixedsizeMemPool()
    {
        m_pstHead = NULL;
        m_piFreeList = NULL;
        m_pucUsedBit = NULL;
        m_pDataBuff = NULL;

        m_iGetNextPos = 0;
    }

    ~CFixedsizeMemPool()
    {
    }


    /**
     * @brief ��ȡ����iNodeNum�Ľڵ���Ҫ���ڴ��С
     *
     * @param: iNodeNum
     *
     * @return MemSize
     */
    int GetNeedMemSize(int iNodeNum)
    {
        int iMemSize =  (int)(sizeof(Head) + iNodeNum * (sizeof(int) + sizeof(DATATYPE) + sizeof(char)));
        return iMemSize;
    }

    /*
     * @summary:����һ���ڵ���ΪiNodeNum���ڴ��
     * @param:pvMem:Ԥ������ڴ��ַ
     * @param:iMemSize:Ԥ������ڴ�ռ�
     * @param:iNodeNum:���ݽڵ������
     * @param:iClearFlag:������ݱ�־λ�������λΪ1����ʾ����ڴ�ص���������
     * @return:0 success, -1 error
     */
    int Init (void *pvMem, int iMemSize, int iNodeNum, int iClearFlag = 0)
    {
        if (iNodeNum < 0)
        {
            return E_NODE_NUM;
        }
        else if (iNodeNum == 0)
        {
            iNodeNum = (iMemSize - sizeof(Head))/(sizeof(int) + sizeof(DATATYPE) + sizeof(char));   //�˷���һ����ڴ� :)
        }

        if (pvMem == NULL)
        {
            //ָ��Ϊ��
       
            return E_MEM_SIZE;
        }

        if (iMemSize < (int)((sizeof(Head))+(sizeof(int)*iNodeNum)+(sizeof(DATATYPE)*iNodeNum)+((iNodeNum+7)/8)))
        {
        
            //�ڴ治��
            return E_MEM_SIZE;
        }

        m_pvMem = pvMem;

        //ͷ�ڵ�ռ�
        m_pstHead = (Head *)pvMem;

        //����սڵ��б�ռ�
        m_piFreeList = (int *)((char *)m_pstHead + sizeof(Head));

        //�����ڴ��ʹ�ñ�ǿռ�
        m_pucUsedBit = (unsigned char *)((char *)m_piFreeList + (sizeof(int)*iNodeNum));

        //�������ݿռ�
        m_pDataBuff = (DATATYPE *)((char *)m_pucUsedBit + ((iNodeNum+7)/8));

        if(iClearFlag == 1)
        {
            memset(m_pstHead, 0x0, sizeof(Head));
            memset(m_piFreeList, 0x0, sizeof(int)*iNodeNum);
            memset(m_pucUsedBit, 0x0, (iNodeNum+7)/8);
            memset(m_pDataBuff, 0x0, sizeof(DATATYPE)*iNodeNum);

            for(int i=0; i<iNodeNum; i++)
            {
                m_piFreeList[i] = i;
            }

            m_pstHead->iMemSize = iMemSize;
            m_pstHead->iNodeNum = iNodeNum;
            m_pstHead->iUsedNodeNum = 0;
            m_pstHead->iFreeHead = 0;
            m_pstHead->iFreeTail = iNodeNum - 1;
        }
        else
        {
            //�ڴ��С��һ��
            if (m_pstHead->iMemSize != iMemSize)
            {
                return E_MEM_SIZE;
            }

            //�ڵ�����һ��
            if (m_pstHead->iNodeNum != iNodeNum)
            {
                return E_NODE_NUM;
            }
        }

        return 0;
    }

    int Verify()
    {
        //���FreeListѭ�������еĽڵ�ָ���Ƿ���Ч
        for (int i=0; i<m_pstHead->iNodeNum; i++)
        {
            if (m_piFreeList[i] >= m_pstHead->iNodeNum)
            {
                return E_VERIFY_CYCLE_LIST_ERR;
            }
        }

        if ((m_pstHead->iNodeNum - ((((m_pstHead->iFreeTail + m_pstHead->iNodeNum) - m_pstHead->iFreeHead)%m_pstHead->iNodeNum) + 1)) != m_pstHead->iUsedNodeNum)
        {
            printf("m_pstHead->iNodeNum = %d, m_pstHead->iFreeTail=%d, m_pstHead->iFreeHead=%d, m_pstHead->iUsedNodeNum=%d, Must=%d\n", m_pstHead->iNodeNum, m_pstHead->iFreeTail, m_pstHead->iFreeHead, m_pstHead->iUsedNodeNum, (m_pstHead->iNodeNum - ((((m_pstHead->iFreeTail + m_pstHead->iNodeNum) - m_pstHead->iFreeHead)%m_pstHead->iNodeNum) + 1)));
            return E_VERIFY_USEDNODE_NUM;
        }

        int iUsedNumFromBitFlag = 0;
        for(int i=0; i<m_pstHead->iNodeNum; i++)
        {
            if ((m_pucUsedBit[i/8]&(1<<(i%8))) != 0)
            {
                //�ýڵ㱻ʹ�ù�
                iUsedNumFromBitFlag++;
            }
        }

        if (iUsedNumFromBitFlag != m_pstHead->iUsedNodeNum)
        {
            printf("iUsedNumFromBitFlag=%d m_pstHead->iUsedNodeNum=%d\n", iUsedNumFromBitFlag, m_pstHead->iUsedNodeNum);
            return E_VERIFY_USEDBIT_FLAG;
        }

        return SUCCESS;
    }

    /*
     * @summary:���ڴ���з���һ���ڵ�
     * @return:NULL failed, !=NULL OK
     */
    DATATYPE *AllocateNode()
    {
        DATATYPE *pFreeNode = NULL;

        if (((m_pstHead->iFreeHead+1)%m_pstHead->iNodeNum) == m_pstHead->iFreeTail)
        {
            //û�пռ����ʹ����
            pFreeNode = NULL;
        }
        else
        {
            pFreeNode = &m_pDataBuff[m_piFreeList[m_pstHead->iFreeHead]];
            int iMaskBit = m_piFreeList[m_pstHead->iFreeHead]%8;
            //����λ��Ϊ�Ѿ���ʹ��״̬
            m_pucUsedBit[m_piFreeList[m_pstHead->iFreeHead]/8]|=(1<<iMaskBit);
            m_pstHead->iFreeHead = (m_pstHead->iFreeHead+1)%m_pstHead->iNodeNum;
            m_pstHead->iUsedNodeNum ++;
        }

        return pFreeNode;
    }

    /*
     * @summary:�ͷ��ڴ���е�һ���ڵ�
     * @return:0 success, -1 error
     */
    int ReleaseNode(DATATYPE *pData)
    {
        if ((pData < m_pDataBuff)
                ||(pData > (m_pDataBuff + m_pstHead->iNodeNum))
                ||((((char *)pData-(char *)m_pDataBuff)%sizeof(DATATYPE))!= 0)
                )
        {
            //���ڴ�ؿռ��е�����,����ָ�벻�����ݿ�����λ��
            return E_DATAP_ERR;
        }

        int iMaskPos = ((char *)pData-(char *)m_pDataBuff)/sizeof(DATATYPE);
        int iMaskBit = iMaskPos%8;
        if ((m_pucUsedBit[iMaskPos/8]&(1<<iMaskBit)) == 0)
        {
            //�ýڵ��ǿսڵ㣬��Ӧ�ñ����գ����������һ�β����κβ���
            return 0;
        }
        else
        {
            m_piFreeList[m_pstHead->iFreeTail] = iMaskPos;
            //�ýڵ���Ϊ�Ѿ��ͷ�
            m_pucUsedBit[iMaskPos/8]&=(~(1<<iMaskBit));
            m_pstHead->iFreeTail = (m_pstHead->iFreeTail+1)%m_pstHead->iNodeNum;
            m_pstHead->iUsedNodeNum--;
        }

        return 0;
    }

    /*
     * @summary:���ڴ���л�ȡ��һ����ʹ�õĽڵ�
     *          ��һ�ε���ʱ���ص�һ����ʹ�õĽڵ�
     *          ���һ�ε��÷���NULL
     *          �ú������ڱ������б�ʹ�õĽڵ�
     * @param:iFirstNodeFlag:��ʶ�Ƿ�ӵ�һ���ڵ㿪ʼ��ȡ 0-���� 1-��
     * @return:NULL failed, !=NULL OK
     */
    DATATYPE *GetNextNode(int iFirstNodeFlag = 0)
    {
        DATATYPE *pRetData = NULL;

        if(iFirstNodeFlag != 0)
        {
            m_iGetNextPos = 0;
        }

        for (int i=m_iGetNextPos; i< m_pstHead->iNodeNum; i++)
        {
            if ((m_pucUsedBit[i/8]&(1<<(i%8))) != 0)	// 1
            {
                pRetData=m_pDataBuff+i;
		m_iGetNextPos = i+1;	// 2
                break;
            }
        }

        return pRetData;
    }

    void *GetMem()
    {
        return m_pvMem;
    }

    void *GetDataMem()
    {
        return m_pDataBuff;
    }

    /**
     * @brief У������ָ���Ƿ���Ч
     */
    int VerifyData(DATATYPE *pData)
    {
        int iMemOffSet = ((char *)pData - (char *)m_pvMem);
        int iDataOffSet = iMemOffSet - sizeof(Head) - (sizeof(int)*(m_pstHead->iNodeNum) - ((m_pstHead->iNodeNum+7)/8));

        if ((iDataOffSet%sizeof(DATATYPE)) == 0)
        {
            return SUCCESS;
        }

        return ERROR;
    }

    int GetMemSize()
    {
        return m_pstHead->iMemSize;
    }

    int GetNodeNum()
    {
        return m_pstHead->iNodeNum;
    }

    int GetUsedNodeNum()
    {
        return m_pstHead->iUsedNodeNum;
    }

    void Show()
    {
        printf("==Head==\n");
        printf("MemSize=%d\nNodeNum=%d\nUsedNodeNum=%d\nFreeHead=%d\nFreeTail=%d\n",
                m_pstHead->iMemSize,
                m_pstHead->iNodeNum,
                m_pstHead->iUsedNodeNum,
                m_pstHead->iFreeHead,
                m_pstHead->iFreeTail);
        printf("==HeadEnd==\n");


        printf("==FreeList==\n");
        for(int i=0;i<m_pstHead->iNodeNum;i++)
        {
            printf(" %02d", m_piFreeList[i]);
            if ((i%20)==19)
            {
                printf("\n");
            }
        }
        printf("\n==FreeListEnd==\n");

        printf("==UsedBit==\n");
        for (int i=0;i<m_pstHead->iNodeNum;i++)
        {
            printf("%d",((m_pucUsedBit[i/8]&(1<<(i%8)))==0)?0:1);
            if ((i%50)==49)
            {
                printf("\n");
            }
        }
        printf("\n==UsedBitEnd==\n");

    }
};

}   //namespace snslib
#endif /*_FIXEDSIZE_MEM_POOL_H_*/
