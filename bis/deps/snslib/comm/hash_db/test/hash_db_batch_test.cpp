#include <stdio.h>
#include <string.h>
#include <sys/time.h>

#include "comm/hash_db/hash_db.h"
#include "comm/log/pet_log.h"
#include "comm/util/pet_util.h"

using namespace snslib;

const int HDB_BATCH_TEST_MAX_VAL_LEN = 102400;

void usage(const char *pszProgName)
{
    printf("usage: %s <conf_file> <key_len_avg> <val_len_avg> <val_len_chg_pct> <out_cnt_pct> <get_cnt_pct> <key_max_num> <loop_num> [debug_flag]\n", pszProgName);
    printf(" key_len_avg - key��ƽ������\n");
    printf(" val_len_avg - val��ƽ������\n");
    printf(" val_len_chg_pct - val���ȱ仯�ı��ʣ���λ%%\n");
    printf(" out_cnt_pct - ����out�Ĵ�������\n");
    printf(" get_cnt_pct - ����get�Ĵ�������\n");
    printf(" key_max_num - key�����ֵ\n");
    printf(" loop_num - ѭ������\n");
}

int main(int argc, char *argv[])
{
    if (argc < 9)
    {
        usage(argv[0]);
        return -1;
    }

    const char *pszConfFile = argv[1];
    int iKeyLenAvg = atoi(argv[2]);
    int iValLenAvg = atoi(argv[3]);
    int iValLenChgPct = atoi(argv[4]);
    int iOutCntPct = atoi(argv[5]);
    int iGetCntPct = atoi(argv[6]);
    int iKeyMaxNum = atoi(argv[7]);
    int iLoopNum = atoi(argv[8]);

    bool bDebugFlag = false;
    if (argc > 9)
    {
        bDebugFlag = (atoi(argv[9]) != 0);
    }
    if ((iKeyLenAvg <= 14) || (iKeyLenAvg > 100))
    {
        printf("key_len_avg[%d] is not valid\n", iKeyLenAvg);
        return -1;
    }

    if ((iValLenAvg <= 0) || (iValLenAvg >= (HDB_BATCH_TEST_MAX_VAL_LEN/2)))
    {
        printf("val_len_avg[%d] is not valid\n", iValLenAvg);
        return -1;
    }

    if ((iValLenChgPct < 0)||(iValLenChgPct >= 100))
    {
        printf("val_len_chg_pct[%d] is not valid\n", iValLenChgPct);
        return -1;
    }

    if ((iOutCntPct < 0)||(iOutCntPct >= 100)
            ||(iGetCntPct < 0)||(iGetCntPct >= 100)
            ||((iOutCntPct + iGetCntPct) >= 100))
    {
        printf("out_cnt_pct[%d] get_cnt_pct[%d] is not valid\n", iOutCntPct, iGetCntPct);
        return -1;
    }

    if (iKeyMaxNum <= 0)
    {
        printf("key_max_num[%d] is not valid\n", iKeyMaxNum);
        return -1;
    }

    if (iLoopNum < 0)
    {
        printf("loop_num[%d] is not valid\n", iLoopNum);
        return -1;
    }

    OpenPetLog("hdb_batch_test");

    int iRetVal = 0;
    CHashDB objDB;

    iRetVal = objDB.Init(pszConfFile);
    if (iRetVal != 0)
    {
        printf("db init failed, ret=%d, errmsg=%s\n", iRetVal, objDB.GetErrMsg());
        return -1;
    }

    char szValBuff[HDB_BATCH_TEST_MAX_VAL_LEN];
    char szValBuffTmp[HDB_BATCH_TEST_MAX_VAL_LEN];
    char szKeyBuff[HDB_BATCH_TEST_MAX_VAL_LEN];
    memset(szValBuff, 'V', sizeof(szValBuff));

    int iPutNum = 0;
    int iGetNum = 0;
    int iOutNum = 0;

    int iPutENum = 0;
    int iGetENum = 0;
    int iOutENum = 0;

    int iPutNumAll = 0;
    int iGetNumAll = 0;
    int iOutNumAll = 0;

    int iPutENumAll = 0;
    int iGetENumAll = 0;
    int iOutENumAll = 0;

    struct timeval stTimeBegin, stTimeLast, stTimeNow;

    gettimeofday(&stTimeBegin, NULL);
    stTimeLast = stTimeBegin;

    printf("%-17s|%-10s|%-10s|%-10s|%-10s|%-10s|%-10s|%-10s|%-10s|%-10s|%-10s|%-10s\n", "TIME", "PUT", "GET", "OUT", "PUTE", "GETE", "OUTE", "FILESIZE", "RNUM", "PUTS", "GETS", "OUTS");

    for(int i=0; i<iLoopNum; i++)
    {
        int iNowKey = CRandomTool::Instance()->Get(0, iKeyMaxNum);

        int iNowPct = i%100;

        if (bDebugFlag)printf("-----------------BEGIN-----------------\n");
        if (bDebugFlag)objDB.ShowDBInfo();

        if (iNowPct < (100 - iOutCntPct - iGetCntPct))
        {

            if (bDebugFlag)printf("------------------PUT------------------\n");
            //ִ�в������
            int iNowValLen = iValLenAvg - (iValLenAvg * iValLenChgPct / 100) + (2 *  iValLenAvg * CRandomTool::Instance()->Get(0, iValLenChgPct) / 100);
            snprintf(szKeyBuff, sizeof(szKeyBuff), "KK%010dKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKK", iNowKey);

            iRetVal = objDB.Put(&iNowKey, sizeof(int), szValBuff, iNowValLen);
            if (iRetVal == CHashDB::HDBE_DBFULL)
            {
                memcpy(szValBuffTmp, szKeyBuff, iKeyLenAvg);
                szValBuffTmp[iKeyLenAvg] = '\0';
                //PetLog(0, 0, PETLOG_WARN, "PUT_FAIL|%s|%d|%d|%s", szValBuffTmp, iNowValLen, iRetVal, objDB.GetErrMsg());
            }
            else if (iRetVal != 0)
            {
                iPutENum++;
                iPutENumAll++;
                memcpy(szValBuffTmp, szKeyBuff, iKeyLenAvg);
                szValBuffTmp[iKeyLenAvg] = '\0';
                PetLog(0, 0, PETLOG_WARN, "PUT_FAIL|%s|%d|%d|%s", szValBuffTmp, iNowValLen, iRetVal, objDB.GetErrMsg());
            }
            else
            {
                iPutNum++;
                iPutNumAll++;
            }
        }
        else if ((iNowPct >= (100 - iOutCntPct - iGetCntPct))&&(iNowPct < (100 - iOutCntPct)))
        {
            if (bDebugFlag)printf("------------------GET------------------\n");
            //ִ�ж�ȡ����
            snprintf(szKeyBuff, sizeof(szKeyBuff), "KK%010dKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKK", iNowKey);

            int iValBuffTmpLen = sizeof(szValBuffTmp);
            iRetVal = objDB.Get3(&iNowKey, sizeof(int), szValBuffTmp, &iValBuffTmpLen);
            if ((iRetVal != 0)&&(iRetVal != CHashDB::HDBE_NORECORD))
            {
                iGetENum++;
                iGetENumAll++;
                memcpy(szValBuffTmp, szKeyBuff, iKeyLenAvg);
                szValBuffTmp[iKeyLenAvg] = '\0';
                PetLog(0, 0, PETLOG_WARN, "GET_FAIL|%s|%d|%s", szValBuffTmp, iRetVal, objDB.GetErrMsg());
            }
            else
            {
                iGetNum++;
                iGetNumAll++;
            }
        }
        else if (iNowPct >= (100 - iOutCntPct))
        {
            if (bDebugFlag)printf("------------------OUT------------------\n");
            //ִ��ɾ������
            snprintf(szKeyBuff, sizeof(szKeyBuff), "KK%010dKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKK", iNowKey);

            iRetVal = objDB.Out(&iNowKey, sizeof(int));
            if ((iRetVal != 0)&&(iRetVal != CHashDB::HDBE_NORECORD))
            {
                iOutENum++;
                iOutENumAll++;
                memcpy(szValBuffTmp, szKeyBuff, iKeyLenAvg);
                szValBuffTmp[iKeyLenAvg] = '\0';
                PetLog(0, 0, PETLOG_WARN, "OUT_FAIL|%s|%d|%s", szValBuffTmp, iRetVal, objDB.GetErrMsg());
            }
            else
            {
                iOutNum++;
                iOutNumAll++;
            }
        }

        if (bDebugFlag)printf("------------------END------------------\n");
        if (bDebugFlag)objDB.ShowDBInfo();

        if ((i+1)%10000 == 0)
        {
            gettimeofday(&stTimeNow, NULL);

            long long llTimeVal = (long long)(stTimeNow.tv_sec - stTimeLast.tv_sec) * 1000000 + (stTimeNow.tv_usec - stTimeLast.tv_usec);

            int iPutS = (long long)iPutNum * 1000000 / llTimeVal;
            int iGetS = (long long)iGetNum * 1000000 / llTimeVal;
            int iOutS = (long long)iOutNum * 1000000 / llTimeVal;

            printf("%010d.%06d|%10d|%10d|%10d|%10d|%10d|%10d|%10llu|%10llu|%10d|%10d|%10d\n", (int)stTimeNow.tv_sec, (int)stTimeNow.tv_usec, iPutNum, iGetNum, iOutNum, iPutENum, iGetENum, iOutENum, objDB.GetFileSize(), objDB.GetRecordNum(), iPutS, iGetS, iOutS);

            iPutNum = 0;
            iGetNum = 0;
            iOutNum = 0;

            iPutENum = 0;
            iGetENum = 0;
            iOutENum = 0;

            stTimeLast = stTimeNow;
        }

    }

    gettimeofday(&stTimeNow, NULL);
    printf("%d.%06d|%10d|%10d|%10d|%10d|%10d|%10d|%10llu|%10llu\n", (int)stTimeNow.tv_sec, (int)stTimeNow.tv_usec, iPutNumAll, iGetNumAll, iOutNumAll, iPutENumAll, iGetENumAll, iOutENumAll, objDB.GetFileSize(), objDB.GetRecordNum());

    return 0;
}
