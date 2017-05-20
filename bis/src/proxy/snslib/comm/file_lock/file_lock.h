/**
 * @file    file_lock.h
 * @brief   �ļ���
 * @author  jamieli@tencent.com
 * @date    2008-07-02
 */

#ifndef _FILE_LOCK_H_
#define _FILE_LOCK_H_

namespace snslib
{

class CFileLock
{
public:
    const static int SUCCESS = 0;
    const static int ERROR = -1;

    const static int E_FILE_LOCK_CREATE_FILE = -201;

public:
	const static int FILE_LOCK_READ = 1;
	const static int FILE_LOCK_WRITE = 2;

	CFileLock(void);
	~CFileLock(void);

	int Init(const char *pszFileName);

	/**
	 * @brief ���ļ������������ʧ�ܣ����ȴ�
	 * @param iOffSet ������ƫ����
	 * @param iSize �����������С
	 * @param iWaitFlag ����ʧ���Ƿ�ȴ���Ĭ��Ϊ�ȴ�
	 *
	 * @return 0-�ɹ� ����-ʧ��
	 */
	int Lock(int iType, int iOffSet, int iSize, int iWaitFlag = 1);

    /**
     * @brief ����ļ���
     * @param iOffSet ������ƫ����
     * @param iSize �����������С
     *
     * @return 0-�ɹ� ����-ʧ��
     */
	int UnLock(int iOffSet, int iSize);

private:
	int m_iFile;

};

}

#endif
