#include <errno.h>
#include <sys/stat.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h> ///< for pthread_*()
#include <stdlib.h>
#include "pet_log.h"
#include "comm/file_lock/file_lock.h"

using namespace snslib;

CPetLog g_objPetLog;

#define SELF_LOG(iLogID, uiUin, iLogLevel, szFormat, args...) \
{ \
    if (iLogLevel <= m_iSelfLogLevel) \
    { \
        SelfLog(iLogID, uiUin, iLogLevel, szFormat, ##args); \
    } \
} while(false)

const char *CPetLog::GetLogHeadTime(time_t tSec, time_t tUsec)
{
    static char szGetLogHeadTimeRet[64] = {0};
    struct tm *stSecTime = localtime(&tSec); //ת��ʱ���ʽ
    int year = 1900 + stSecTime->tm_year; //��1900�꿪ʼ������
    int month = stSecTime->tm_mon + 1; //��0��ʼ������
    int day = stSecTime->tm_mday; //��1��ʼ������
    int hour = stSecTime->tm_hour; //��0��ʼ��Сʱ��
    int min = stSecTime->tm_min; //��0��ʼ�ķ�����
    int sec = stSecTime->tm_sec; //��0��ʼ������

    sprintf(szGetLogHeadTimeRet, "%04d-%02d-%02d %02d:%02d:%02d.%06ld", year, month, day, hour, min, sec, tUsec);
    return szGetLogHeadTimeRet;
}

const char *CPetLog::GetFileNameTime(time_t tSec)
{
    static char szGetFileNameTimeRet[64] = {0};
    struct tm *stSecTime = localtime(&tSec); //ת��ʱ���ʽ
    int year = 1900 + stSecTime->tm_year; //��1900�꿪ʼ������
    int month = stSecTime->tm_mon + 1; //��0��ʼ������
    int day = stSecTime->tm_mday; //��1��ʼ������
    int hour = stSecTime->tm_hour; //��0��ʼ��Сʱ��

    sprintf(szGetFileNameTimeRet, "%04d%02d%02d%02d", year, month, day, hour);
    return szGetFileNameTimeRet;
}

int CPetLog::CheckDir(const char * szPath)
{
	int iRetVal = ERROR;
    if (NULL == szPath)
    {
        return iRetVal;
    }

    umask(0);

    struct stat stBuf;
    if (lstat(szPath, &stBuf) < 0)
    {
    	if(mkdir(szPath, 0777) != -1)
    	{
    		iRetVal = SUCCESS;
    	}
    }
    else
    {
    	iRetVal = SUCCESS;
    }

    umask(0022);

    return iRetVal;
}

// 2014��03��13�� 16:03:51/shimmeryang
// ����Ŀ¼��֧��Ƕ��Ŀ¼����
int CPetLog::MakeDir(const char * szPath,bool bIsFilePath)
{
	if (szPath == NULL)
	{
		return ERROR;
	}

	int iLength = strlen(szPath);

	if (iLength > 0)
	{
		char szDir[512] = { 0 };
		for (int i = 0; i < iLength; i++)
		{
			szDir[i] = szPath[i];
			if (szPath[i] == '/' && i != 0)
			{
				//printf("mkdir %s\n",szDir);
				if (CheckDir(szDir) != SUCCESS)
				{
					printf("mkdir faild!dir:%s\n", szDir);
					return ERROR;
				}
			}
		}
		if (szDir[iLength - 1] != '/' && !bIsFilePath)
		{
			//printf("mkdir %s\n",szDir);
			if (CheckDir(szDir) != SUCCESS)
			{
				printf("mkdir faild!dir:%s\n", szDir);
				return ERROR;
			}
		}
	}
	return SUCCESS;
}

// 2014��03��13�� 16:04:51/shimmeryang
// ���ģ������ĸ����㼶��Ŀ¼
void CPetLog::CheckDirAll(const char * szPath)
{
    if (NULL == szPath)
    {
        return;
    }
    struct stat stBuf;
    char szDir[256] =
    { 0 };

    umask(0);

    if (lstat(szPath, &stBuf) < 0)
    { //no such path, return
        mkdir(szPath, 0777);
        lstat(szPath, &stBuf);
    }

    if (!S_ISDIR(stBuf.st_mode))
    { //not a directory, return
        return;
    }

    //check sub dir
    snprintf(szDir, sizeof(szDir), "%s/info", szPath);
    if (lstat(szDir, &stBuf) < 0)
    { // sub directory not exist,create
        mkdir(szDir, 0777);
    }

    snprintf(szDir, sizeof(szDir), "%s/error", szPath);
    if (lstat(szDir, &stBuf) < 0)
    { //sub directory not exist,create
        mkdir(szDir, 0777);
    }

    snprintf(szDir, sizeof(szDir), "%s/debug", szPath);
    if (lstat(szDir, &stBuf) < 0)
    { //sub directory not exist,create
        mkdir(szDir, 0777);
    }

    snprintf(szDir, sizeof(szDir), "%s/trace", szPath);
    if (lstat(szDir, &stBuf) < 0)
    { //sub directory not exist,create
        mkdir(szDir, 0777);
    }

    snprintf(szDir, sizeof(szDir), "%s/warn", szPath);
    if (lstat(szDir, &stBuf) < 0)
    { //sub directory not exist,create
        mkdir(szDir, 0777);
    }

    umask(0022);
}

//����ļ��Ƿ񳬹������ļ���С
//ע�⣬�ڸú����ڲ�����ʹ��SELF_LOG����
int CPetLog::CheckFileSize(FILE *fpFile, int iFileSize)
{
    int iRetVal = 0;
    struct stat stFileStat;

    if (fpFile == NULL)
    {
        return -1;
    }

    int iFD = fileno(fpFile);
    if (iFD < 0)
    {
        return 0;
    }
    else
    {
        iRetVal = fstat(iFD, &stFileStat);
        if (iRetVal != 0)
        {
            return 0;
        }
        else
        {
            //log_api��߼���ļ���Сʱ���ٺ���logsvr����ֹ����ͬʱ��⵽
            if (stFileStat.st_size > iFileSize)
            {
                return 1;
            }
            else
            {
                return 0;
            }
        }
    }
}

//ע�⣬�ڸú����ڲ�����ʹ��SELF_LOG����
int CPetLog::RenameFile(const char *pszFileName, int iMaxFileNum)
{
    int iRetVal = 0;
    char szCurrFileName[1024] = {0};
    CFileLock objFileLock;

    iRetVal = objFileLock.Init(pszFileName);
    if (iRetVal != 0)
    {
        return -1;
    }

    iRetVal = objFileLock.Lock(objFileLock.FILE_LOCK_WRITE, 0, 0, 0);
    if (iRetVal != 0)
    {
        //�����������ڽ����ļ���������
        return -2;
    }

    //���и�������
    if (iMaxFileNum <= 0)
    {
        iMaxFileNum = PETLOG_DEFAULT_MAX_FILE_NUM;
    }

    int iCurrFileNum = 0;
    for(int i=1; i<iMaxFileNum; i++)
    {
        snprintf(szCurrFileName, sizeof(szCurrFileName), "%s.%02d", pszFileName, i);
        iRetVal = access(szCurrFileName, F_OK);
        if (iRetVal == 0)
        {
            //ָ�����ļ�����
            iCurrFileNum = i;
            continue;
        }
        else
        {
            //ָ�����ļ�������
            break;
        }
    }

    if (iCurrFileNum == (iMaxFileNum-1))
    {
        snprintf(szCurrFileName, sizeof(szCurrFileName), "%s.%02d", pszFileName, iCurrFileNum);
        unlink(szCurrFileName);
        iCurrFileNum--;
    }

    char szOldFileName[1024] = {0};
    char szNewFileName[1024] = {0};
    for(int i=iCurrFileNum; i>0; i--)
    {
        snprintf(szOldFileName, sizeof(szOldFileName), "%s.%02d", pszFileName, i);
        snprintf(szNewFileName, sizeof(szNewFileName), "%s.%02d", pszFileName, i+1);
        rename(szOldFileName, szNewFileName);
    }

    snprintf(szNewFileName, sizeof(szNewFileName), "%s.01", pszFileName);
    rename(pszFileName, szNewFileName);

    objFileLock.UnLock(0, 0);

    return 0;
}

//ע�⣬�ڸú����ڲ�����ʹ��SELF_LOG����
FILE *CPetLog::OpenFile(const char *pszFileName, const char *pszMode)
{
    FILE *fpRetFile;

    umask(0);
    fpRetFile = fopen(pszFileName, pszMode);
    umask(0022);

    return fpRetFile;
}


CPetLog::CPetLog()
{

    m_iInitFlag = 0;

    m_iSelfLogLevel = 3;

    m_pfSelfLogFile = NULL;
    m_iSelfLogTime = 0;
    m_tSelfLogLastSizeCheckTime = 0;

    m_bSnsLogQueueFlag = false;

    snprintf(m_szPetLogPath, sizeof(m_szPetLogPath), "/tmp/log/petlog");
}

CPetLog::~CPetLog()
{
    m_iInitFlag = 0;
}

int CPetLog::Open(const char *pszModuleName, int iLogLevel)
{
	int iRetVal = 0;
    m_objModuleName = pszModuleName;

    //����Ƿ�������PETLOG_SELFLOG_LEVEL��������
    char *pszSelfLogLevel = getenv("PETLOG_SELFLOG_LEVEL");
    char *pszPetLogPath = getenv("PETLOG_SELFLOG_PATH");

    if(pszPetLogPath != NULL)
    {
    	snprintf(m_szPetLogPath, sizeof(m_szPetLogPath), "%s/pet_log",pszPetLogPath);
    }

    if (pszSelfLogLevel != NULL)
    {
        m_iSelfLogLevel = PETLOG_TRACE;
        SELF_LOG(0, 0, PETLOG_INFO, "petlog_selflog_level=%s", pszSelfLogLevel);
        m_iSelfLogLevel = atoi(pszSelfLogLevel);
    }

    if (m_iSelfLogLevel > 0)
    {
        if(CheckDir(m_szPetLogPath) != SUCCESS)
        {
        	if(MakeDir(m_szPetLogPath)!= SUCCESS)
        	{
        		return ERROR;
        	}
        }
    }

    // 2014��03��13�� 16:18:11/shimmeryang
    // ���һЩ��־�������۲�logsvr������
    SELF_LOG(0, 0, PETLOG_TRACE, "pet_log_path=%s", m_szPetLogPath);

    iRetVal = m_objLogQueue.Init(PETLOG_QUEUE_KEY, PETLOG_QUEUE_SIZE);
    if (iRetVal != 0)
    {
        SELF_LOG(0, 0, PETLOG_ERR, "init log_queue failed, key=%d, size=%d, ret=%d", PETLOG_QUEUE_KEY, PETLOG_QUEUE_SIZE, iRetVal);
        return ERROR;
    }

    // 2014��03��13�� 17:06:37/shimmeryang
    // TODO: ���queue����������ļ������õĻ�������������
    iRetVal = m_objSnsLogQueue.Init(PETLOG_SNS_QUEUE_KEY, PETLOG_SNS_QUEUE_SIZE);
    if (iRetVal != 0)
    {
        SELF_LOG(0, 0, PETLOG_ERR, "init sns_log_queue failed, key=%d, size=%d, ret=%d", PETLOG_SNS_QUEUE_KEY, PETLOG_SNS_QUEUE_SIZE, iRetVal);
        m_bSnsLogQueueFlag = false;
    }
    m_bSnsLogQueueFlag = true;

    iRetVal = m_objLogConfShm.Create(PETLOG_CONF_SHM_KEY, PETLOG_CONF_SHM_SIZE, 0666);
    if ((iRetVal != 0)&&(iRetVal != 1))
    {
        SELF_LOG(0, 0, PETLOG_ERR, "create conf_shm failed, key=%d, size=%d, ret=%d", PETLOG_CONF_SHM_KEY, PETLOG_CONF_SHM_SIZE, iRetVal);
        return ERROR;
    }

    iRetVal = m_objLogConfShm.Attach();
    if (iRetVal != 0)
    {
        SELF_LOG(0, 0, PETLOG_ERR, "attach conf_shm failed, ret=%d", iRetVal);
        return ERROR;
    }

    m_objLogConf.m_pstGlobalConf = (PetLogOutputConf *)m_objLogConfShm.GetMem();
    int iModuleConfHashListSize = m_objLogConf.m_objModuleConf.CalcSize(PETLOG_MAX_SPEC_MODULE_NUM, PETLOG_MAX_SPEC_MODULE_NUM);
    int iLogIDConfHashListSize = m_objLogConf.m_objLogIDConf.CalcSize(PETLOG_MAX_SPEC_LOGID_NUM, PETLOG_MAX_SPEC_LOGID_NUM);
    int iUINConfHashListSize = m_objLogConf.m_objUINConf.CalcSize(PETLOG_MAX_SPEC_UIN_NUM, PETLOG_MAX_SPEC_UIN_NUM);
    int iLogCentreHashListSize = m_objLogConf.m_objLogCentreLogIDConf.CalcSize(PETLOG_MAX_LOG_CENTRE_LOGID_NUM, PETLOG_MAX_LOG_CENTRE_LOGID_NUM);

    if ((sizeof(PetLogOutputConf) + iModuleConfHashListSize + iLogIDConfHashListSize + iUINConfHashListSize + iLogCentreHashListSize) > ((unsigned int)PETLOG_CONF_SHM_SIZE))
    {
        SELF_LOG(0, 0, PETLOG_ERR, "all_conf_shm_size=%d, allocated_size=%d", (sizeof(PetLogOutputConf) + iModuleConfHashListSize + iLogIDConfHashListSize + iUINConfHashListSize), PETLOG_CONF_SHM_SIZE);
        return ERROR;
    }

    iRetVal = m_objLogConf.m_objModuleConf.Init((char *)m_objLogConf.m_pstGlobalConf + sizeof(PetLogOutputConf), iModuleConfHashListSize, PETLOG_MAX_SPEC_MODULE_NUM, PETLOG_MAX_SPEC_MODULE_NUM);
    if (iRetVal != 0)
    {
        SELF_LOG(0, 0, PETLOG_ERR, "init module_conf_hash_list failed, ret=%d", iRetVal);
        return ERROR;
    }
    iRetVal = m_objLogConf.m_objLogIDConf.Init((char *)m_objLogConf.m_pstGlobalConf + sizeof(PetLogOutputConf) + iModuleConfHashListSize, iLogIDConfHashListSize, PETLOG_MAX_SPEC_LOGID_NUM, PETLOG_MAX_SPEC_LOGID_NUM);
    if (iRetVal != 0)
    {
        SELF_LOG(0, 0, PETLOG_ERR, "init logid_conf_hash_list failed, ret=%d", iRetVal);
        return ERROR;
    }

    iRetVal = m_objLogConf.m_objUINConf.Init((char *)m_objLogConf.m_pstGlobalConf + sizeof(PetLogOutputConf) + iModuleConfHashListSize + iLogIDConfHashListSize, iUINConfHashListSize, PETLOG_MAX_SPEC_UIN_NUM, PETLOG_MAX_SPEC_UIN_NUM);
    if (iRetVal != 0)
    {
        SELF_LOG(0, 0, PETLOG_ERR, "init uin_conf_hash_list failed, ret=%d", iRetVal);
        return ERROR;
    }

    iRetVal = m_objLogConf.m_objLogCentreLogIDConf.Init((char *)m_objLogConf.m_pstGlobalConf + sizeof(PetLogOutputConf) + iModuleConfHashListSize + iLogIDConfHashListSize + iUINConfHashListSize, iLogCentreHashListSize, PETLOG_MAX_LOG_CENTRE_LOGID_NUM, PETLOG_MAX_LOG_CENTRE_LOGID_NUM);
    if (iRetVal != 0)
    {
        SELF_LOG(0, 0, PETLOG_ERR, "init log_centre_hash_list failed, ret=%d", iRetVal);
        return ERROR;
    }

    if (m_objLogConf.m_pstGlobalConf->iLogConfShmInitFlag != PETLOG_CONFSHM_INIT_FLAG)
    {
        SELF_LOG(0, 0, PETLOG_DEBUG, "log_conf_shm have not inited, flag=0x%08X", m_objLogConf.m_pstGlobalConf->iLogConfShmInitFlag);

        m_objLogConf.m_pstGlobalConf->iLogConfShmInitFlag = PETLOG_CONFSHM_INIT_FLAG;
        m_objLogConf.m_pstGlobalConf->iOutputLogLevel = PETLOG_DEFAULT_LOG_LEVEL;
        m_objLogConf.m_pstGlobalConf->iOutputLogType = 1;
        snprintf(m_objLogConf.m_pstGlobalConf->szOutputPathDir, sizeof(m_objLogConf.m_pstGlobalConf->szOutputPathDir), PETLOG_DEFAULT_LOG_PATH);
        m_objLogConf.m_pstGlobalConf->iMaxFileSize = PETLOG_DEFAULT_MAX_FILE_SIZE;
        m_objLogConf.m_pstGlobalConf->iMaxFileNum = PETLOG_DEFAULT_MAX_FILE_NUM;
        m_objLogConf.m_pstGlobalConf->iFlushTime = 10;

        iRetVal = m_objLogConf.m_objModuleConf.Clear();
        if (iRetVal != 0)
        {
            SELF_LOG(0, 0, PETLOG_ERR, "clear module_conf_hash_list failed, ret=%d", iRetVal);
            return ERROR;
        }
        iRetVal = m_objLogConf.m_objLogIDConf.Clear();
        if (iRetVal != 0)
        {
            SELF_LOG(0, 0, PETLOG_ERR, "clear logid_conf_hash_list failed, ret=%d", iRetVal);
            return ERROR;
        }

        iRetVal = m_objLogConf.m_objUINConf.Clear();
        if (iRetVal != 0)
        {
            SELF_LOG(0, 0, PETLOG_ERR, "clear uin_conf_hash_list failed, ret=%d", iRetVal);
            return ERROR;
        }

        iRetVal = m_objLogConf.m_objLogCentreLogIDConf.Clear();
        if (iRetVal != 0)
        {
            SELF_LOG(0, 0, PETLOG_ERR, "clear log_centre_hash_list failed, ret=%d", iRetVal);
            return ERROR;
        }
    }

    if (m_objLogConf.m_pstGlobalConf->iMaxFileSize < PETLOG_DEFAULT_MIN_FILE_SIZE)
    {
        //��ֹ�����ϰ屾��LogSvr���°汾��PetLogAPI
        m_objLogConf.m_pstGlobalConf->iMaxFileSize = PETLOG_DEFAULT_MIN_FILE_SIZE;
        m_objLogConf.m_pstGlobalConf->iMaxFileNum = PETLOG_DEFAULT_MAX_FILE_NUM;
        SELF_LOG(0, 0, PETLOG_ERR, "global_conf/max_file_size is not valid, set to default %d", PETLOG_DEFAULT_MIN_FILE_SIZE);
    }

    iRetVal = m_objLogConf.m_objModuleConf.Verify();
    if (iRetVal != 0)
    {
        SELF_LOG(0, 0, PETLOG_ERR, "verify module_conf_hash_list failed, ret=%d", iRetVal);
        return ERROR;
    }

    iRetVal = m_objLogConf.m_objLogIDConf.Verify();
    if (iRetVal != 0)
    {
        SELF_LOG(0, 0, PETLOG_ERR, "verify logid_conf_hash_list failed, ret=%d", iRetVal);
        return ERROR;
    }

    iRetVal = m_objLogConf.m_objUINConf.Verify();
    if (iRetVal != 0)
    {
        SELF_LOG(0, 0, PETLOG_ERR, "verify uin_conf_hash_list failed, ret=%d", iRetVal);
        return ERROR;
    }

    iRetVal = m_objLogConf.m_objLogCentreLogIDConf.Verify();
    if (iRetVal != 0)
    {
        SELF_LOG(0, 0, PETLOG_ERR, "verify log_centre_hash_list failed, ret=%d", iRetVal);
        return ERROR;
    }

    m_iInitFlag = 1;
    return SUCCESS;
}

void CPetLog::Log(int iLogID, unsigned uiUin, int iPriority, const char* pszLogContent)
{
    int iRetVal = 0;

    if (m_iInitFlag != 1)
    {
        //û�г�ʼ����־
        return;
    }

    if (!IsShouldWrite(iLogID, uiUin, iPriority))
    {
        //����Ҫ�����־
        return;
    }

    PetLogHeader *pstPetLogHeader = (PetLogHeader *)m_szLogBuff;
    memset(pstPetLogHeader, 0x0, sizeof(PetLogHeader));

    struct timeval stTimeNow;
    gettimeofday(&stTimeNow, NULL);

    pstPetLogHeader->uiLogTimeSec = stTimeNow.tv_sec;
    pstPetLogHeader->uiLogTimeUSec = stTimeNow.tv_usec;
    pstPetLogHeader->iLogLevel = iPriority;
    pstPetLogHeader->iLogID = iLogID;
    pstPetLogHeader->iProcID = getpid();
    strncpy(pstPetLogHeader->szModuleName, m_objModuleName.m_szModuleName, sizeof(pstPetLogHeader->szModuleName)-1);
    pstPetLogHeader->uiUin = uiUin;

    int iLogBodyLen = 0;

    iLogBodyLen = snprintf(pstPetLogHeader->szLogContent, PETLOG_LENGTH_MAX, "%s", pszLogContent);

    SELF_LOG(0, uiUin, PETLOG_TRACE, "%d|%d|LOG_CONTENT(%d)[%s]", iLogID, iPriority, iLogBodyLen, pstPetLogHeader->szLogContent);

    iRetVal = m_objLogQueue.InQueue(m_szLogBuff, iLogBodyLen + sizeof(PetLogHeader));
    if (iRetVal != m_objLogQueue.SUCCESS)
    {
        //TODO ������Queueʧ�ܵ������ֻ��дһ��SELFLOG
        SELF_LOG(0, uiUin, PETLOG_WARN, "log in queue failed, content=%s", pstPetLogHeader->szLogContent);
    }

    if (m_bSnsLogQueueFlag && IsShouldSnsWrite(iLogID, uiUin, iPriority))
    {
        iRetVal = m_objSnsLogQueue.InQueue(m_szLogBuff, iLogBodyLen + sizeof(PetLogHeader));
        if (iRetVal != m_objSnsLogQueue.SUCCESS)
		{
            SELF_LOG(0, uiUin, PETLOG_WARN, "log in sns_queue failed, content=%s", pstPetLogHeader->szLogContent);
		}
    }

    return;
}

int CPetLog::GetLogLevel()
{
    return m_objLogConf.m_pstGlobalConf->iOutputLogLevel;
}

bool CPetLog::IsShouldWrite(int iLogID, unsigned int uiUin, int iPriority)
{
    if (m_iInitFlag != 1)
    {
        //û�г�ʼ����־
        return false;
    }

    if ((iPriority < PETLOG_INFO)||(iPriority > PETLOG_TRACE))
    {
        return false;
    }

    //�ж���־�ȼ�
    int iLogOutputFlag = 0;         //0-��Ҫ����� 1-Ҫ�����

    SELF_LOG(0, uiUin, PETLOG_DEBUG, "IS_SHOULD_WRITE|%d|%d|%d", m_objLogConf.m_pstGlobalConf->iOutputLogLevel, iLogID, iPriority);

    PetLogOutputConf stPetLogOutputConf;
    if(iPriority > m_objLogConf.m_pstGlobalConf->iOutputLogLevel)
    {
        //�ж��Ƿ�����������ģ�����
        if ((m_objLogConf.m_objModuleConf.GetUsedDataNum() > 1)&&((m_objLogConf.m_objModuleConf.Get(m_objModuleName, stPetLogOutputConf) == m_objLogConf.m_objModuleConf.SUCCESS)&&(iPriority <= stPetLogOutputConf.iOutputLogLevel)))
        {
            iLogOutputFlag = 1;
        }

        //�ж��Ƿ�����������LogID���
        if ((m_objLogConf.m_objLogIDConf.GetUsedDataNum() > 1)&&((m_objLogConf.m_objLogIDConf.Get(iLogID, stPetLogOutputConf) == m_objLogConf.m_objLogIDConf.SUCCESS)&&(iPriority <= stPetLogOutputConf.iOutputLogLevel)))
        {
            iLogOutputFlag = 1;
        }

        //�ж��Ƿ�����������UIN���
        if ((m_objLogConf.m_objUINConf.GetUsedDataNum() > 1)&&((m_objLogConf.m_objUINConf.Get(uiUin, stPetLogOutputConf) == m_objLogConf.m_objUINConf.SUCCESS)&&(iPriority <= stPetLogOutputConf.iOutputLogLevel)))
        {
            iLogOutputFlag = 1;
        }

        //�ж��Ƿ������˽��������־ID�������־����
        if ((m_objLogConf.m_objLogCentreLogIDConf.GetUsedDataNum() > 1)&&((m_objLogConf.m_objLogCentreLogIDConf.Get(iLogID, stPetLogOutputConf) == m_objLogConf.m_objLogCentreLogIDConf.SUCCESS)&&(iPriority <= stPetLogOutputConf.iOutputLogLevel)))
        {
            iLogOutputFlag = 1;
        }
    }
    else
    {
        iLogOutputFlag = 1;
    }

    if (iLogOutputFlag != 1)
    {
        //����־����Ҫ���
        return false;
    }

    return true;
}

bool CPetLog::IsShouldSnsWrite(int iLogID, unsigned int uiUin, int iPriority)
{
    if (m_iInitFlag != 1)
    {
        //û�г�ʼ����־
        return false;
    }

    if ((iPriority < PETLOG_INFO)||(iPriority > PETLOG_TRACE))
    {
        return false;
    }

    //�ж���־�ȼ�
    int iLogOutputFlag = 0;         //0-��Ҫ����� 1-Ҫ�����

    SELF_LOG(0, uiUin, PETLOG_DEBUG, "IS_SHOULD_SNS_WRITE|%d|%d|%d", m_objLogConf.m_pstGlobalConf->iOutputLogLevel, iLogID, iPriority);
    PetLogOutputConf stPetLogOutputConf;

    //�ж��Ƿ������˽��������־ID�������־����
    if ((m_objLogConf.m_objLogCentreLogIDConf.GetUsedDataNum() > 1)&&((m_objLogConf.m_objLogCentreLogIDConf.Get(iLogID, stPetLogOutputConf) == m_objLogConf.m_objLogCentreLogIDConf.SUCCESS)&&(iPriority <= stPetLogOutputConf.iOutputLogLevel)))
    {
        iLogOutputFlag = 1;
    }

    if (iLogOutputFlag != 1)
    {
        //����־����Ҫ���
        return false;
    }

    return true;
}

void CPetLog::SelfLog(int iLogID, unsigned int uiUin, int iPriority, const char* pszFormat, ...)
{
    //������ļ�����һ�ο��أ���������²�����õ��ú������������ֻ���ڶ�λ��־����ʱʹ��

    if (iPriority > m_iSelfLogLevel)
    {
        return;
    }

    PetLogHeader stPetLogHeader;
    memset(&stPetLogHeader, 0x0, sizeof(stPetLogHeader));

    struct timeval stTimeNow;
    gettimeofday(&stTimeNow, NULL);
    time_t tTimeNow = stTimeNow.tv_sec;

    stPetLogHeader.uiLogTimeSec = stTimeNow.tv_sec;
    stPetLogHeader.uiLogTimeUSec = stTimeNow.tv_usec;
    stPetLogHeader.iLogLevel = iPriority;
    stPetLogHeader.iLogID = iLogID;
    stPetLogHeader.iProcID = getpid();
    strncpy(stPetLogHeader.szModuleName, m_objModuleName.m_szModuleName, sizeof(stPetLogHeader.szModuleName)-1);
    stPetLogHeader.uiUin = uiUin;

    char szLogHeaderBuff[256];
    int iLogHeadLen = 0;
    char szLogType[32] = {0};

    switch(stPetLogHeader.iLogLevel)
    {
        case PETLOG_INFO:
        {
            snprintf(szLogType, sizeof(szLogType), "INFO");
            break;
        }
        case PETLOG_ERR:
        {
            snprintf(szLogType, sizeof(szLogType), "ERRO");
            break;
        }
        case PETLOG_WARN:
        {
            snprintf(szLogType, sizeof(szLogType), "WARN");
            break;
        }
        case PETLOG_DEBUG:
        {
            snprintf(szLogType, sizeof(szLogType), "DEBU");
            break;
        }
        case PETLOG_TRACE:
        {
            snprintf(szLogType, sizeof(szLogType), "TRAC");
            break;
        }
        default:
        {
            return;
        }
    }

    iLogHeadLen = snprintf(szLogHeaderBuff, sizeof(szLogHeaderBuff), "%s|%s|%s(%d)|%d|%u||||||",
            szLogType,
            GetLogHeadTime(stPetLogHeader.uiLogTimeSec, stPetLogHeader.uiLogTimeUSec),
            stPetLogHeader.szModuleName,
            stPetLogHeader.iProcID,
            stPetLogHeader.iLogID,
            stPetLogHeader.uiUin
            );

    int iCurLogTime = atoi(GetFileNameTime(stPetLogHeader.uiLogTimeSec));

    char szLogFileFullName[1024];

	if(m_pfSelfLogFile == NULL)
	{
		snprintf(szLogFileFullName, sizeof(szLogFileFullName), "%s/%d.log", m_szPetLogPath, iCurLogTime);
		m_pfSelfLogFile = OpenFile(szLogFileFullName, "a");
        m_iSelfLogTime = iCurLogTime;
        m_tSelfLogLastSizeCheckTime = tTimeNow;
	}

    if (m_iSelfLogTime != iCurLogTime)
    {
        if (m_pfSelfLogFile != NULL)
        {
            fclose(m_pfSelfLogFile);
        }
        snprintf(szLogFileFullName, sizeof(szLogFileFullName), "%s/%d.log", m_szPetLogPath, iCurLogTime);
        m_pfSelfLogFile = OpenFile(szLogFileFullName, "a");
        m_iSelfLogTime = iCurLogTime;
        m_tSelfLogLastSizeCheckTime = tTimeNow;
    }
    else if (((tTimeNow - m_tSelfLogLastSizeCheckTime) < 0)||(tTimeNow - m_tSelfLogLastSizeCheckTime) > PETLOG_SIZE_CHECK_TIMEVAL)
    {
        if (CheckFileSize(m_pfSelfLogFile, SELFLOG_MAX_SIZE) != 0)
        {
            if (m_pfSelfLogFile != NULL)
            {
                fclose(m_pfSelfLogFile);
            }
            snprintf(szLogFileFullName, sizeof(szLogFileFullName), "%s/%d.log", m_szPetLogPath, iCurLogTime);
            m_pfSelfLogFile = OpenFile(szLogFileFullName, "a");
            if (CheckFileSize(m_pfSelfLogFile, SELFLOG_MAX_SIZE) != 0)
            {
                fclose(m_pfSelfLogFile);
                RenameFile(szLogFileFullName, PETLOG_DEFAULT_MAX_FILE_NUM);
                m_pfSelfLogFile = OpenFile(szLogFileFullName, "a");
            }
            m_iSelfLogTime = iCurLogTime;
        }
        m_tSelfLogLastSizeCheckTime = tTimeNow;
    }

    char szLogContentBuff[PETLOG_LENGTH_MAX];
    va_list otherarg;
    va_start(otherarg, pszFormat);
    vsnprintf(szLogContentBuff, sizeof(szLogContentBuff), pszFormat, otherarg);
    va_end(otherarg);

    fprintf(m_pfSelfLogFile, "%s%s\n", szLogHeaderBuff, szLogContentBuff);

    return;
}

int OpenPetLog(const char* pszModuleName, int iLogLevel /*=0*/, const char* pszLogFilePath /*=NULL*/)
{
    return g_objPetLog.Open(pszModuleName, iLogLevel);
}
void PetLogInternal(int iLogID, unsigned int uiUin, int iPriority, const char* pszFormat, ...)
{
    //TODO ������Զ���ȫ�ֱ���������һ���ڴ濽��
    va_list otherarg;
    va_start(otherarg, pszFormat);
    char szLogContent[PETLOG_LENGTH_MAX+1];
    vsnprintf(szLogContent, PETLOG_LENGTH_MAX, pszFormat, otherarg);
    va_end(otherarg);
    g_objPetLog.Log(iLogID, uiUin, iPriority, szLogContent);
}
void ClosePetLog()
{
}

int GetLogLevel()
{
    return g_objPetLog.GetLogLevel();
}

