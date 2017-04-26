#ifndef _TT_FEEDS_API_H_
#define _TT_FEEDS_API_H_

#include <stdio.h>
#include <string>
#include <vector>
#include <map>

/**
 * Feeds Key Define:
 * uin FeedsVal
 * uin_nc NewFeeds Count
 */

namespace snslib
{

typedef std::map<std::string, std::string> keyval;

class CTTFeedsApi
{
public:
    const static int MAX_TT_HOST_NUM = 10;
    const static int MAX_TT_FEEDS_LEN = 8192;
    const static int MAX_TT_BUFF_LEN = MAX_TT_FEEDS_LEN + 1024;
    const static int MAX_TT_KEY_LEN = 64;

    const static int TT_FEEDS_ERR_NORECORED = 101;
    const static int TT_FEEDS_ERR_NOTSUPPORT = 102;    //����û������FeedsCount������GetCount��ȡ��Ϣ

    CTTFeedsApi();
    ~CTTFeedsApi();

    int Init(const char *pszConfFile);

    int AddFeeds(unsigned int uiUin, const std::string sFeedsInfo);

    int GetFeeds(unsigned int uiUin, unsigned long long ullFeedsID, std::string &sFeedsInfo);

    int GetAllFeeds(unsigned int uiUin, std::vector<unsigned long long> &vullFeedsID, std::vector<std::string> &vsFeedsInfo, int iCleanNewCountFlag = 0);

    int DelFeeds(unsigned int uiUin, unsigned long long ullFeedsID);

    int DelAllFeeds(unsigned int uiUin);

    int GetNewFeedsCount(unsigned int uiUin, unsigned int *puiNewFeedsCount);

    int CleanNewFeedsCount(unsigned int uiUin);

    // �������������ɶ�ʹ��   $APPID$��$MSGID$��ΪAppID��MsgID��key����MapKeyVal������ʹ������Ϊkey
    int AddFeedsKeyVal(unsigned int uiUin, int nAppID, int nMsgID, keyval &MapKeyVal);

    int GetAllFeedsKeyVal(
    		unsigned int uiUin,
    		std::vector<unsigned long long> &vullFeedsID,	// feedsʱ��
    		std::vector<keyval> &vecFeedsKeyVal,			// feedsԭʼ��keyvalֵ
    		std::vector<std::string> &vsFeeds,				// ���ṩ��Ϣģ��ʱ���᷵��������feeds��¼
    		int iCleanNewCountFlag = 0);

private:
    void *m_pstRDBMS[MAX_TT_HOST_NUM];
    int m_iHostNum;
    int m_iCountFlag;   //�Ƿ��¼δ��������־λ 0-����¼ 1-��¼
    char m_szFeedsTemplatesConf[1024];
    std::map<std::string, std::string> m_mapFeedsTemplates;

    int GetHostIdx(unsigned int uiUin);
    int ReadFeedsTemplates();
    int GenFeedsInfo(std::vector<keyval> &vecFeedsKeyVal, std::vector<std::string> &vsFeedsInfo);

    char m_szKeyVal[MAX_TT_KEY_LEN];
    int m_iKeyLen;

    int m_iKeepNum;
    int m_iKeepSec;

    int CheckUserFeeds(unsigned int uiUin);

};
}

#endif
