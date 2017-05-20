/**
 * @file    pet_util.h
 * @brief   һЩ��װ�õ�ʹ�ù��ߺ���
 */

#ifndef _PET_UTIL_H_
#define _PET_UTIL_H_

#include <stdarg.h>
#include <string>
#include <vector>
#include <inttypes.h>
#include <sys/time.h>

namespace snslib
{

class CBuffTool
{
public:

    static int ReadByte(const void *pvBuffer, unsigned char &ucVal);
    static int ReadByte(const void *pvBuffer, char &cVal);
    static int WriteByte(void *pvBuffer, unsigned char ucVal);
    static int WriteByte(void *pvBuffer, char cVal);

    static int ReadShort(const void *pvBuffer, unsigned short &ushVal, int iToHostOrder = 1);
    static int ReadShort(const void *pvBuffer, short &shVal, int iToHostOrder = 1);
    static int WriteShort(void *pvBuffer, unsigned short ushVal, int iToNetOrder = 1);
    static int WriteShort(void *pvBuffer, short shVal, int iToNetOrder = 1);

    static int ReadInt(const void *pvBuffer, unsigned int &uiVal, int iToHostOrder = 1);
    static int ReadInt(const void *pvBuffer, int &iVal, int iToHostOrder = 1);
    static int WriteInt(void *pvBuffer, unsigned int uiVal, int iToNetOrder = 1);
    static int WriteInt(void *pvBuffer, int iVal, int iToNetOrder = 1);

    static int ReadLong(const void *pvBuffer, unsigned long &ulVal, int iToHostOrder = 1);
    static int ReadLong(const void *pvBuffer, long &lVal, int iToHostOrder = 1);
    static int WriteLong(void *pvBuffer, unsigned long ulVal, int iToNetOrder = 1);
    static int WriteLong(void *pvBuffer, long lVal, int iToNetOrder = 1);

    static int ReadLongLong(const void *pvBuffer, unsigned long long &ullVal, int iToHostOrder = 0);
    static int ReadLongLong(const void *pvBuffer, long long &llVal, int iToHostOrder = 0);
    static int WriteLongLong(void *pvBuffer, unsigned long long ullVal, int iToNetOrder = 0);
    static int WriteLongLong(void *pvBuffer, long long llVal, int iToNetOrder = 0);

#if __WORDSIZE == 64
    static int ReadLongLong(const void *pvBuffer, uint64_t &llVal, int iToHostOrder = 0);
    static int WriteLongLong(void *pvBuffer, uint64_t llVal, int iToNetOrder = 0);
#endif
    static int ReadString(const void *pvBuffer, char *pszVal, int iStrLen);
    static int WriteString(void *pvBuffer, const char *pszVal, int iStrLen);

    static int ReadBuf(const void *pvBuffer, void *pszVal, int iStrLen);
    static int WriteBuf(void *pvBuffer, const void *pszVal, int iStrLen);
};

class CFileUtil
{
public:
  static bool ReadFile(const std::string &path, std::string &data);
  static bool ReadFile(const std::string &path, char *ptr, size_t &len);
  static bool FileExists(const std::string &path);
};

class CStrTool
{
    static void InternalAppend(std::string& sDst, const char* pszFmt, va_list stApList);
public:
    static const char *Str2Hex(const void *pBuff, int iSize);
	// By michaelzhao 2010-08-23 Ϊʲô��Str2Hex��û��Hex2Str
	static const char* Hex2Str(const char *pBuff, int iSize, int *pSize);
    static unsigned short CheckSum(const void *pvBuff, int iSize);

    // Added by Jiffychen@2010-03-30
    // Replace any characters in pszRemove with chReplaceWith
    static void StripString(std::string* psStr, const char* pszRemove, char chReplaceWith);

    // Replace any occurance of sOldSub in sStr by sNewSub,
    // The result is appended to psRes, if bReplaceAll is false, only the
    // first occurance will be replaced
    static void StringReplace(const std::string& sStr, const std::string& sOldSub,
        const std::string& sNewSub, bool bReplaceAll, std::string* psRes);

    // See above, the result is returned
    static std::string StringReplace(const std::string& sStr, const std::string& sOldSub,
        const std::string& sNewSub, bool bReplaceAll);

    // Split sFull by any character in pszDelim
    static void SplitStringUsing(const std::string& sFull,
        const char* pszDelim, std::vector<std::string>* pvsResult);

    static void SplitStringUsing(const std::string& sFull,
        const char* pszDelim, std::vector<std::string>* pvsResult, int iMaxSplitTimes);

    // Join the items with pszDelim as a separator
    static void JoinStrings(const std::vector<std::string>& vsItems, const char* pszDelim,
        std::string * psResult);

    static std::string JoinStrings(const std::vector<std::string>& vsItems,
        const char* pszDelim);

    // Convert the given int to its string representation
    static std::string SimpleItoa(int i);

    // Format to string
    static std::string Format(const char* pszFmt, ...);

    // Append mode
    static void Append(std::string& sDst, const char* pszFmt, ...);

    // time_t to str
    static const char *TimeString(time_t tTime);

    // str to time_t
    static time_t FromTimeString(const char *pszTimeStr);

    static const int BASE64_TYPE_NORMAL = 0;
    static const int BASE64_TYPE_URL = 1;
    static int Base64Encode(const void *pvDataBuff, int iDataLen, void *pvEncodeDataBuff, int *iEncodeDataLen, int iBase64Type = BASE64_TYPE_NORMAL);
    static int Base64Decode(const void *pvDataBuff, int iDataLen, void *pvDecodeDataBuff, int *iDecodeDataLen, int iBase64Type = BASE64_TYPE_NORMAL);
    //����Ϊ����ת������������Խ�utf8תΪgbk����������ת��
    static int CodeConvert(const char* from_charset,const char* to_charset, const char* inbuf, size_t inlen, char* outbuf, size_t& outbyteslef);
    static int U2G(const char* inbuf,size_t inlen, char* outbuf, size_t& outlen);
    static int G2U(const char* inbuf,size_t inlen, char* outbuf, size_t& outlen);

    static int U2G(std::string &p_rInOut);
    static int G2U(std::string &p_rInOut);

	static bool CheckValidNameGBK(const char *pszInStr, char *pszOutStr);

    // timeval to str
    static const char *TimeString(struct ::timeval stTime);

    //ip addr to str
    static const char *IPString(unsigned int uiIP);
    static unsigned long long SizeStrToNum(const char *pszSize);
    static int IsUTF8(const char *pszBuf, size_t uBuflen);
    /* ����utf8�����ַ����еķǷ��ַ��ͺ����������jsonʧ�ܵ��ַ�������\" �� \\�� \/�� \b�� \f�� \n�� \r�� \t�� \u
     * ��ȷ��strIn��utf8���룬���ط���utf8�淶���ַ���
     */
    static std::string FilterUtf8ForJson(const std::string &strIn);
};

class CSysTool
{
public:
    const static int SUCCESS = 0;
    const static int ERROR = -1;

    const static int E_SYS_TOOL_MODULE_NAME = -101;
    const static int E_SYS_TOOL_OPEN_FILE = -102;
    const static int E_SYS_TOOL_LOCK_FILE = -103;
    const static int E_SYS_TOOL_FORK = -104;

    /*
     * @brief ����Ƿ������ͬ����
     * @param:pszModuleName ģ����
     *
     * @note �ú������ûᴴ��pid�ļ���/usr/local/pet50/pid/<ModuleName>.pid���������������ļ�
     */
    static int LockProc(const char* pszModuleName);

    static int DaemonInit();

    static const char *GetNicAddr(const char *pszIfName);
	
	static const char *GetExecutableName(const char *path);


    static long long GetFileSize(const char * pszFileName);

    static long long GetFileSize(int iFD);

    static time_t GetFileMTime(const char * pszFileName);

    static bool IsSameDay(time_t t1, time_t t2);

    static int diff_days(time_t t1, time_t t2);

    static bool IsSameWeek(time_t t1, time_t t2);

    /* �õ���һ������һ��ʱ��� */
    static time_t NextWeek(time_t timestamp);
};

class CRandomTool
{
public:
    static CRandomTool * Instance()
    {
        static CRandomTool * p = new CRandomTool;

        return p;
    }

    //return val [iMin iMax)
    int Get(int iMin, int iMax);

    long GetLong(long lMin, long lMax);

protected:
    CRandomTool();
};

/*
 * class CSingleton
 */
template <typename T>
class CSingleton
{
public:
    static T & Instance()
    {
        static T instance;
        return instance;
    }

protected:
    CSingleton() {}
};


/**
 * add by kevin
 *
 *�������Ҫ�����ڳ��õ�һЩ����������ɻ���������ʵĲ�����
 *������и��û�Ҳ�ܳ��õķ���������û�У���ӭ���۲��������
 */

class CRand
{
public:

        //����0-iMax֮����������  [0,iMax)
    static int Random(int iMax );

    //����iarrPosibilities�еĸ����б������еڼ����[0, iCount-1]
    static int RandNumPrizeLevel(int iarrPosibilities[],  int iCount );

    //�����������time(NULL)+pidΪ���ӵģ����Ҫͬʱ�����ܶ����������мǲ�Ҫʹ��������ʹ��Random()
    static int GetRand(int iMin, int iMax);

    static int  Random(int iMin, int iMax );

};

// copy from petlib by shimmeryang
class CHtmlUtil
{
    //��������ַ������Էŵ�js�������ַ����У�����ʾ��ҳ����
public:
    static std::string simpleHtmlEncode(std::string src);

    /**
    GBK���뷶Χ(ch1Ϊ���ַ���ch2Ϊ�ڶ����ַ�):
    0x81<=ch1<=0xFE && (0x40<=ch2<=0x7E || 0x7E<=ch2<=0xFE)

    GB2312
    0xA1<=ch1<=0xF7 && (0xA1<=ch2<=0xFE)
    */
    static void cutGBRaw( char *szInfo, unsigned int uiMaxLen );

    static std::string cutGB(std::string str);

};

class  CTimeTools
{
public:
    //��2010�����ھ���������
    static unsigned int DayFrom2010(time_t tTheDay);

    //����time_t���һ��Ψһ������
    static unsigned short UniqDay(time_t t);


    //����˵��: �ж�ĳ��ʱ���Ƿ�������ʱ���֮��
    //��������: 0���ڻʱ����  <0 ����  >0�ڻʱ����
    //��������: ǰ����������ʾ��ʼ�ͽ���ʱ�䣬���һ������������Ҫ�Ƚϵ�ʱ��,Ĭ��Ϊ0ָ��ǰʱ��
    //          szActBegin��szActEnd����ʽ���: %Y-%m-%d %H:%M:%S ��2010-01-28 11:24:00
    static int   BetweenTime(const char*  szActBegin, const char* szActEnd, time_t  timeNow = 0);

    //szTheDay �Ǿ�ȷ������ַ�����ʾ %Y-%m-%d �� "2011-02-24"
    //timeNowĬ���ǵ�ǰʱ��time(NULL),Ҳ��������
    //*pdaydiff ���ص�ǰʱ������쵽Ŀ��ʱ��
    //����szTheDay="2011-02-24"
    //��������� 2011-02-23������ʱ�̣� ��ô����1
    //��������� 2011-02-24������ʱ�̣� ��ô����0
    //��������� 2011-02-25������ʱ�̣� ��ô����-1
    //����ֵ 0=ok�� -1=szTheDayʱ���ʽ����
    static int DayDiff(int* pdaydiff,const char* szTheDay, time_t  timeNow = 0);

};

}

#endif

