#ifndef _MSG_CENTRE_PROTOCOL_H_
#define _MSG_CENTRE_PROTOCOL_H_

#include <map>
#include <vector>

#include "comm/util/pet_util.h"

namespace snslib
{
const int MC_MSG_MAX_VALLEN = 64;
const int MC_MSG_MAGIC_NUM = 0x4D534743;    //MSGC

//���ڽӿڼ䴫�ݵ���Ϣ
typedef struct tagMsgCentreMsg
{
    unsigned short ushMsgType;
    unsigned short ushMsgValLen;
    char szMsgVal[MC_MSG_MAX_VALLEN];   //���ֶ�����Ϊ����������Ǽ��������ͣ�����ͨ��ָ��ָ��ͷ��ֱ��ʹ��
}MsgCentreMsg;

enum
{
    //�������Ա仯
    MCID_MODIFY_PETNAME			= 1,	//�����ǳ�
    MCID_MODIFY_QQNAME			= 2,	//QQ�ǳ�
    MCID_MODIFY_VIPFLAG			= 3,	//�����־λ
    MCID_MODIFY_PETLEVEL		= 4,	//����ȼ�
    MCID_MODIFY_PERMISSION		= 5,	//����Ȩ��
    MCID_MODIFY_TITLEID			= 6,	//���ﵱǰ�ƺ�
    MCID_MODIFY_PETSTATUS		= 7,	//���ﵱǰ״̬
    MCID_MODIFY_AVATARVER		= 8,	//�����Avatar�汾
    MCID_MODIFY_PETHEALTH		= 9,	//����Ľ�������
    MCID_MODIFY_AVATARINFO		= 10,	//�����Avatarװ�緽��
    MCID_MODIFY_LOVINGNESS		= 11,	//���ﰮ��ֵ
    MCID_MODIFY_ZONEID			= 12,	//�޸Ĵ���ID
    MCID_MODIFY_AREAID			= 13,	//�޸�С��ID
    MCID_MODIFY_YUANBAO			= 14,	//�޸��û�Ԫ��
    MCID_MODIFY_GROWTH			= 15,	//�޸��û��ɳ�ֵ
    MCID_MODIFY_ONLINETIME		= 16,	//�޸��û�����ʱ��
	MCID_MODIFY_VIPLEVEL		= 17,	//VIP�ȼ��仯
	MCID_MODIFY_VIPYEARFLAG		= 18,	//VIP��ѱ�Ǳ仯

    //��������ͬ��
    MCID_SYNC_PETNAME			= 501,  //�����ǳ�
    MCID_SYNC_QQNAME			= 502,	//QQ�ǳ�
    MCID_SYNC_VIPFLAG			= 503,	//�����־λ
    MCID_SYNC_PETLEVEL			= 504,	//����ȼ�
    MCID_SYNC_PERMISSION		= 505,	//����Ȩ��
    MCID_SYNC_TITLEID			= 506,	//���ﵱǰ�ƺ�
    MCID_SYNC_PETSTATUS			= 507,  //���ﵱǰ״̬
    MCID_SYNC_AVATARVER			= 508,	//�����Avatar�汾
    MCID_SYNC_VIPLEVEL          = 509,  //VIP�ȼ�

    //��Ҫ���͵�ÿ̨Router�ļ��ء�ɾ����������Ϣ
    MCID_PETLOADNODE			= 1001,	//����ڵ���SESSION�м��أ�ע�⣬����Ϣ����ʹ��PetHeader������0x7702
    MCID_PETDELNODE				= 1002,	//����ڵ���SESSION��ɾ����ע�⣬����Ϣ����ʹ��PetHeader������0x7702
    MCID_PETREGIST				= 1003,	//����ע��
    MCID_PETDISCARD				= 1004,	//��������
    MCID_PETLOGIN				= 1005,	//�����½  LOGINʱ��Ҫ�����г������Է��ͳ�ȥ
    MCID_PETLOGOUT				= 1006,	//�����˳�

    //����ϵͳ�ĸ���ID
    MCID_FISH_GET               = 2001, //�����ջ��� VAL:[�������:4Byte][���������:4Byte]
    MCID_FARM_GET               = 2002, //ũ�������� VAL:[���������:4Byte][������ĸ���:4Byte]
    MCID_NPC_TALK               = 2003, //���NPC�Ի� VAL:[SCeneID:2Byte][NPCID:2Byte][NPC�Ի�����]
    MCID_NPC_GAMESUCC           = 2004, //���С��ϷVAL:[GAMEID:4Byte][GAMELEVEL:4Byte]
};

};

#endif
