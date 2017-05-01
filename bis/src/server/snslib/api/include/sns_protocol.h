/**
 * @file    sns_protocol.h
 * @brief   ����sns project�еĸ���Э��
 * @author  winuxli
 * @date    2010-04-14
 */

#ifndef SNS_PROTOCOL_H
#define SNS_PROTOCOL_H

namespace snslib
{

const int MAX_KEY_LENGTH = 16;

// ȫ��ҵ�����η���
const unsigned short APPID_PCL                = 1;
const unsigned short APPID_WEBPCL             = 2;
const unsigned short APPID_GROUTE             = 3;
const unsigned short APPID_APPROUTE           = 4;
const unsigned short APPID_QQLIST             = 5;
const unsigned short APPID_QQINFO             = 6;
const unsigned short APPID_FLAG	              = 7;

// ҵ��ID
const unsigned short APPID_APP_TEST           = 100;
const unsigned short APPID_APP_TEST_YY        = 101;
const unsigned short APPID_APP_TNT            = 102;
//const unsigned short APPID_APP_PET_FIGHT      = 103;


typedef struct tagBusHeader
{
    unsigned int uiSrcID;
    unsigned int uiDestID;
    unsigned int uiRouterID;
    unsigned int uiTTL;
    unsigned int uiClientPos;                                  // PCLȷ����Ҷ�Ӧsocket������, ���������粻ͬ�ڵ���ʾһ������������
}BusHeader;

typedef struct tagAppHeader
{
    unsigned int uiLength;						// ���ݳ��ȣ�������appheader
    unsigned int uiUin;							// ���uin
    unsigned int uiIP;							// �û�����IP
    unsigned short ushVersion;					//Э��汾
    unsigned short ushZoneID;					//����ID
    unsigned short ushCmdID;					// ������
    unsigned short ushSrcSvrID;                 			// ԴSVRID
    unsigned short ushDestSvrID;              			// Ŀ��SVRID
    unsigned short ushSrcMID;					// Դģ��ID
    unsigned short ushDestMID;					// Ŀ��ģ��ID,  ֵΪTYPE_MODULE_TRANSACTION������Ӧ��������ʽ����
    unsigned short ushCheckSum;				 // ���ݰ�ͷ+�����У��λ
//    unsigned int uiTransactionID;					// ����id
    //ֻ��һ��TransactionID������������zonesvr֮����������޸�Ϊsrc/dest����. stanleyluo
    unsigned int uiSrcTranID;                   // Դ����ID
    unsigned int uiDestTranID;                  // Ŀ������ID
    unsigned short ushStepID;					// ����id
    int 			iRet;						// ������ֵ
//  ���ֶ�ֻ��PCL��Client����ʱʹ�ã�����SNS�ڲ�����
//    char szKey[MAX_KEY_LENGTH];
}AppHeader;

// ������ṹ����
typedef struct tagTransactionHeader
{
	unsigned int		uiUin;
	unsigned short 		ushType;				// �޸������������
	unsigned short 		ushStepNum;				// �������
	unsigned int 		uiTransactionID;		// ����ID���������߳��Զ�����
	unsigned long long  ullTimerID;				// ��ʱ��ID
	int					iRet;
}TransactionHeader;

typedef struct tagStepHeader
{
	unsigned int 		uiUin;
	unsigned short 		ushID;					// ����ID���Զ����ɣ���1��ʼ
	unsigned short 		ushType;				// ��������
	unsigned short		ushLevel;				// ���󼶱𣬴�1��ʼ�����ܼ�ϣ�ͬһ����ͬʱ����
	unsigned short 		ushAppID;				// Ӧ�� ID
	unsigned short 		ushSvrID;				// ������ ID
	unsigned short 		ushCmd;					// ������
	unsigned int		uiParmLen;				// ���ݳ��ȣ�������header����
	unsigned short 		ushProcessFlag;			// �����־λ
	int					iRet;
}StepHeader;

typedef struct tagSNSQQInfo
{
    unsigned int uiUin;
    char szNickName[16+1];      //QQ�ǳ�
    char cVipFlag;              //�����־λ
    char cCFaceFlag;            //�Զ���ͷ���־λ 0-ϵͳͷ�� 1-�Զ���ͷ��
    unsigned short ushFaceID;   //ϵͳͷ���ID
    unsigned long ulFlag;     //��ͨ��ϵ
    unsigned long ulReserve[3];
}SNSQQInfo;

// ģ�����Ͷ���
const unsigned short TYPE_MODULE_APP			=1;			// appӦ��
const unsigned short TYPE_MODULE_TRANSACTION	=2;			// ����Ӧ��
const unsigned short TYPE_MODULE_LOADER         =3;         //loader
const unsigned short TYPE_MODULE_WRITER         =4;         //writer

// �������Ͷ���
const unsigned short TRANSACTION_TYPE_ADD		= 1;		// �������
const unsigned short TRANSACTION_TYPE_MODIFY	= 2;		// �޸�����Snsapp V2 �Ѿ������ˣ�
const unsigned short TRANSACTION_TYPE_MIDDLE	= 10;		// �м䴦����
const unsigned short TRANSACTION_TYPE_END		= 20;		// �������

// �������Ͷ���
const unsigned short STEP_TYPE_APP				=1;			// Ӧ��
const unsigned short STEP_TYPE_LOGIC			=2;			// �߼�����

// ���輶�����
const unsigned short STEP_LEVEL_AUTO			=1;			// �Զ�������Ĭ��
const unsigned short STEP_LEVEL_HOLD			=2;			// ���ϸ�������ͬ

// �����־λ����
const unsigned short STEP_NOT_PROCESS 			= 0;	// δ����
const unsigned short STEP_SENDED				= 1;	// ��Ϣ�Ѿ�����
const unsigned short STEP_RECVED				= 2;	// ��Ϣ�Ѿ�����

// ������������
const int TRANSACTION_RETURN_OK					= 0;
const int TRANSACTION_RETURN_TIMEOUT			= 1;		// ����ʱ
const int TRANSACTION_RETURN_SYS_ERROR			= 2;		// ϵͳ����

//add by jamieli ��ʱ����ʹ�ã������CmdID��winuxli����
const int CMD_ADD_TIMER_REQ = 0x0101;
const int CMD_ADD_TIMER_RSP = 0x0102;
const int CMD_DEL_TIMER_REQ = 0x0103;
const int CMD_TIMER_INFO = 0x0104;  //��ʱ��������Ϣ
const int CMD_TEST_ECHO = 0x0105;   //����������
const int CMD_TEST_ADDTIMER = 0x0106;   //���Ӷ�ʱ��
const int CMD_TEST_ADD_TRANSACTION = 0x0107;   //�������
const int CMD_TEST_PUTHDB_DATA = 0x0108;    //��HDB��������ݣ�ͬʱ���Ի�д
const int CMD_TEST_GETHDB_DATA = 0x0109;    //��HDB�ж�ȡ����
const int CMD_TEST_OUTHDB_DATA = 0x010a;    //��HDB��ɾ������

const int CMD_TEST_DELTIMER = 0x010b;       //ɾ����ʱ��
const int CMD_BENCH_TEST = 0x010c;          //����ѹ�����Ե�������
const int CMD_TEST_FRIENDLIST = 0x0110;		// ��ȡ�����б�

// LOADER WRITER
const unsigned short CMD_LOAD_DATA = 0x0201;    //��������
const unsigned short CMD_WRITE_DATA = 0x0202;   //��д����
const unsigned short CMD_LAST_WRITE_DATA = 0x0210;	//��д����

// QQ List
const int CMD_GET_QQLIST = 0x0301;
const int CMD_QQLIST_OIDB = 0x0302;
const int CMD_QQLIST_JITONG = 0x0303;
const int CMD_GET_GROUPED_QQLIST = 0x0304;
const int CMD_GROUPED_QQLIST_OIDB = 0x0305;
const int CMD_CHECK_FRIEND = 0x0306;
const int CMD_CHECK_QQLIST_OIDB = 0x0307;
const int CMD_QQLIST_SYNC = 0x0308;




// QQ INFO
const int CMD_GET_QQINFO= 0x0401;
const int CMD_QQINFO_OIDB = 0x0402;
const int CMD_UPDATE_QQINFO = 0x0403;
const int CMD_QQINFO_SYNC = 0x0404;

// QQ FLAG
const unsigned short CMD_GET_FLAG				= 0x0001;			// ��ȡ��־λ
const unsigned short CMD_SET_FLAG				= 0x0002;			// ���ñ�־λ
const unsigned short CMD_DEL_FLAG				= 0x0003;			// ɾ����־λ

// APP Framework ���ݼ��ط���
const unsigned short CMD_DATA_LOAD_INFO = 0x0501;   //�������ݽ����������APP
const unsigned short CMD_FRAMEWORK_SPEED_MONITOR = 0xFFFF; 	// ����ٶ���

// REPLICATION
const unsigned short CMD_REPLICATION_UPDATE = 0x0601;   //���Ƹ���
const unsigned short CMD_REPLICATION_DELETE = 0x0602;   //����ɾ��

// SNSINFO
const unsigned short CMD_SNSINFO_GET = 0x0701;  //��ȡSNSINFO�е���Ϣ
const unsigned short CMD_SNSINFO_PUT = 0x0702;  //����SNSINFO�е���Ϣ
const unsigned short CMD_SNSINFO_OUT = 0x0703;  //ɾ��SNSINFO�е���Ϣ

// LOGCENTRE
const unsigned short CMD_LOGCENTRE_PUT = 0x0801;    //����־���ķ�����־


// PAYSVR
const unsigned short CMD_PAYSVR_REQ = 0x0901;   //PAYSVR��������
const unsigned short CMD_PAYSVR_RSP = 0x0902;   //PAYSVR����Ӧ��

// LOCK SVR
const unsigned short CMD_LOCK_GET				= 0x0A01;			// ��ȡLOCK
const unsigned short CMD_LOCK_SET				= 0x0A02;			// ����LOCK
const unsigned short CMD_LOCK_DEL				= 0x0A03;			// ɾ��LOCK

//ROUTER
const unsigned short CMD_SNSROUTER_SET_REQ	= 0x1001;	//router������ĳuin->svr
const unsigned short CMD_SNSROUTER_SET_RSP	= 0x1002;	//router������ĳuin->svr
const unsigned short CMD_SNSROUTER_DEL_REQ    = 0x1003;		//router��ɾ��ĳuin->svr
const unsigned short CMD_SNSROUTER_DEL_RSP     = 0x1004;		//router��ɾ��ĳuin->svr
const unsigned short CMD_SNSAPP_HEARTBEAT    = 0x1005;        // snsapp->router����

//  PCL�İ�ͷ
#pragma pack(1)

// PCL�����ư�ͷ
typedef struct tagClientHeader{
	unsigned int	uiLength;			// �����ȣ����������ֶ�
	unsigned int	uiUin;				// �û�UIN
	unsigned short	ushVersion;			// Э��汾��
	unsigned int	uiAppID;			// Ӧ��ID
	unsigned short	ushZoneID;			// ����ID
	unsigned short	ushCmdID;			// ������
	unsigned short	ushCheckSum;		// ����У��
}ClientHeader;

// WEBPCL��������������AppHeader
typedef struct tagAppHeader_NetByteOrder
{
    unsigned int   uiLength;					// ���ݳ��ȣ�������appheader
    unsigned int   uiUin;						// UIN
	unsigned int   uiIP;						// IP
    unsigned short ushVersion;
    unsigned short ushZoneID;
    unsigned short ushCmdID;					// ������
    unsigned short ushSrcSvrID;                 // ԴSVRID
    unsigned short ushDestSvrID;                // Ŀ��SVRID
    unsigned short ushSrcMID;					// Դģ��ID
    unsigned short ushDestMID;					// Ŀ��ģ��ID
    unsigned short ushCheckSum;
    unsigned int   uiTransactionID;				// ����id
    unsigned short ushStepID;					// ����id
    int 		   iRet;						// ������ֵ
}AppHeader_NetByteOrder;

#pragma pack()

// PCLִ��ָ��
const unsigned short CMD_PCL_AUTH = 0x0001;
const unsigned short CMD_PCL_TRANSMIT_APP_HEADER = 0x0101;

}

enum
{
    // HTTPPost:
    // HTTPHeader + ProtobufBody
    // TCP:
    // ClientHeader + ProtobufBody
    PCL_BODY_BINARY = 0,
    // HTTPPost:
    // HttpHeader + JsonBody
    // TCP:
    // JsonBody
    PCL_BODY_JSON   = 1,

    // httpget:
    // get /<uri> http/1.0
    PCL_BODY_HTTPGET = 10,
    // httpget:
    // get /crossdomain.xml http/1.0
    // tcp:
    // <policy-file-request/>
    PCL_BODY_POLICY = 11,


    // HTTPGET or HTTPPOST
    PCL_PROTO_HTTP  = 1 << 8,
    // TCP
    PCL_PROTO_TCP   = 3 << 8,
};

enum PlatformType
{
    QQ_PLATFORM = 0,  // QQ/QZ friend list and nickname
    XY_PLATFORM = 1,  // pengyou friend list and pengyou nick
    QG_PLATFORM = 2,  // qqgame friend list and qqgame nick
};


//TT feeds
const int CMD_TTFEEDS = 0x1100; //��ȡTTFeeds



#endif
