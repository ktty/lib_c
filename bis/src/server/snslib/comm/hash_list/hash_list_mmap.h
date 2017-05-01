/**
 * @file    hash_list_mmap.h
 * @brief   �ڴ�ӳ���ļ�+�ļ���ʵ�ֵ�HashList(��ϣ����)��ģ����
 * @author  jamieli@tencent.com
 * @date    2008-08-12
 */
#ifndef _HASH_LIST_MMAP_H_
#define _HASH_LIST_MMAP_H_

#include "hash_list.h"
#include "comm/mmap_file/mmap_file.h"
#include "comm/file_lock/file_lock.h"

//�����ڲ�ָ�붼ʹ��ƫ����
namespace snslib
{

template<class KEYTYPE, class DATATYPE>
class CHashListMMap: public CHashList<KEYTYPE, DATATYPE>
{
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
    CHashListMMap(void)
	{
	}

	virtual ~CHashListMMap(void)
	{
	}

	int Init(void *pvMem, int iMemSize, int iIndexNum, int iDataNum)
	{
	    //������ʹ�øú������г�ʼ��
	    return ERROR;
	}
	virtual int Init(const char *pszMapFile, int iMemSize, int iIndexNum, int iDataNum)
	{

        int iRetVal = 0;

        //��ʼ���ڴ湲���ļ�
        iRetVal = m_objMMapFile.Create(pszMapFile, iMemSize);
        if (iRetVal != SUCCESS)
        {
            return iRetVal;
        }

        void *pMem = m_objMMapFile.GetMem();

        //��ʼ��HashList����
        iRetVal = CHashList<KEYTYPE, DATATYPE>::Init(pMem, iMemSize, iIndexNum, iDataNum);
        if (iRetVal != SUCCESS)
        {
            return iRetVal;
        }

        //��ʼ���ļ���
        iRetVal = m_objFileLock.Init(pszMapFile);
        if (iRetVal != SUCCESS)
        {
            return iRetVal;
        }

		return SUCCESS;
	}

	//����д��
	//summary:���Ҫʵ�ַ���HashList���⣬��Ҫ��������������
	//        ����ͨ����������ʵ�ֲ��������д����Ҳ����ͨ���ź���ʵ��
	//        �����洢������
	//param:iType��ʾ�������� 1-ReadLock 2-WriteLock
	//param:iOffSet��ʾ��ʼ����λ��
	//param:iSize��ʾ��Ҫ��������
	//return: 0-�ɹ� !=0-ʧ��
	int Lock(int iType, int iOffSet, int iSize)
	{
	    return m_objFileLock.Lock(iType, iOffSet, iSize);
	}

	//�ͷ���
	int UnLock(int iOffSet, int iSize)
	{
	    return m_objFileLock.UnLock(iOffSet, iSize);
	}

protected:
    CFileLock m_objFileLock;
    CMMapFile m_objMMapFile;
};

}

#endif
