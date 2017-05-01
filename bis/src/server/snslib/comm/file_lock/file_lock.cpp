/**
 * @file    file_lock.cpp
 * @brief   �ļ���
 * @author  jamieli@tencent.com
 * @date    2008-07-02
 */

#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

#include <stdio.h>

#include "file_lock.h"

using namespace snslib;

CFileLock::CFileLock(void)
{
	m_iFile = -1;
}

CFileLock::~CFileLock(void)
{
	if (m_iFile != -1)
	{
		close(m_iFile);
	}
}

/*
 * @summary:ͨ���ļ��������ļ���
 * @param:pszFileName:�ļ���
 * @return 0 success, !=0 failed
 */
int CFileLock::Init(const char *pszFileName)
{
	m_iFile = open(pszFileName, O_RDWR|O_CREAT, 00644);
	if (-1 == m_iFile)
	{
		return E_FILE_LOCK_CREATE_FILE;
	}

	return SUCCESS;
}

/*
 * @summary:��ָ�����������
 * @param:iType:�������ͣ� FILE_LOCK_READ:���� other:д��
 * @param:iOffSet:������λ��
 * @param:iSize:����������
 * @return 0 success, !=0 failed
 */
int CFileLock::Lock(int iType, int iOffSet, int iSize, int iWaitFlag /*= 1*/)
{
	int iRetVal = 0;
	struct flock lstFlock;
	memset(&lstFlock, 0, sizeof(lstFlock));

	if (FILE_LOCK_READ == iType)
	{
		lstFlock.l_type = F_RDLCK;
	}
	else
	{
		lstFlock.l_type = F_WRLCK;
	}

	lstFlock.l_whence = SEEK_SET;
	lstFlock.l_start = iOffSet;
	lstFlock.l_len = iSize;

	//printf("DEBUG:Lock|%d|%d|%d\n", iType, iOffSet, iSize);
	//TODO �����жϵ��������Ҫ����
	if (iWaitFlag == 1)
	{
	    while (true)
	    {
	        iRetVal = fcntl (m_iFile, F_SETLKW, &lstFlock);
	        if (iRetVal != 0)
	        {
	            if (errno == EINTR)
	            {
	                //���жϵĲ�����������
	                //printf("fcntl lock interrupted\n");
	                continue;
	            }
	            else
	            {
	                //��ʧ��
	                //printf("fcntl lock failed, errno=%d\n", errno);
	                break;
	            }
	        }
	        else
	        {
	            break;
	        }
	    }
	}
	else
	{
        iRetVal = fcntl (m_iFile, F_SETLK, &lstFlock);
	}

	if ( iRetVal != 0)
	{
		return ERROR;
	}

	return SUCCESS;
}

/*
 * @summary:��ָ�������������
 * @param:iOffSet:������λ��
 * @param:iSize:����������
 * @return 0 success, !=0 failed
 */
int CFileLock::UnLock(int iOffSet, int iSize)
{
	int iRetVal = 0;
	struct flock lstFlock;
	memset (&lstFlock, 0, sizeof(lstFlock));

	lstFlock.l_type = F_UNLCK;
	lstFlock.l_whence = SEEK_SET;
	lstFlock.l_start = iOffSet;
	lstFlock.l_len = iSize;

	//printf("DEBUG:UnLock|%d|%d\n", iOffSet, iSize);
    while (true)
    {
        iRetVal = fcntl (m_iFile, F_SETLKW, &lstFlock);
        if (iRetVal != 0)
        {
            if (errno == EINTR)
            {
                //���жϵĲ�������������
                //printf("fcntl unlock interrupted\n");
                continue;
            }
            else
            {
                //����ʧ��
                //printf("fcntl unlock failed, errno=%d\n", errno);
                break;
            }
        }
        else
        {
            break;
        }
    }
    if ( iRetVal != 0)
    {
        return ERROR;
    }

	return SUCCESS;
}
