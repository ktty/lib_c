/**
 * @file    hash_db.h
 * @brief   ��tdb���з�װ�Ľӿ���
 * @author  jamieli@tencent.com
 * @date    2010-03-30
 */

#ifndef _HASH_DB_H_
#define _HASH_DB_H_

#include "comm/hash_db/tchdb.h"

namespace snslib
{
class CHashDB
{
public:
    const static int SUCCESS = 0;
    const static int ERROR = -1;

    const static int HDBE_NORECORD = 101;
    const static int HDBE_DBFULL = 102;

public:

    /**
     @brief ���캯��
     */
    CHashDB();

    /**
     @brief ��������
     */
    ~CHashDB();

    /**
     * brief ��ʼ��
     * param pszConfFile �����ļ���
     */
    int Init(const char *pszConfFile, const char * pszSection = "HASH_DB" );

    /**
     * brief ����/���Ǽ�¼
     * param pvKeyBuff ���key��buffָ��
     * param iKeySize key�ĳ���
     * param pvValBuff �����ֵ��buffָ��
     * param iValSize val�ĳ���
     *
     * note ���øýӿڣ�����Ѿ������˶�Ӧkey�ļ�¼�����ᱻ���ǣ����û�ж�Ӧkey�ļ�¼����������
     */
    int Put(const void *pvKeyBuff, int iKeySize, const void *pvValBuff, int iValSize);

    /**
     * brief ����/���Ǽ�¼
     * param pszKey ���key���ַ�����������\0��β
     * param pszVal �����ֵ���ַ�����������\0��β
     *
     * note ���øýӿڣ�����Ѿ������˶�Ӧkey�ļ�¼�����ᱻ���ǣ����û�ж�Ӧkey�ļ�¼����������
     */
    int Put2(const char *pszKey, const char *pszVal);

    /**
     * brief ���Ӽ�¼
     * param pvKeyBuff ���key��buffָ��
     * param iKeySize key�ĳ���
     * param pvValBuff �����ֵ��buffָ��
     * param iValSize val�ĳ���
     *
     * note ���øýӿڣ�����Ѿ������˶�Ӧkey�ļ�¼��������ʾ�������û�ж�Ӧkey�ļ�¼����������
     */
    int PutKeep(const void *pvKeyBuff, int iKeySize, const void *pvValBuff, int iValSize);

    /**
     * brief ���Ӽ�¼
     * param pszKey ���key���ַ�����������\0��β
     * param pszVal �����ֵ���ַ�����������\0��β
     *
     * note ���øýӿڣ�����Ѿ������˶�Ӧkey�ļ�¼��������ʾ�������û�ж�Ӧkey�ļ�¼����������
     */
    int PutKeep2(const char *pszKey, const char *pszVal);

    /**
     * brief ��ǰ��¼׷��/����
     * param pvKeyBuff ���key��buffָ��
     * param iKeySize key�ĳ���
     * param pvValBuff �����ֵ��buffָ��
     * param iValSize val�ĳ���
     *
     * note ���øýӿڣ�����Ѿ������˶�Ӧkey�ļ�¼�����µ�����׷�ӵ�ԭ�����ݺ��棬���û�ж�Ӧkey�ļ�¼��������
     */
    int PutCat(const void *pvKeyBuff, int iKeySize, const void *pvValBuff, int iValSize);

    /**
     * brief ��ǰ��¼׷��/����
     * param pszKey ���key���ַ�����������\0��β
     * param pszVal �����ֵ���ַ�����������\0��β
     *
     * note ���øýӿڣ�����Ѿ������˶�Ӧkey�ļ�¼�����µ�����׷�ӵ�ԭ�����ݺ��棬���û�ж�Ӧkey�ļ�¼��������
     */
    int PutCat2(const char *pszKey, const char *pszVal);

    /**
     * brief ɾ����¼
     * param pvKeyBuff ���key��buffָ��
     * param iKeySize key�ĳ���
     *
     * note ���øýӿڣ�ɾ��ָ��key�ļ�¼�����ܴ�����񣬶��᷵�سɹ�
     */
    int Out(const void *pvKeyBuff, int iKeySize);

    /**
     * brief ɾ����¼
     * param pszKey ���key���ַ�����������\0��β
     *
     * note ���øýӿڣ�ɾ��ָ��key�ļ�¼�����ܴ�����񣬶��᷵�سɹ�
     */
    int Out2(const char *pszKey);

    /**
     * brief ��ȡ��¼
     * param pvKeyBuff ���key��buffָ��
     * param iKeySize key�ĳ���
     * param ppvValBuff �����ֵ��buffָ���ָ��
     * param piValSize �ڲ����ٿռ�Ĵ�С
     *
     * note ppvValBuffָ��Ŀռ䲻��Ҫ�ⲿ���٣��ɺ����ڲ����٣����ٵĳ���ΪpiValSize����Ҫ�ⲿ�����ͷ�
     */
    int Get(const void *pvKeyBuff, int iKeySize, void **ppvValBuff, int *piValSize);

    /**
     * brief ��ȡ��¼
     * param pszKey ���key���ַ�����������\0��β
     * param ppszVal �����ֵ��buffָ���ָ�룬��\0��β
     *
     * note ppszValָ��Ŀռ䲻��Ҫ�ⲿ���٣��ɺ����ڲ����٣�δ���ؿ��ٳ��ȣ���Ҫ�ⲿ�����ͷ�
     */
    int Get2(const char *pszKey, char **ppszVal);

    /**
     * brief ��ȡ��¼
     * param pvKeyBuff ���key��buffָ��
     * param iKeySize key�ĳ���
     * param pvValBuff �ⲿ���Val��Buff��С���洢�ռ����ⲿ����
     * param piValBuffSize IN/OUT ����ʱ����ΪBuff��С����������ʱ����ΪVal�ĳ���
     *
     * note pvValBuff�������ⲿ���٣�ͬʱ��Buff������ΪpiValBuffSize�������룬����洢�ռ䲻��val�洢���ӿڽ����ش���
     */
    int Get3(const void *pvKeyBuff, int iKeySize, void *pvValBuff, int *piValBuffSize);

    /**
     * brief ��ȡĳ����¼VAL�ĳ���
     * param pvKeyBuff ���key��buffָ��
     * param iKeySize key�ĳ���
     * param piValSize OUT���ظü�¼�ĳ���
     */
    int GetVSize(const void *pvKeyBuff, int iKeySize, int *piValSize);

    /**
     * brief ��ȡĳ����¼VAL�ĳ���
     * param pszKey ���key���ַ�����������\0��β
     * param piValSize OUT���ظü�¼�ĳ���
     */
    int GetVSize2(const char *pszKey, int *piValSize);

    /**
     * brief ����ʱ����Ӧ��ָ��ָ��ĳ����¼
     * param pvKeyBuff ��Ҫ��iteratorָ��ļ�¼��Ӧ��key
     * param iKeySize ��Ҫ��iteratorָ��ļ�¼��Ӧkey�ĳ���
     *
     * note ���iKeySize=0����pvKeyBuff=NULL��iterator��ָ��DB�еĵ�һ����¼
     */
    int IterInit(const void *pvKeyBuff = NULL, int iKeySize = 0);

    /**
     * brief ����ʱ����Ӧ��ָ��ָ��ĳ����¼
     * param pszKey ��Ҫ��iteratorָ��ļ�¼��Ӧ��key����\0��β
     *
     * note ���pszKey=NULL����strlen(pszKey)=0��iterator��ָ��DB�еĵ�һ����¼
     */
    int IterInit2(const char *pszKey = NULL);

    /**
     * brief ����ʱ����ȡ��һ��Keyֵ
     * param ppvKeyBuff �����ֵ��keyָ���ָ��
     * param piKeySize Key�ĳ���
     *
     * note ppvKeyBuff��Ӧ�Ĵ洢�ռ����ڲ����٣��ⲿ�ͷ�
     */
    int IterNextKey(void **ppvKeyBuff, int *piKeySize);

    /**
     * brief ����ʱ����ȡ��һ��Keyֵ
     * param ppszKeyBuff �����ֵ��keyָ���ָ�룬��'\0'��β
     *
     * note ppszKeyBuff��Ӧ�Ĵ洢�ռ����ڲ����٣��ⲿ�ͷ�
     */
    int IterNextKey2(char **ppszKeyBuff);

    /**
     * brief ����ʱ����ȡ��һ��Keyֵ
     * param pvKeyBuff �����ֵ��keyָ��
     * param piKeyBuffSize IN/OUT ����Buff�Ĵ�С��ͬʱ��ʾ����Key�ĳ���
     *
     * note pvKeyBuff��Ӧ�Ĵ洢�ռ����ⲿ����
     */
    int IterNextKey3(void *pvKeyBuff, int *piKeyBuffSize);

    /**
     * brief ����ʱ����ȡ��һ��Key/Valֵ
     * param ppvKeyBuff �����ֵ��keyָ���ָ��
     * param piKeySize Key�ĳ���
     * param ppvValBuff �����ֵ��valָ���ָ��
     * param piValSize val�ĳ���
     *
     * note ppvKeyBuff ppvValBuff�����ɺ����ڲ����٣���Ҫ�ⲿ�ͷ�
     */
    int IterNext(void **ppvKeyBuff, int *piKeySize, void **ppvValBuff, int *piValSize);

    /**
     * brief ����ʱ����ȡ��һ��Key/Valֵ
     * param ppszKeyBuff �����ֵ��keyָ���ָ��
     * param ppszValBuff �����ֵ��valָ���ָ��
     *
     * note ppszKeyBuff ppszValBuff�����ɺ����ڲ����٣���Ҫ�ⲿ�ͷ�
     */
    int IterNext2(char **ppszKeyBuff, char **ppszValBuff);

    /**
     * brief ����ʱ����ȡ��һ��Key/Valֵ
     * param pvKeyBuff �����ֵ��keyָ��
     * param piKeyBuffSize IN/OUT ����Buff�ĳ��ȣ�����Key�ĳ���
     * param pvValBuff �����ֵ��valָ��
     * param piValBuffSize IN/OUT ����Buff�ĳ��ȣ�����Val�ĳ���
     *
     * note pvKeyBuff pvValBuff���ⲿ����ռ�
     */
    int IterNext3(void *pvKeyBuff, int *piKeyBuffSize, void *pvValBuff, int *piValBuffSize);

    /**
     * brief �������DB
     *
     * note ����ɾ������DB�������еļ�¼
     */
    int VanishDB();

    unsigned long long GetFileSize();

    unsigned long long GetRecordNum();

    void ShowDBInfo();

    const char *GetErrMsg()
    {
        return m_szErrMsg;
    }

private:
    char m_szErrMsg[256];

    TCHDB *m_pstTCHDB;

};
} /* snslib */

#endif
