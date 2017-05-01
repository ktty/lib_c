#ifndef PAY_DEFINE_H
#define PAY_DEFINE_H

namespace snslib
{

// ֧����������
const unsigned char PAY_CHANNEL_QQACCT          = 1;    // QB֧��
const unsigned char PAY_CHANNEL_QQPOINT         = 2;    // Q��֧��
const unsigned char PAY_CHANNEL_QB_QPOINT       = 3;    // ���֧������QB��Q��
const unsigned char PAY_CHANNEL_QPOINT_QB       = 4;    // ���֧��, ��Q����QB
const unsigned char PAY_CHANNEL_TENPAY          = 5;    // ��Ϸ�ʻ�֧��

// Item���Ͷ���
const unsigned char PAY_ITEM_TYPE_COMMODITY     = 0;    // ֧����Ʒ
const unsigned char PAY_ITEM_TYPE_GOODS         = 1;    // ֧����Ʒ
const unsigned char PAY_ITEM_TYPE_HAPPYFIGHT    = 2;    // ֧�����ֶ���Ʒ
const unsigned char PAY_ITEM_TYPE_KONGFU        = 3;    // ֧��������Ʒ

// session��֤���Ͷ���
const unsigned char PAY_SESSION_QQ_CLIENT       = 1;    // �ͻ��˵�¼key
const unsigned char PAY_SESSION_COMMUNITY       = 2;    // ������¼key
const unsigned char PAY_SESSION_QQ_3G           = 3;    // 3G key
const unsigned char PAY_SESSION_NO_SESSION      = 4;    // ��sessionģʽ����Ҫ��������

// ���Ը��£���Ϊ����
typedef struct tagPayAttr
{
    int     yb;             // Ԫ��
    int     growth;         // �ɳ�ֵ
    int     starvation;     // ����ֵ
    int     cleanness;      // ���ֵ
    short   feeling;        // ����ֵ
    int     strong;         // ����ֵ
    int     iq;             // ����ֵ
    int     charm;          // ����ֵ
    int     love;           // ����ֵ
} PayAttr;

// ֧����Ʒ
typedef struct tagPayItem
{
    unsigned long long itemID;
    unsigned short count;
} PayItem;

const unsigned int PAY_MAX_PAY_MONEY    = 30000;    // һ��֧��������֧���Ľ��
const unsigned int PAY_MAX_ITEM_COUNT   = 32;       // һ��֧��������֧������Ʒ������
const int PAY_MAX_PAY_INFO_LEN          = 256;      // ֧����Ϣ��󳤶�
const int PAY_MAX_PPKINFO_LEN           = 256;      // ��չ��Ϣ����ʱֻ���ܱ�ǩ���õ���
const int PAY_MAX_SESSION_KEY_LEN       = 256;      // session key��󳤶�

// ֧��������Ϣ
typedef struct tagPayRequest
{
    unsigned char channel;                      // ֧������
    unsigned char itemType;                     // Item���ͣ��ο������Item���Ͷ���
    unsigned int itemCount;                     // Item������
    PayItem items[PAY_MAX_ITEM_COUNT];          // Items
    unsigned int price;                         // �۸�,����֧������ǰ������,������д
    char payInfo[PAY_MAX_PAY_INFO_LEN + 1];     // ֧����Ϣ,���û���ѯ,����淶��д
    unsigned int payUin;                        // ֧��UIN
    unsigned int provideUin;                    // ����UIN
    unsigned long long  providePetID;           // ����PetID
    unsigned char sessionType;                  // Session����
    char sessionKey[PAY_MAX_SESSION_KEY_LEN + 1];
    char clientIP[16];      // �ͻ�IP��ַ
    unsigned char vip;                          // ֧�����ķ���λ 0: �Ƿ��� 1: ����
    char szPPKInfo[PAY_MAX_PPKINFO_LEN + 1];
} PayRequest;

const int PAY_MAX_ANS_DISPLAY_INFO_LEN      = 512;
const int PAY_MAX_ANS_SERIALNO_LEN          = 128;

const int PAY_REQUEST_MIN_LEN = 32;
const int PAY_REQUEST_MAX_LEN = 1200;

// ֧��Ӧ����Ϣ
typedef struct tagPayAns
{
    int retCode;
    char displayInfo[PAY_MAX_ANS_DISPLAY_INFO_LEN + 1];
    char serialNo[PAY_MAX_ANS_SERIALNO_LEN + 1];
    int money;
    int qb;
    int qpoint;

} PayAns;

// Portal�����붨��
const int PAY_PORTAL_OK                 = 0;          //�ɹ�
const int PAY_PORTAL_ERR_PRICE          = -201;       //���۴���
const int PAY_PORTAL_ERR_PROVIDE        = -202;       //��������
const int PAY_PORTAL_ERR_NOT_EXIST      = -203;       //�ʻ�������
const int PAY_PORTAL_ERR_FREEZE         = -204;       //�ʻ�����
const int PAY_PORTAL_ERR_LOSS           = -205;       //�ʻ���ʧ
const int PAY_PORTAL_ERR_BALANCE        = -206;       //����
const int PAY_PORTAL_ERR_PAYLIMIT       = -207;       //��������
const int PAY_PORTAL_ERR_DAYLIMIT       = -208;       //��������
const int PAY_PORTAL_ERR_TOTAL_ENOUGH   = -209;       //QB��QD���㣬�����Ի��֧��
const int PAY_PORTAL_ERR_FRIENDS_7DAY   = -210;       //����7������
const int PAY_PORTAL_ERR_OTHER          = -240;       //��������

// Э��ͷ����
typedef struct tagPayHeader
{
    unsigned short version;
    unsigned short cmd;
    unsigned short payID;
    unsigned short length;
} PayHeader;

const unsigned int PAY_PROTO_HEADER_SIZE         = 8;

// Э�������ֶ���
const unsigned short CMD_PAY_PAYITEM        = 0x0001;
const unsigned short CMD_PAY_PAYITEM_ATTR   = 0x0002;

typedef struct tagPayInnerRequest
{
    unsigned long long providePetID;
    unsigned int price;
    unsigned char itemType;
    unsigned int itemCount;
    PayItem items[PAY_MAX_ITEM_COUNT];
} PayInnerRequest;

}

#endif

