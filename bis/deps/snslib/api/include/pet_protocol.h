/*
 * FileName:    pet_protocol.h
 * Author:      borisliu
 * Date:        2009-03-09
 * Description: ����Э�鶨���ļ�
 * History:
 *  <author>        <time>          <desc>
 *  borisliu        2009-03-09      ����
 */

#ifndef PET_PROTOCOL_H
#define PET_PROTOCOL_H

#include <time.h>

namespace snslib
{

const int MAX_PET_PKG_LEN = 8192;
const unsigned short MAX_SESSION_LEN		 = 16;
const unsigned short GAME_HEAD_LENGTH       = 26;

/*
 * �ڲ�ͨ�ŵ�Bus��ͷ����

typedef struct tagBusHeader
{
    unsigned int uiSrcID;           // ԴBusID
    unsigned int uiDestID;          // Ŀ��BusID
    unsigned int uiRouterID;        // ·��BusID��ע�⣬�����ֵΪ0xFFFFFFFF����ʾ�ð���HeartBeat���ݰ�
    unsigned int uiTTL;             // TTL
    unsigned int uiClientPos;       // ClientPos��WebPCL������λ����
} BusHeader;
*/
/*
 * �����ͷ����
 */
typedef struct tagPetHeader
{
    unsigned short ushLength;       // �����ͷ+����ĳ���
    unsigned long long ullPetID;    // ����ID
    unsigned short ushVersion;      // Э��汾��
    unsigned char byLangVer;        // ���԰汾��
    unsigned short ushCmdID;        // Э��������
    unsigned short ushCheckSum;     // �����ͷ+�����У���
} PetHeader;

const int PET_HEADER_LEN = 17;      // �����ͷ���� 17�ֽ�

/*
 * �ڲ�ʹ��Queueͨ�ŵ�Queue��ͷ����
 */
typedef struct tagQueueHeader
{
    unsigned short ushCmdID;        // ������
    time_t tInTime;                 // ��Queueʱ��
    unsigned short ushLength;       // ���峤��
    char achReserved[8];            // �����ֶ�
} QueueHeader;

/*
 * �¼ӵ�С��Ϸ�İ�ͷ
*/
typedef struct tagGameHeader
{
    unsigned short ushCmd;
    unsigned short ushVersion;
    unsigned short ushGameID;
    unsigned int uiSvrID;
    char szSession[MAX_SESSION_LEN + 1];
} GameHeader;
/*
 * �ͻ��˻�web��serverͨ�������ַ�Χ�涨��
 *
 * game_svr     0x0300-0x03FF
 * feed_svr     0x0500-0x05FF
 * trip_svr     0x0600-0x06FF
 * friend_svr   0x0700-0x07FF
 * friend_svr2  0x0900-0x09FF
 * home_svr     0x0A00-0x0AFF
 * goods_svr    0x1000-0x10FF
 * zone_svr     0x1100-0x11FF
 *
 * �ж�һ�������������ĸ����񣬿����� ushCmdID & CMD_TYPE_MASK��Ȼ�����CMD_TYPE���жԱ�
 */

// ��������������
const unsigned short CMD_TYPE_MASK          = 0xFF00;

const unsigned short CMD_TYPE_GAME_SVR      = 0x0332;
const unsigned short CMD_TYPE_FEED_SVR      = 0x0500;
const unsigned short CMD_TYPE_TRIP_SVR      = 0x0600;
const unsigned short CMD_TYPE_FRIEND_SVR    = 0x0700;
const unsigned short CMD_TYPE_FRIEND_SVR2   = 0x0900;
const unsigned short CMD_TYPE_HOME_SVR      = 0x0A00;
const unsigned short CMD_TYPE_GOODS_SVR     = 0x1000;
const unsigned short CMD_TYPE_ZONE_SVR      = 0x1100;
const unsigned short CMD_TYPE_MSG_PROXY_SVR	= 0x1200;
const unsigned short CMD_TYPE_MSG_EVENT     = 0x1300;
const unsigned short CMD_TYPE_MSG_CENTRE    = 0x7700;

const unsigned short CMD_TYPE_REX_GAME_SVR  = 0xFF00;

// msg_proxy�����ֶ���
const unsigned short CMD_MSG_PROXY_SEND_MSG		= 0x1201;	// ����Ϣ

// feed_svr �����ֶ���
const unsigned short CMD_TYPE_FEEDSVR                   = 0x0500;
const unsigned short CMD_FEED_SSO_LOGIN                 = 0x052F; //SSO��½
const unsigned short CMD_FEED_LOGIN                     = 0x0530; //��½
const unsigned short CMD_FEED_LOGOUT                    = 0x0531; //�˳�
const unsigned short CMD_FEED_GET_PETINFO               = 0x0532; //��ȡ�������ԣ��ͻ���ʹ�ã�
const unsigned short CMD_FEED_GET_ZONE_SHOWINFO         = 0x0533; //��ȡ����Zone��ʾ��Ϣ��֧��������ѯ
const unsigned short CMD_FEED_DISCARD                   = 0x0534; //��������
const unsigned short CMD_FEED_ENTER_NORMALSTATUS        = 0x0535; //��ֹ��/ѧϰ
const unsigned short CMD_FEED_HELLO                     = 0x0536; //Hello��
const unsigned short CMD_FEED_SET_AUTOFEEDFLAG          = 0x0537; //�����Զ�ι����־λ
const unsigned short CMD_FEED_FEED                      = 0x0538; //Feed��ι���ӿڣ�
const unsigned short CMD_FEED_GET_AREA_INFO             = 0x0539; //��ȡ������С����Ϣ
const unsigned short CMD_FEED_GET_ZONE_SHOWINFO2        = 0x053A; //��ȡ����Zone��ʾ��Ϣ�汾2�������˳ƺ�ϵͳ
const unsigned short CMD_FEED_SPECIAL_INTERACT          = 0x053B; //����Ľ�����Ϣ�����ڵ������潻����
const unsigned short CMD_FEED_COMM_INTERACT             = 0x053C; //ͨ�ý�����Ϣ���������鶯��Ȧ���Ժ���Ҫʵʱ�������Եĵط���
const unsigned short CMD_FEED_GET_OTHER_PETINFO			= 0x053D; //��ȡ�����������

const unsigned short CMD_FEED_ATTR_CHG_NOTIFY           = 0x0550; //����ֵ�仯֪ͨ��Ϣ
const unsigned short CMD_FEED_STATUS_CHG_NOTIFY         = 0x0551; //״ֵ̬�仯֪ͨ��Ϣ
const unsigned short CMD_FEED_NICKNAME_CHG_NOTIFY       = 0x0552; //���������ǳ�/�����ǳƸı�
const unsigned short CMD_FEED_COMBINEPIC_CHG_NOTIFY     = 0x0553; //���ﱳ���ϳ�ͼƬ�仯֪ͨ
const unsigned short CMD_FEED_URLMSG_NOTIFY             = 0x0554; //��URL�����Ϣ
const unsigned short CMD_FEED_LOGOUT_NOTIFY             = 0x0555; //֪ͨ�˳�
const unsigned short CMD_FEED_UNUSED001                 = 0x0556; //��Ч������
const unsigned short CMD_FEED_CLIENTCONFXML_CHG_NOTIFY  = 0x0557; //�ͻ�������XML�ļ��汾����
const unsigned short CMD_FEED_POPPIC_CHG_NOTIFY         = 0x0558; //��������װ��ID�仯
const unsigned short CMD_FEED_AVATAR_CHG_NOTIFY         = 0x0559; //����Avatar��װ��汾�仯
const unsigned short CMD_FEED_VEHICLE_CHG_NOTIFY        = 0x055A; //��ͨ���߱��֪ͨ
const unsigned short CMD_FEED_VIPFLAG_CHG_NOTIFY        = 0x055B; //�����־λ�仯֪ͨ
const unsigned short CMD_FEED_PET_TIPS_NOTIFY           = 0x055C; //�·��������TIPS��Ϣ
const unsigned short CMD_FEED_AUTOFEEDFLAG_CHG_NOTIFY   = 0x055D; //�Զ�ι��״̬���֪ͨ
const unsigned short CMD_FEED_BUTTON_CHG_NOTIFY         = 0x055E; //ͼ����ʾ���֪ͨ
const unsigned short CMD_FEED_MISC_INFO_NOTIFY          = 0x055F; //��½ʱһЩ������Ϣ����־λ��֪
const unsigned short CMD_FEED_NEW_FEEDS_NOTIFY          = 0x0560; //��½ʱ�·�����SNS�Ĵ����Feeds����
const unsigned short CMD_FEED_CHECK_HELLO_NOTIFY        = 0x0561; //����Ҫ��ͻ����Ϸ�Hello��
const unsigned short CMD_FEED_CLIENT_LOG                = 0x0562; //���Ϳͻ�����־

const unsigned short CMD_FEED_WORK                      = 0x0570; //��ʼ��
const unsigned short CMD_FEED_STUDY                     = 0x0571; //��ʼѧϰ
const unsigned short CMD_FEED_ENTER_STATUS              = 0x0572; //��ʼѧϰ
const unsigned short CMD_FEED_WEBGET_PETINFO            = 0x0573; //��ȡ�������ԣ�WEBʹ�ã�
const unsigned short CMD_FEED_UPDATEPET_ATTR            = 0x0574; //�޸ĳ�������
const unsigned short CMD_FEED_WEBFEED                   = 0x0575; //ͨ��Webι��
const unsigned short CMD_FEED_UPDATE_SCHOLARSHIP        = 0x0576; //���½�ѧ����ȡ��־λ
const unsigned short CMD_FEED_UPDATE_COMMUNITYINFO      = 0x0577; //���������洢�ֶ�
const unsigned short CMD_FEED_GET_AVATARINFO            = 0x0578; //��ȡAvatarװ�䷽��
const unsigned short CMD_FEED_UPDATE_AVATARINFO         = 0x0579; //����Avatarװ�䷽��
const unsigned short CMD_FEED_GET_STUDYINFO             = 0x057A; //��ȡ��ѧ�γ���Ϣ
const unsigned short CMD_FEED_UPDATEPET_SPEC_ATTR       = 0x057B; //�޸ĳ����һЩ�������ԣ����ڲ��Բ��������κ�ҵ��ģ��
const unsigned short CMD_FEED_ENTER_TRAVEL              = 0x057C; //��������״̬
const unsigned short CMD_FEED_INNER_GET_PETINFO_REQ     = 0x057D; //��ȡ��������-���󣨲�ͬ����֮���ѯ��
const unsigned short CMD_FEED_INNER_GET_PETINFO_RSP     = 0x057E; //��ȡ��������-Ӧ�𣨲�ͬ����֮���ѯ��
const unsigned short CMD_FEED_FROZEN_ACCOUNT            = 0x057F; //���ᡢ�ⶳ�ʻ�
const unsigned short CMD_FEED_UPDATE_STUDYINFO          = 0x0580; //����ѧϰ�Ŀγ���Ϣ
const unsigned short CMD_FEED_WEBHELLO                  = 0x0581; //Web���Hello��
const unsigned short CMD_FEED_WEBLOGIN                  = 0x0582; //Web���Login��
const unsigned short CMD_FEED_UPDATE_PERMISSION         = 0x0583; //���³���Ȩ��
const unsigned short CMD_FEED_LOADING_REPORT            = 0x0584; //�ͻ��˼��ط�ʽ�ϱ�


// trip_svr�����ֶ���
const unsigned short CMD_TRIP_END                       = 0x0602; //��������


// goods_svr�����ֶ���


// zone_svr �����ֶ���
const unsigned short CMD_ZONE_TIME_TICK         = 0x1101;   // ��ʱ
const unsigned short CMD_ZONE_LOGIN             = 0x1102;   // ��¼С��
const unsigned short CMD_ZONE_CHANGE_SCENE      = 0x1103;   // �л�����
const unsigned short CMD_ZONE_MOVE              = 0x1104;   // �����ƶ�
const unsigned short CMD_ZONE_CHAT              = 0x1105;   // ����
const unsigned short CMD_ZONE_EMOTION           = 0x1106;   // ����
const unsigned short CMD_ZONE_SCENE_PET_LIST    = 0x1107;   // ��ȡ�������г���
const unsigned short CMD_ZONE_LOGOUT            = 0x1108;   // �˳�С��
const unsigned short CMD_ZONE_GET_POSITION      = 0x1109;   // ��ȡС�����ﵱǰλ��
const unsigned short CMD_ZONE_JUMP_SCENE        = 0x110A;   // ֱ������һ������
const unsigned short CMD_ZONE_SEND_SYS_MSG      = 0x110B;   // ����ϵͳ��Ϣ
const unsigned short CMD_ZONE_XOGAME_CHANGESCENE_MSG = 0x110C;  // XO Game�����л���Ϣ
const unsigned short CMD_ZONE_XOGAME_PLAYER_CONTINUE_MSG      = 0x110D;   // ���������Ϣ
const unsigned short CMD_ZONE_XOGAME_QUESTION_MSG      = 0x110E;   // XO Game������Ϣ
const unsigned short CMD_ZONE_XOGAME_RESULT_MSG      = 0x110F;   // XO Game ��������Ϣ
const unsigned short CMD_ZONE_XOGAME_FAILEDLIST_MSG      = 0x1110;   // XO Game ��������Ϣ
const unsigned short CMD_ZONE_XOGAME_GAME_READY_MSG = 0x1111; //XO Game׼����ʼ��Ϣ
const unsigned short CMD_ZONE_XOGAME_RIGHTLIST_MSG      = 0x1112;   // XO Game ��������Ϣ
const unsigned short CMD_ZONE_SKILL_CAST_CONFIG_MSG = 0x1113;    //����Ͷ������
const unsigned short CMD_ZONE_SKILL_CAST_MSG = 0x1114;       //�ͷ�Ͷ������
const unsigned short CMD_ZONE_SKILL_CAST_RESPONSE_MSG = 0x1115; //�ͷ�Ͷ������Ӧ��
const unsigned short CMD_ZONE_SKILL_CAST_HITLIST_MSG = 0x1116;  //Ͷ�����������б�
const unsigned short CMD_ZONE_SKILL_CHANGE_CONFIG_MSG = 0x1117;
const unsigned short CMD_ZONE_SKILL_CHANGE_MSG = 0x1118;
const unsigned short CMD_ZONE_SKILL_CHANGE_STATUS_BC = 0x1119;

const unsigned short CMD_ZONE_BC_PATH           = 0x1120;   // ·���㲥
const unsigned short CMD_ZONE_BC_VIEW           = 0x1121;   // ��ͼ�仯�㲥
const unsigned short CMD_ZONE_BC_CHAT           = 0x1122;   // ������Ϣ�㲥
const unsigned short CMD_ZONE_BC_EMOTION        = 0x1123;   // ����㲥
const unsigned short CMD_ZONE_BC_AREA_PETS      = 0x1124;   // С��������Ϣ�㲥
const unsigned short CMD_ZONE_BC_INFO_VER       = 0x1125;   // ������Ϣ�汾�仯�㲥
const unsigned short CMD_ZONE_BC_ACTION_PLAY    = 0x1126;   // �㲥���Ŷ�����Ϣ

const unsigned short CMD_ZONE_SET_CHAT_DENY_TIME    = 0x1130;   // ���ý���ʱ��

const unsigned short CMD_ZONE_XOGAME2_CHANGESCENE_MSG = CMD_ZONE_XOGAME_CHANGESCENE_MSG;
const unsigned short CMD_ZONE_XOGAME2_PAYOPENGAME = 0x1141;
const unsigned short CMD_ZONE_XOGAME2_CONTINUE_MSG = CMD_ZONE_XOGAME_PLAYER_CONTINUE_MSG;
const unsigned short CMD_ZONE_XOGAME2_BOXLIST_MSG = 0x1143;
const unsigned short CMD_ZONE_XOGAME2_OPENBOX_MSG = 0x1144;
const unsigned short CMD_ZONE_XOGAME2_BOXOPENED_MSG = 0x1145;
const unsigned short CMD_ZONE_XOGAME2_FINISHED_MSG = 0x1146;
const unsigned short CMD_ZONE_XOGAME2_GAME_READY_MSG = 0x1147;


// homesvr �����ֶ���
const unsigned short CMD_HOME_GET_HOME_CONFIG           = 0x0A01;
const unsigned short CMD_HOME_POST_POSITION             = 0x0A03;
const unsigned short CMD_HOME_POST_MESSAGE              = 0x0A04;
const unsigned short CMD_HOME_VISIT_HOME                = 0x0A05;
const unsigned short CMD_HOME_INVITE_REQ                = 0x0A06;
const unsigned short CMD_HOME_INVITE_RESP               = 0x0A07;
const unsigned short CMD_HOME_LEAVE                     = 0x0A08;
const unsigned short CMD_HOME_EVICT                     = 0x0A0A;
const unsigned short CMD_HOME_POST_EMOTION              = 0x0A0C;

const unsigned short CMD_HOME_WEB_GET_HOME_INFO         = 0x0A11;
const unsigned short CMD_HOME_WEB_HOLD_PARTY            = 0x0A12;
const unsigned short CMD_HOME_WEB_CHANGE_HOME           = 0x0A13;

const unsigned short CMD_HOME_NOTIFY_HOME_INFO          = 0x0A21;
const unsigned short CMD_HOME_NOTIFY_POSITION_INFO      = 0x0A22;
const unsigned short CMD_HOME_NOTIFY_TALK_MSG           = 0x0A23;
const unsigned short CMD_HOME_NOTIFY_PARTY              = 0x0A24;
const unsigned short CMD_HOME_NOTIFY_GUEST_IN           = 0x0A25;
const unsigned short CMD_HOME_NOTIFY_EMOTION            = 0x0A26;

const unsigned short CMD_HOME_GET_HOME_INFO_2           = 0x0A31;
const unsigned short CMD_HOME_GET_HOME_INFO_3           = 0x0A32;
const unsigned short CMD_HOME_VISIT_HOME_2              = 0x0A33;
const unsigned short CMD_HOME_VISIT_HOME_3              = 0x0A34;
const unsigned short CMD_HOME_VISIT_HOME_4              = 0x0A35;
const unsigned short CMD_HOME_INVITE_RESP_2             = 0x0A36;
const unsigned short CMD_HOME_LEAVE_2                   = 0x0A37;
const unsigned short CMD_HOME_NOTIFY_LEAVE              = 0x0A38;
const unsigned short CMD_HOME_EVICT_2                   = 0x0A39;

const unsigned short CMD_HOME_NOTIFY_LOGIN              = 0x0A51;
const unsigned short CMD_HOME_NOTIFY_LOGOUT             = 0x0A52;


/*
 * �ڲ�Queueͨ�������ַ�Χ�涨��
 *
 * feed_svr         0x0100-0x01FF
 * goods_svr        0x0200-0x02FF
 * pet_loader       0x0300-0x03FF
 * pet_lazywriter   0x0400-0x04FF
 * state_svr        0x0500-0x05FF
 * zone_svr         0x0600-0x06FF
 * msg_proxy        0x0700-0x07FF
 */

const unsigned short QUEUE_CMD_TYPE_FEED_SVR        = 0x0100;
const unsigned short QUEUE_CMD_TYPE_GOODS_SVR       = 0x0200;
const unsigned short QUEUE_CMD_TYPE_PET_LOADER      = 0x0300;
const unsigned short QUEUE_CMD_TYPE_PET_LAZYWRITER  = 0x0400;
const unsigned short QUEUE_CMD_TYPE_STATE_SVR       = 0x0500;
const unsigned short QUEUE_CMD_TYPE_ZONE_SVR        = 0x0600;
const unsigned short QUEUE_CMD_TYPE_MSG_PROXY		= 0x0700;
const unsigned short QUEUE_CMD_TYPE_STAT_MAP_ROUTER = 0x0800;
const unsigned short QUEUE_CMD_TYPE_STAT_MAP_INFO   = 0x0900;
const unsigned short QUEUE_CMD_TYPE_DIR             = 0x0A00;

// feed_svr cmd define
const unsigned short QUEUE_CMD_FEED_MSG	               = 0x0101;    //��ͨ�Ŀͻ������ݰ�
const unsigned short QUEUE_CMD_FEED_LOAD_INFO          = 0x0102;    //��½ʱ������LOAD��Ϸ��ص����ݰ�
const unsigned short QUEUE_CMD_FEED_LOAD_RETURN_INFO   = 0x0103;    //��ѯ������Ϣʱ������LOAD��Ϸ��ص����ݰ�
const unsigned short QUEUE_CMD_FEED_LOGOUT_IMMEDIATE   = 0x0104;    //��ʱ�˳������
const unsigned short QUEUE_CMD_FEED_AUTOFEED_CHECK     = 0x0105;    //����Զ�ι���Ƿ���Կ�ͨ��Ӧ��

// zone_svr cmd define
const unsigned short QUEUE_CMD_ZONE_MSG	                = 0x0601;
const unsigned short QUEUE_CMD_ZONE_LOGOUT_NOTIFY       = 0x0602;
const unsigned short QUEUE_CMD_ZONE_ATTR_CHG_NOTIFY     = 0x0603;
const unsigned short QUEUE_CMD_ZONE_SYSTEM_MSG          = 0x0604;
const unsigned short QUEUE_CMD_ZONE_USEGOODS_NOTIFY     = 0x0605;

// goodsvr cmd define
const unsigned short QUEUE_CMD_GOODS_MSG	    = 0x0201;

// loader cmd define
const unsigned short QUEUE_CMD_LOADER_LOAD_INFO		    = 0x0301; // load��session����ȥ�������ڶ�Ӧ��queue�з����Ƿ�ɹ�
const unsigned short QUEUE_CMD_LOADER_LOAD_RETURN_INFO   = 0x0302; // ��load��session����ȥ��ֱ�ӽ��õ��ĳ�����Ϣ(��������Ʒ��Ϣ)���ص���Ӧ��queue

//lazy writer cmd define
const unsigned short QUEUE_CMD_WRITER_WRITE_IMMEDIATELY = 0x0401; // �������ٶȽ���������ݻ�д�����ݿ�����ȥ��ͨ����������ַ��͵��û�����Ҫ���������ߵ�

// statesvr cmd define
const unsigned short QUEUE_CMD_STATESVR_TIMER_ADDNODE           = 0x0501;
const unsigned short QUEUE_CMD_STATESVR_TIMER_USER_LOGOUT		= 0x0502;
const unsigned short QUEUE_CMD_STATESVR_TIMER_USER_LOGIN        = 0x0503;
const unsigned short QUEUE_CMD_STATESVR_TIMER_DELNODE           = 0x0504;
const unsigned short QUEUE_CMD_STATESVR_TIMER_STATUS_BEGIN		= 0x0510;
const unsigned short QUEUE_CMD_STATESVR_TIMER_STATUS_STOP		= 0x0511;
const unsigned short QUEUE_CMD_STATESVR_TIMER_MSG				= 0x0520;
const unsigned short QUEUE_CMD_STATESVR_TIMER_DELETE_REQ		= 0x0550;

// msgproxy cmd define
const unsigned short QUEUE_CMD_MSG_PROXY_MSG    = 0x0701;

// stat_map_clt_cmd define
const unsigned short QUEUE_CMD_STAT_MAP_ROUTER  = 0x0801;
const unsigned short QUEUE_CMD_STAT_MAP_INFO    = 0x0802;

// Send Svr queue Э��
const unsigned short QUEUE_CMD_SEND_EVENT       = 0x0901;       // �����¼�
const unsigned short QUEUE_CMD_SEND_WL_NOTIFY   = 0x0902;       // ���͸����߳������أ��ó������߻�����
const unsigned short QUEUE_CMD_SEND_SET_TITLE   = 0x0903;       // ���óƺ�
const unsigned short QUEUE_CMD_SEND_NOTIFY_PETLOGIN = 0x0904;   // ���ͳ����½�ɹ���֪ͨ
const unsigned short QUEUE_CMD_SEND_XOGAME_RANK = 0x0905; //����XOGame Rank
const unsigned short QUEUE_CMD_SEND_STOP_TRAVEL  = 0x0906; //���ͽ�������״̬֪ͨ

// dir�ϱ�
const unsigned short QUEUE_CMD_DIR_ZONE_INFO_SUBMIT = 0x0A01; // ������Ϣ�ϱ�
const unsigned short QUEUE_CMD_DIR_AREA_INFO_SUBMIT = 0x0A02; // С����Ϣ�ϱ�
const unsigned short QUEUE_CMD_DIR_SUBMIT_TIMER = 0x0A03; // ֪ͨdir��ʱ��ͳ��ƽ̨�ϱ�����С����Ϣ

// ���ı��� queue cmd ����
const unsigned short QUEUE_CMD_AUTOFEED_FEED    = 0x0B01;       // ����֪ͨ
const unsigned short QUEUE_CMD_AUTOFEED_STUDY   = 0x0B02;       // ѧϰ����֪ͨ
const unsigned short QUEUE_CMD_AUTOFEED_CHECK   = 0x0B03;       // ����Ƿ���Կ������ı���
const unsigned short QUEUE_CMD_AUTOFEED_STATUS  = 0x0B04;		// ״̬�������
const unsigned short QUEUE_CMD_AUTOFEED_WORK	= 0x0B05;		// �򹤽���֪ͨ
const unsigned short QUEUE_CMD_AUTOFEED_TRAVEL	= 0x0B06;		// ���ν���֪ͨ
const unsigned short QUEUE_CMD_AUTOFEED_LOGIN	= 0x0B07;		// �����¼֪ͨ
const unsigned short QUEUE_CMD_AUTOFEED_FLAG	= 0x0B08;		// �Զ�ι����Ǳ��

// ��԰ϵͳ queue cmd ����
const unsigned short QUEUE_CMD_HOME_LOAD_HOME_INFO  = 0x0C01;       // ���ؼ�԰��Ϣ
const unsigned short QUEUE_CMD_HOME_GET_RANDOM_PET  = 0x0C02;       // ��ȡ�������
const unsigned short QUEUE_CMD_HOME_HOLD_PARTY      = 0x0C03;       // �ٿ��ɶ�

// ����Ԥ����ѯ�ӿͻ��˴������������֣��Լ����ظ��ͻ��˵�������
const unsigned short CMD_WEATHER_QUERY = 0x1501; // ��ѯ�������

// �¼�ϵͳЭ��
const unsigned short CMD_EVENT_PASSPORT			=0x1301;   		// �¼���֤֪ͨ
const unsigned short CMD_EVENT_CHECK_PASSPORT	=0x1302;		// check��֤��Ϣ
const unsigned short CMD_EVENT_SET_OPTION		=0x1303;		// �¼�������

//���ϵͳsvr
const unsigned short CMD_MARRY_SVR              =0x1400;
const unsigned short CMD_MARRY_PROPOSE          =0x1401; // ��麯��
const unsigned short CMD_MARRY_QUERY_DIVORCE    =0x1402; // ��ѯ�����¼��������
const unsigned short CMD_MARRY_PROCESS_PROPOSE  =0x1403; // ������鱻����¼��������
const unsigned short CMD_MARRY_DIVORCE          =0x1404; // �����������
const unsigned short CMD_MARRY_GET_MANIFESTO    =0x1405; // ��ȡ���������������
const unsigned short CMD_MARRY_MARRYINFO        =0x1406; // ��ȡ�������ݿ���Ϣ��������
const unsigned short CMD_MARRY_LAYEGG           =0x1407; // ��������������
const unsigned short CMD_MARRY_QUERY_EGG        =0x1408; // ��ѯ���ﵰ����������
const unsigned short CMD_MARRY_PROCESS_EGG      =0x1409; // ������ﵰ����������
const unsigned short CMD_MARRY_QUERY_MAIL       =0x1410; // ��ѯ�����ʼ�����������
const unsigned short CMD_MARRY_PROCESS_MAIL     =0x1411; // ���������ʼ�����������
const unsigned short CMD_MARRY_ADD_LOVE         =0x1412; // �Ӱ���ֵ����������
const unsigned short CMD_MARRY_GET_GIFT         =0x1413; // ȡ������������

//ħ��������,����0x0300Э��
const unsigned short CMD_BALL_PROTO             =0x0332;

//��Ϣ����ר��������
const unsigned short CMD_MSG_CENTRE             =0x7700;
const unsigned short CMD_MSG_CENTRE_INFO        =0x7701;    //ת������Ϣ���ĵ���Ϣ
const unsigned short CMD_MSG_CENTRE_ROUTE       =0x7702;    //ת����Router����Ϣ

}

#endif

