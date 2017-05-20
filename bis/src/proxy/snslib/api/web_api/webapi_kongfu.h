/*
 * webapi_kongfu.h
 *
 *  Created on: 2010-9-16
 *      Author: jiffychen
 */

#ifndef WEBAPI_KONGFU_H_
#define WEBAPI_KONGFU_H_

#include "webapi_base.h"
#include "api/proto/replay.pb.h"
#include "api/proto/feeds.pb.h"

#include <vector>
#include<string>

namespace snslib
{
typedef struct tagKongfuBasicInfo
{
	// ����id
	unsigned long long ullPetID;
	// ����ȼ�
	unsigned uiLevel;
	// ����ɳ�ֵ
	unsigned uiGrowth;
	// ����Ԫ��
	unsigned uiYuanBao;
	// ����ֵ
	unsigned uiSavvy;
	// ��
	unsigned uiCurMP;
	// ��
	unsigned uiCurVIT;
	// �Ƿ񱻷�ţ���0��ʾ����ţ����ֶα�ʾ���ԭ��
	unsigned uiFrozen;
	// ���ʱ�䣨��1970/1/1��ʼ��������
	unsigned uiFrozenTo;
	// ����HP
	unsigned uiBaseHP;
	// ����MP
	unsigned uiBaseMP;
	// ����������
	unsigned uiBaseATK;
	// ����������
	unsigned uiBaseDEF;
	// �����ٶ�
	unsigned uiBaseATI;
	// ��������
	unsigned uiBaseMAG;
	// ��Ϸ��ɫ����ʱ��
	unsigned uiCreateTime;
	// �ϴε�¼��Ϸ��ʱ��
	unsigned uiLastLogin;
} KongfuBasicInfo;

typedef struct tagGoodsItem
{
    unsigned uiItemID;
    // ��ʾ���ߵ�������> 0 ��ʾ���ӣ�< 0 ��ʾ����
    int iItemCount;
} GoodsItem;

typedef struct tagSkillInfo
{
	// ����ID
	unsigned uiSkillID;
	// �����µȼ���Ϊ0��ʾ���ܽ���ɾ��
	int iLevel;
} SKillInfo;

typedef struct tagKongfuAttr
{
	// ����hp���ֵ����
	int iBaseHP;
	// ����mp���ֵ����
	int iBaseMP;
	// ����������ֵ����
	int iBaseATK;
	// ����������ֵ����
	int iBaseDEF;
	// �����ٶ�ֵ����
	int iBaseATI;
	// ��������ֵ����
	int iBaseMAG;
	// ����ֵ����
	int iCurVIT;
	// ħ��ֵ����
	int iCurMP;
} KongfuAttr;

typedef struct tagTypedFightSkill
{
	std::vector<unsigned> vActiveSkills;
	std::vector<unsigned> vPassiveSkills;
} TypedFightSkill;

typedef struct tagFightSkill
{
	TypedFightSkill stAttack;
	TypedFightSkill stDefense;
} FightSkill;

typedef struct tagPlayerInfo
{
	KongfuBasicInfo stBasicInfo;
	std::vector<snslib::GoodsItem> vGoodsItems;
	std::vector<snslib::SKillInfo> vSkillInfos;
	FightSkill stFightSkills;
} PlayerInfo;

class CWebApiKongfu : public CWebApiBase
{
public:
	CWebApiKongfu();
	~CWebApiKongfu();

	// ��ӻ���ٵ���
	int ModifyGoods(unsigned uiUin, std::vector<snslib::GoodsItem>& vItems, const char* pszDesc);

	// ���, ���ʱ��Ϊ�ӵ�ǰʱ��+iFrozeHours(iFrozenHours < 0 ���Խ��)
	int Froze(unsigned uiUin, unsigned uiFrozeType, int iFrozenHours);

	// ���
	int UnFroze(unsigned uiUin);

	// �����򽵼�����
	int ModifySkills(unsigned uiUin, std::vector<snslib::SKillInfo>& vSkills);

	// �޸�����
	int ModifyAttr(unsigned uiUin, snslib::KongfuAttr& stAttr);

	// Ԫ��ֻ�ܼӣ����ܼ���Ҫ����ֱ�Ӽ�����
	int AddYuanBao(unsigned uiUin, unsigned uiYuanBaoAdd, const char* pszDesc);

	// ����ֵ���Լӻ����isavvyaddΪ����ʾ������ֵ
	int AddSavvy(unsigned uiUin, unsigned iSavvyAdd, const char* pszDesc);

	// �ɳ�ֵֻ�ܼӣ����ܼ���Ҫ����ֱ�Ӽ�����
	int AddGrowth(unsigned uiUin, unsigned uiGrowthAdd, const char* pszDesc);

	// ��ȡ��ɫ��Ϣ
	int GetUserInfo(unsigned uiUin, PlayerInfo& stPlayerInfo);

	//2011-06-08 ����¼��ӿ�  ghostzheng

	//��ȡ¼���б�
	int GetReplayList( unsigned int uiUin/*�û�id*/ , kongfupet::GetReplayListResponse& stReplayList/*���ص�¼���б�*/);

	//��ȡ¼����ϸ��Ϣ
	int GetReplay(unsigned int uiUin/*�û�id*/,unsigned int request_id/*¼��id*/, unsigned int fight_time /*��ʼ��ʱ��*/ , kongfupet::GetReplayResponse& szReplay/*���ص�¼��*/);

	//��ȡ����Ϊtype ��feeds�б�
	int GetFeedsList( unsigned int uiUin/*�û�id*/ ,  kongfupet::KongfuFeedsData::KongfuFeedType type,kongfupet::GetFeedsListResponse& stFeedsList/*���ص�����Ϊtype ��feed�б�*/);

	//��ȡ�ֶ�2 Feeds + ¼���б�
	int GetWebFeeds(unsigned int uiUin/*�û�id*/ ,kongfupet::WebFeeds& stWebFeeds/*���ص�feeds*/);

};
}

#endif /* WEBAPI_KONGFU_H_ */

	

