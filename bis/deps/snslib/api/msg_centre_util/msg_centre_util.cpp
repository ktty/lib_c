#include "api/msg_centre_util/msg_centre_util.h"
using namespace snslib;

//��̬��Ա��ʼ��
int CMCUtil::m_iInitFlag = 0;
std::map<unsigned short, MsgCentreMetaData> CMCUtil::m_mMsgCentreMetaData;

MsgCentreMetaData g_stMsgCentreMetaData[]=
{
    {"M_PetName",     MCID_MODIFY_PETNAME,       MC_TYPE_STR,        16},
    {"M_QQName",      MCID_MODIFY_QQNAME,        MC_TYPE_STR,        16},
    {"M_VipFlag",     MCID_MODIFY_VIPFLAG,       MC_TYPE_BYTE,       1},
    {"M_PetLevel",    MCID_MODIFY_PETLEVEL,      MC_TYPE_SHORT,      2},
    {"M_Permission",  MCID_MODIFY_PERMISSION,    MC_TYPE_INT,        4},
    {"M_TitleID",     MCID_MODIFY_TITLEID,       MC_TYPE_SHORT,      2},
    {"M_PetStatus",   MCID_MODIFY_PETSTATUS,     MC_TYPE_SHORT,      2},
    {"M_AvatarVer",   MCID_MODIFY_AVATARVER,     MC_TYPE_SHORT,      2},
    {"M_PetHealth",   MCID_MODIFY_PETHEALTH,     MC_TYPE_SHORT,      2},
    {"M_AvatarInfo",  MCID_MODIFY_AVATARINFO,    MC_TYPE_BIN,        40},
    {"M_Lovingness",  MCID_MODIFY_LOVINGNESS,    MC_TYPE_INT,        4},
    {"M_ZoneID",      MCID_MODIFY_ZONEID,        MC_TYPE_SHORT,      2},
    {"M_AreaID",      MCID_MODIFY_AREAID,        MC_TYPE_SHORT,      2},
    {"M_YuanBao",     MCID_MODIFY_YUANBAO,       MC_TYPE_INT,        4},
    {"M_Growth",      MCID_MODIFY_GROWTH,        MC_TYPE_INT,        4},
    {"M_OnlineTime",  MCID_MODIFY_ONLINETIME,    MC_TYPE_INT,        4},
	{"M_VipLevel",    MCID_MODIFY_VIPLEVEL,      MC_TYPE_SHORT,      2},
	{"M_VipYearFlag", MCID_MODIFY_VIPYEARFLAG,   MC_TYPE_BYTE,       1},

    {"S_PetName",     MCID_SYNC_PETNAME,       MC_TYPE_STR,        16},
    {"S_QQName",      MCID_SYNC_QQNAME,        MC_TYPE_STR,        16},
    {"S_VipFlag",     MCID_SYNC_VIPFLAG,       MC_TYPE_BYTE,       1},
    {"S_PetLevel",    MCID_SYNC_PETLEVEL,      MC_TYPE_SHORT,      2},
    {"S_Permission",  MCID_SYNC_PERMISSION,    MC_TYPE_INT,        4},
    {"S_TitleID",     MCID_SYNC_TITLEID,       MC_TYPE_SHORT,      2},
    {"S_PetStatus",   MCID_SYNC_PETSTATUS,     MC_TYPE_SHORT,      2},
    {"S_AvatarVer",   MCID_SYNC_AVATARVER,     MC_TYPE_SHORT,      2},
    {"S_VipLevel",    MCID_SYNC_VIPLEVEL,      MC_TYPE_SHORT,      2},

    {"PetLoadNode", MCID_PETLOADNODE,   MC_TYPE_INT,        4},
    {"PetDelNode",  MCID_PETDELNODE,    MC_TYPE_INT,        4},
    {"PetRegist",   MCID_PETREGIST,     MC_TYPE_NOR,        0},
    {"PetDiscard",  MCID_PETDISCARD,    MC_TYPE_NOR,        0},
    {"PetLogin",    MCID_PETLOGIN,      MC_TYPE_NOR,        0},
    {"PetLogout",   MCID_PETLOGOUT,     MC_TYPE_NOR,        0},

    //����ϵͳ��ӵ���Ϣ
    {"FishGet",   MCID_FISH_GET,     MC_TYPE_BIN,         8},
    {"FarmGet",   MCID_FARM_GET,     MC_TYPE_BIN,         8},
	{"NpcTalk",   MCID_NPC_TALK,     MC_TYPE_BIN,		  8},
    {"GameSucc",  MCID_NPC_GAMESUCC, MC_TYPE_BIN,         8},

};

void CMCUtil::Init()
{
    if (m_iInitFlag == 0)
    {
        m_mMsgCentreMetaData.clear();
        int iMCMetaDataNum = sizeof(g_stMsgCentreMetaData)/sizeof(MsgCentreMetaData);
        for(int i=0; i<iMCMetaDataNum; i++)
        {
            m_mMsgCentreMetaData.insert(std::pair<unsigned short, MsgCentreMetaData>(g_stMsgCentreMetaData[i].ushFieldID, g_stMsgCentreMetaData[i]));
        }

        m_iInitFlag = 1;
    }
}
//�ڲ����ݽṹ��ʽ��
// �����ʽ��[20Byte:BusHeader][20Byte:PetHeader][2Byte:MsgCentrePkgNum][MsgCentrePkg_1][MsgCentrePkg_2]����[MsgCentrePkg_n]
// ������Ϣ��[4Byte:MagicNum][2Byte:PkgType][2Byte:PkgLen][PkgLen Byte:PkgVal]
//�����������������PetHeader�����������ݵĴ���ͽ������

/**
 * @brief PackMCPkg ������Ķ��MsgCentreMsg�����ָ����Buff��
 * @param pvBuffer �ⲿ����Ĵ洢�ռ�ָ��
 * @param piPackLen[IN/OUT] �ⲿ����Ĵ洢�ռ䳤�ȣ����ʱ����������ó��ȣ�����ʧ���˳������������Buff�еĳ���
 * @param vstMsgCentreMsg ��Ҫ�������Ϣ����
 * @return 0-�ɹ� ����-ʧ��
 */
int CMCUtil::PackMCPkg(void* pvBuffer, int *piPackLen, std::vector<MsgCentreMsg> vstMsgCentreMsg)
{
    Init();
    int iOffSet = 0;
    int iMsgCentreMsgNum = 0;

    if ((pvBuffer == NULL)||(*piPackLen < 2)||vstMsgCentreMsg.size() == 0)
    {
        //�����������
        return -1;
    }

    iMsgCentreMsgNum = vstMsgCentreMsg.size();

    iOffSet += CBuffTool::WriteShort((char *)pvBuffer + iOffSet, (unsigned short)iMsgCentreMsgNum);

    for(int i=0; i<iMsgCentreMsgNum; i++)
    {
        int iMsgReqLen = 8;     //��Ϣ��䵽Buff������Ҫ�ĳ���
        int iBuffRemainLen = *piPackLen - iOffSet;  //Buffʣ��Ŀռ�

        std::map<unsigned short, MsgCentreMetaData>::iterator pstMsgCentreMetaData = m_mMsgCentreMetaData.find(vstMsgCentreMsg[i].ushMsgType);
        if (pstMsgCentreMetaData == m_mMsgCentreMetaData.end())
        {
            //û���ҵ���Ӧ����Ϣ����
            return -2;
        }

        iMsgReqLen += pstMsgCentreMetaData->second.ushFieldMaxLen;

        if (iBuffRemainLen < iMsgReqLen)
        {
            //BUFF���Ȳ���
            return -3;
        }

        iOffSet += CBuffTool::WriteInt((char *)pvBuffer + iOffSet, MC_MSG_MAGIC_NUM);
        iOffSet += CBuffTool::WriteShort((char *)pvBuffer + iOffSet, vstMsgCentreMsg[i].ushMsgType);

        if ((((pstMsgCentreMetaData->second.ushFieldType == MC_TYPE_STR)||(pstMsgCentreMetaData->second.ushFieldType == MC_TYPE_BIN))&&(vstMsgCentreMsg[i].ushMsgValLen > pstMsgCentreMetaData->second.ushFieldMaxLen))
                ||(((pstMsgCentreMetaData->second.ushFieldType != MC_TYPE_STR)&&((pstMsgCentreMetaData->second.ushFieldType != MC_TYPE_BIN))) && (vstMsgCentreMsg[i].ushMsgValLen != pstMsgCentreMetaData->second.ushFieldMaxLen)))
        {
            //�ֶγ���������
            return -4;
        }

        iOffSet += CBuffTool::WriteShort((char *)pvBuffer + iOffSet, vstMsgCentreMsg[i].ushMsgValLen);
        switch(pstMsgCentreMetaData->second.ushFieldType)
        {
            case MC_TYPE_BYTE:
            case MC_TYPE_STR:
            case MC_TYPE_LL:
            case MC_TYPE_BIN:
            {
                //����Ҫת��������ֱ��дBUFF
                iOffSet += CBuffTool::WriteString((char *)pvBuffer + iOffSet, vstMsgCentreMsg[i].szMsgVal, vstMsgCentreMsg[i].ushMsgValLen);
                break;
            }
            case MC_TYPE_SHORT:
            {
                //��Ҫת��������
                iOffSet += CBuffTool::WriteShort((char *)pvBuffer + iOffSet, *((short *)vstMsgCentreMsg[i].szMsgVal));
                break;
            }

            case MC_TYPE_INT:
            {
                //��Ҫת��������
                iOffSet += CBuffTool::WriteInt((char *)pvBuffer + iOffSet, *((int *)vstMsgCentreMsg[i].szMsgVal));
                break;
            }
            case MC_TYPE_NOR:
            {
                break;
            }
            default:
            {
                //�ֶ�����������
                return -5;
                break;
            }
        }

    }

    *piPackLen = iOffSet;

    return 0;
}

/**
 * @brief UnpackMCPkg ��Buff�е����ݽ���������vstMsgCentreMsg��
 * @param pvBuffer �ⲿ����Ĵ洢�ռ�ָ��
 * @param piUnpackLen[IN/OUT] ��Ҫ����ĳ��ȣ�����ʵ�ʽ���ĳ���
 * @param vstMsgCentreMsg ����Ժ����Ϣ
 * @return 0-�ɹ� ����-ʧ��
 */
int CMCUtil::UnpackMCPkg(void* pvBuffer, int *piUnpackLen, std::vector<MsgCentreMsg> &vstMsgCentreMsg)
{
    Init();

    int iOffSet = 0;

    short shMsgCentreMsgNum = 0;

    int iMCMagicNum = 0;
    MsgCentreMsg stMsgCentreMsg;

    if ((pvBuffer == NULL)||(*piUnpackLen < 2))
    {
        //�����������
        return -1;
    }

    iOffSet += CBuffTool::ReadShort((char *)pvBuffer + iOffSet, shMsgCentreMsgNum);

    if ((shMsgCentreMsgNum < 0)||(shMsgCentreMsgNum > 100))
    {
        return -2;
    }

    vstMsgCentreMsg.clear();

    for(int i=0; i<shMsgCentreMsgNum; i++)
    {
        int iMsgReqLen = 8; //��һ����Ϣ��Ҫ�ĳ���
        int iBuffRemainLen = *piUnpackLen - iOffSet;    //ʣ��δ�ⲿ�ֵĳ���

        if (iBuffRemainLen < iMsgReqLen)
        {
            //ʣ�೤�Ȳ�����
            return -3;
        }

        memset(&stMsgCentreMsg, 0x0, sizeof(stMsgCentreMsg));

        iOffSet += CBuffTool::ReadInt((char *)pvBuffer + iOffSet, iMCMagicNum);
        if (iMCMagicNum != MC_MSG_MAGIC_NUM)
        {
            //MAGICNUM����ȷ
            return -4;
        }

        iOffSet += CBuffTool::ReadShort((char *)pvBuffer + iOffSet, stMsgCentreMsg.ushMsgType);
        iOffSet += CBuffTool::ReadShort((char *)pvBuffer + iOffSet, stMsgCentreMsg.ushMsgValLen);

        std::map<unsigned short, MsgCentreMetaData>::iterator pstMsgCentreMetaData = m_mMsgCentreMetaData.find(stMsgCentreMsg.ushMsgType);
        if (pstMsgCentreMetaData == m_mMsgCentreMetaData.end())
        {
            //û���ҵ���Ӧ����Ϣ����
            return -5;
        }

        if ((((pstMsgCentreMetaData->second.ushFieldType == MC_TYPE_STR)||(pstMsgCentreMetaData->second.ushFieldType == MC_TYPE_BIN))&&(stMsgCentreMsg.ushMsgValLen > pstMsgCentreMetaData->second.ushFieldMaxLen))
            ||(((pstMsgCentreMetaData->second.ushFieldType != MC_TYPE_STR)&&(pstMsgCentreMetaData->second.ushFieldType != MC_TYPE_BIN)) && (stMsgCentreMsg.ushMsgValLen != pstMsgCentreMetaData->second.ushFieldMaxLen)))
        {
            //�ֶγ���������
            return -6;
        }

        iMsgReqLen += stMsgCentreMsg.ushMsgValLen;
        if (iBuffRemainLen < iMsgReqLen)
        {
            //ʣ�೤�Ȳ�����
            return -7;
        }

        switch(pstMsgCentreMetaData->second.ushFieldType)
        {
            case MC_TYPE_BYTE:
            case MC_TYPE_STR:
            case MC_TYPE_BIN:
            case MC_TYPE_LL:
            {
                //����Ҫת��������ֱ��дBUFF
                iOffSet += CBuffTool::ReadString((char *)pvBuffer + iOffSet, stMsgCentreMsg.szMsgVal, stMsgCentreMsg.ushMsgValLen);
                break;
            }
            case MC_TYPE_SHORT:
            {
                //��Ҫת��������
                unsigned short ushVal;
                iOffSet += CBuffTool::ReadShort((char *)pvBuffer + iOffSet, ushVal);
                memcpy(stMsgCentreMsg.szMsgVal, &ushVal, sizeof(ushVal));
                break;
            }

            case MC_TYPE_INT:
            {
                //��Ҫת��������
                unsigned int uiVal;
                iOffSet += CBuffTool::ReadInt((char *)pvBuffer + iOffSet, uiVal);
                memcpy(stMsgCentreMsg.szMsgVal, &uiVal, sizeof(uiVal));
                break;
            }
            case MC_TYPE_NOR:
            {
                break;
            }
            default:
            {
                //�ֶ�����������
                return -8;
                break;
            }
        }

        vstMsgCentreMsg.push_back(stMsgCentreMsg);

    }

    *piUnpackLen = iOffSet;

    return 0;
}

const char *CMCUtil::FormatMCPkg(std::vector<MsgCentreMsg> &vstMsgCentreMsg)
{
    Init();

    static char szMCPkgOutputStr[10240];

    int iOffSet = 0;
    std::vector<MsgCentreMsg>::iterator pstMCPkg = vstMsgCentreMsg.begin();

    for (pstMCPkg = vstMsgCentreMsg.begin();pstMCPkg != vstMsgCentreMsg.end();pstMCPkg++)
    {
        std::map<unsigned short, MsgCentreMetaData>::iterator pstMsgCentreMetaData = m_mMsgCentreMetaData.find(pstMCPkg->ushMsgType);
        if (pstMsgCentreMetaData == m_mMsgCentreMetaData.end())
        {
            //û���ҵ���Ӧ����Ϣ����
            iOffSet+=snprintf(szMCPkgOutputStr+iOffSet, sizeof(szMCPkgOutputStr)-iOffSet, "[%d|UNKNOW_MSG]", pstMCPkg->ushMsgType);
            continue;
        }

        switch(pstMsgCentreMetaData->second.ushFieldType)
        {
            case MC_TYPE_NOR:
            {
                iOffSet+=snprintf(szMCPkgOutputStr+iOffSet, sizeof(szMCPkgOutputStr)-iOffSet, "[%s|%d|%d|]", pstMsgCentreMetaData->second.szFieldName, pstMCPkg->ushMsgType, pstMCPkg->ushMsgValLen);
                break;
            }
            case MC_TYPE_BYTE:
            {
                iOffSet+=snprintf(szMCPkgOutputStr+iOffSet, sizeof(szMCPkgOutputStr)-iOffSet, "[%s|%d|%d|%d]", pstMsgCentreMetaData->second.szFieldName, pstMCPkg->ushMsgType, pstMCPkg->ushMsgValLen, *((unsigned char *)pstMCPkg->szMsgVal));
                break;
            }
            case MC_TYPE_SHORT:
            {
                iOffSet+=snprintf(szMCPkgOutputStr+iOffSet, sizeof(szMCPkgOutputStr)-iOffSet, "[%s|%d|%d|%d]", pstMsgCentreMetaData->second.szFieldName, pstMCPkg->ushMsgType, pstMCPkg->ushMsgValLen, *((unsigned short *)pstMCPkg->szMsgVal));
                break;
            }
            case MC_TYPE_INT:
            {
                iOffSet+=snprintf(szMCPkgOutputStr+iOffSet, sizeof(szMCPkgOutputStr)-iOffSet, "[%s|%d|%d|%d]", pstMsgCentreMetaData->second.szFieldName, pstMCPkg->ushMsgType, pstMCPkg->ushMsgValLen, *((unsigned int *)pstMCPkg->szMsgVal));
                break;
            }
            case MC_TYPE_LL:
            {
                iOffSet+=snprintf(szMCPkgOutputStr+iOffSet, sizeof(szMCPkgOutputStr)-iOffSet, "[%s|%d|%d|%llu]", pstMsgCentreMetaData->second.szFieldName, pstMCPkg->ushMsgType, pstMCPkg->ushMsgValLen, *((unsigned long long *)pstMCPkg->szMsgVal));
                break;
            }
            case MC_TYPE_STR:
            {
                iOffSet+=snprintf(szMCPkgOutputStr+iOffSet, sizeof(szMCPkgOutputStr)-iOffSet, "[%s|%d|%d|%s]", pstMsgCentreMetaData->second.szFieldName, pstMCPkg->ushMsgType, pstMCPkg->ushMsgValLen, pstMCPkg->szMsgVal);
                break;
            }
            case MC_TYPE_BIN:
            {
                iOffSet+=snprintf(szMCPkgOutputStr+iOffSet, sizeof(szMCPkgOutputStr)-iOffSet, "[%s|%d|%d|%s]", pstMsgCentreMetaData->second.szFieldName, pstMCPkg->ushMsgType, pstMCPkg->ushMsgValLen, CStrTool::Str2Hex(pstMCPkg->szMsgVal, pstMCPkg->ushMsgValLen));
                break;
            }

            default:
            {
                iOffSet+=snprintf(szMCPkgOutputStr+iOffSet, sizeof(szMCPkgOutputStr)-iOffSet, "[%d|%d|UNKNOW_FIELD]", pstMCPkg->ushMsgType, pstMsgCentreMetaData->second.ushFieldType);
                break;
            }
        }
    }

    return szMCPkgOutputStr;
}

