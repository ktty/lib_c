/*
 * webapi_kongfu.cpp
 *
 *  Created on: 2010-9-16
 *      Author: jiffychen
 */


#include "api/proto/kongfu_cache.pb.h"
#include "webapi_kongfu.h"
#include "../../../server/kongfu_pet_new/kongfu_pet_protocol.h"


using namespace snslib;
using namespace std;
using namespace kongfupet;

//const int CMD_PET_FIGHT_APP_GET_USER = 0x1013;
//const int CMD_PET_FIGHT_APP_UPDATE_USER = 0x1014;

CWebApiKongfu::CWebApiKongfu() {}
CWebApiKongfu::~CWebApiKongfu() {}

int CWebApiKongfu::ModifyGoods(unsigned uiUin, std::vector<snslib::GoodsItem>& vItems, const char* pszDesc)
{
	UpdateUserRequest objRequest;
	objRequest.set_uin(uiUin);
	objRequest.mutable_user_info()->set_uin(uiUin);

	for (size_t i = 0; i < vItems.size(); i++)
	{
		BagItem* pobjBagItem = objRequest.mutable_user_info()->add_bag_items();
		pobjBagItem->set_id(vItems[i].uiItemID);
		pobjBagItem->set_count(vItems[i].iItemCount);
	}

	UpdateUserResponse objResponse;
    int iRetVal = SendAndRecv(uiUin, CMD_PET_FIGHT_APP_UPDATE_USER, 0, 0, objRequest, objResponse);
    if (iRetVal != 0)
    {
        return -2;
    }

    return objResponse.ecode() > 0 ? 0 - objResponse.ecode() : objResponse.ecode();
}

int CWebApiKongfu::Froze(unsigned uiUin, unsigned uiFrozeType, int iFrozenHours)
{
	UpdateUserRequest objRequest;
	objRequest.set_uin(uiUin);
	objRequest.mutable_user_info()->set_uin(uiUin);
	objRequest.mutable_user_info()->mutable_state_info()->set_frozen_reason(uiFrozeType);
	objRequest.mutable_user_info()->mutable_state_info()->set_unfrozen_time(iFrozenHours*60*60);

	UpdateUserResponse objResponse;
	int iRetVal = SendAndRecv(uiUin, CMD_PET_FIGHT_APP_UPDATE_USER, 0, 0, objRequest, objResponse);
    if (iRetVal != 0)
    {
        return -2;
    }

    return objResponse.ecode() > 0 ? 0 - objResponse.ecode() : objResponse.ecode();
}

int CWebApiKongfu::UnFroze(unsigned uiUin)
{
	UpdateUserRequest objRequest;
	objRequest.set_uin(uiUin);
	objRequest.mutable_user_info()->set_uin(uiUin);
	objRequest.mutable_user_info()->mutable_state_info()->set_frozen_reason(0);

	UpdateUserResponse objResponse;
	int iRetVal = SendAndRecv(uiUin, CMD_PET_FIGHT_APP_UPDATE_USER, 0, 0, objRequest, objResponse);
    if (iRetVal != 0)
    {
        return -2;
    }

    return objResponse.ecode() > 0 ? 0 - objResponse.ecode() : objResponse.ecode();
}

// �����򽵼�����
int CWebApiKongfu::ModifySkills(unsigned uiUin, std::vector<snslib::SKillInfo>& vSkills)
{
	UpdateUserRequest objRequest;
	objRequest.set_uin(uiUin);
	objRequest.mutable_user_info()->set_uin(uiUin);

	for (size_t i = 0; i < vSkills.size(); i++)
	{
		SkillItem* pobjSkill = objRequest.mutable_user_info()->add_skills();
		pobjSkill->set_id(vSkills[i].uiSkillID);
		pobjSkill->set_level(vSkills[i].iLevel);
	}

	UpdateUserResponse objResponse;
	int iRetVal = SendAndRecv(uiUin, CMD_PET_FIGHT_APP_UPDATE_USER, 0, 0, objRequest, objResponse);
    if (iRetVal != 0)
    {
        return -2;
    }

    return objResponse.ecode() > 0 ? 0 - objResponse.ecode() : objResponse.ecode();
}

// �޸�����
int CWebApiKongfu::ModifyAttr(unsigned uiUin, snslib::KongfuAttr& stAttr)
{
	UpdateUserRequest objRequest;
	objRequest.set_uin(uiUin);
	objRequest.mutable_user_info()->set_uin(uiUin);

	if (stAttr.iBaseHP)
		objRequest.mutable_user_info()->mutable_basic()->set_hpm(stAttr.iBaseHP);
	if (stAttr.iBaseMP)
		objRequest.mutable_user_info()->mutable_basic()->set_mpm(stAttr.iBaseMP);
	if (stAttr.iBaseATK)
		objRequest.mutable_user_info()->mutable_basic()->set_atk(stAttr.iBaseATK);
	if (stAttr.iBaseDEF)
		objRequest.mutable_user_info()->mutable_basic()->set_def(stAttr.iBaseDEF);
	if (stAttr.iBaseATI)
		objRequest.mutable_user_info()->mutable_basic()->set_ati(stAttr.iBaseATI);
	if (stAttr.iBaseMAG)
		objRequest.mutable_user_info()->mutable_basic()->set_mag(stAttr.iBaseMAG);
	if (stAttr.iCurVIT)
		objRequest.mutable_user_info()->mutable_basic()->set_vit(stAttr.iCurVIT);
	if (stAttr.iCurMP)
		objRequest.mutable_user_info()->mutable_basic()->set_mp(stAttr.iCurMP);

	UpdateUserResponse objResponse;
	int iRetVal = SendAndRecv(uiUin, CMD_PET_FIGHT_APP_UPDATE_USER, 0, 0, objRequest, objResponse);
    if (iRetVal != 0)
    {
        return -2;
    }

    return objResponse.ecode() > 0 ? 0 - objResponse.ecode() : objResponse.ecode();
}

int CWebApiKongfu::AddYuanBao(unsigned uiUin, unsigned uiYuanBaoAdd, const char* pszDesc)
{
	UpdateUserRequest objRequest;
	objRequest.set_uin(uiUin);
	objRequest.mutable_user_info()->set_uin(uiUin);

	objRequest.mutable_user_info()->mutable_pet_info()->set_yuanbao_unsync(uiYuanBaoAdd);

	UpdateUserResponse objResponse;
	int iRetVal = SendAndRecv(uiUin, CMD_PET_FIGHT_APP_UPDATE_USER, 0, 0, objRequest, objResponse);
    if (iRetVal != 0)
    {
        return -2;
    }

    return objResponse.ecode() > 0 ? 0 - objResponse.ecode() : objResponse.ecode();
}

int CWebApiKongfu::AddSavvy(unsigned uiUin, unsigned iSavvyAdd, const char* pszDesc)
{
	UpdateUserRequest objRequest;
	objRequest.set_uin(uiUin);
	objRequest.mutable_user_info()->set_uin(uiUin);

	objRequest.mutable_user_info()->mutable_pet_info()->set_savvy_remain(iSavvyAdd);

	UpdateUserResponse objResponse;
	int iRetVal = SendAndRecv(uiUin, CMD_PET_FIGHT_APP_UPDATE_USER, 0, 0, objRequest, objResponse);
    if (iRetVal != 0)
    {
        return -2;
    }

    return objResponse.ecode() > 0 ? 0 - objResponse.ecode() : objResponse.ecode();
}

int CWebApiKongfu::AddGrowth(unsigned uiUin, unsigned uiGrowthAdd, const char* pszDesc)
{
	UpdateUserRequest objRequest;
	objRequest.set_uin(uiUin);
	objRequest.mutable_user_info()->set_uin(uiUin);

	objRequest.mutable_user_info()->mutable_pet_info()->set_growth_unsync(uiGrowthAdd);

	UpdateUserResponse objResponse;
	int iRetVal = SendAndRecv(uiUin, CMD_PET_FIGHT_APP_UPDATE_USER, 0, 0, objRequest, objResponse);
    if (iRetVal != 0)
    {
        return -2;
    }

    return objResponse.ecode() > 0 ? 0 - objResponse.ecode() : objResponse.ecode();
}

int CWebApiKongfu::GetUserInfo(unsigned uiUin, PlayerInfo& stPlayerInfo)
{
	GetUserRequest objRequest;
	objRequest.set_uin(uiUin);

	GetUserResponse objResponse;
    int iRetVal = SendAndRecv(uiUin, CMD_PET_FIGHT_APP_GET_USER, 0, 0, objRequest, objResponse);
    if (iRetVal != 0)
    {
        return -2;
    }

    if (objResponse.ecode())
    {
    	return objResponse.ecode() > 0 ? 0 - objResponse.ecode() : objResponse.ecode();
    }

    const DBUserInfo& objUserInfo = objResponse.user_info();
    // convert from db user info to player info
    stPlayerInfo.stBasicInfo.ullPetID = objUserInfo.pet_info().petid();
    stPlayerInfo.stBasicInfo.uiLevel = objUserInfo.pet_info().petlevel();
    stPlayerInfo.stBasicInfo.uiGrowth = objUserInfo.pet_info().growth_pet() + objUserInfo.pet_info().growth_unsync();
    stPlayerInfo.stBasicInfo.uiYuanBao = objUserInfo.pet_info().yuanbao_pet() + objUserInfo.pet_info().yuanbao_unsync();
    stPlayerInfo.stBasicInfo.uiSavvy = objUserInfo.pet_info().savvy_remain();
    stPlayerInfo.stBasicInfo.uiCurMP = objUserInfo.basic().mp();
    stPlayerInfo.stBasicInfo.uiCurVIT = objUserInfo.basic().vit();
    stPlayerInfo.stBasicInfo.uiFrozen = objUserInfo.state_info().frozen_reason();
    stPlayerInfo.stBasicInfo.uiFrozenTo = objUserInfo.state_info().unfrozen_time();
    stPlayerInfo.stBasicInfo.uiBaseHP = objUserInfo.basic().hpm();
    stPlayerInfo.stBasicInfo.uiBaseMP = objUserInfo.basic().mpm();
    stPlayerInfo.stBasicInfo.uiBaseATK = objUserInfo.basic().atk();
    stPlayerInfo.stBasicInfo.uiBaseDEF = objUserInfo.basic().def();
    stPlayerInfo.stBasicInfo.uiBaseATI = objUserInfo.basic().ati();
    stPlayerInfo.stBasicInfo.uiBaseMAG = objUserInfo.basic().mag();
    stPlayerInfo.stBasicInfo.uiCreateTime = objUserInfo.basic().create_time();
    stPlayerInfo.stBasicInfo.uiLastLogin = objUserInfo.state_info().last_login();

    for (int i = 0; i < objUserInfo.bag_items_size(); i++)
    {
    	GoodsItem stItem;
    	stItem.uiItemID = objUserInfo.bag_items(i).id();
    	stItem.iItemCount = objUserInfo.bag_items(i).count();
    	stPlayerInfo.vGoodsItems.push_back(stItem);
    }

    for (int i = 0; i < objUserInfo.skills_size(); i++)
    {
    	SKillInfo stSkill;
    	stSkill.uiSkillID = objUserInfo.skills(i).id();
    	stSkill.iLevel = objUserInfo.skills(i).level();
    	stPlayerInfo.vSkillInfos.push_back(stSkill);
    }

    for (int i = 0; i < objUserInfo.selected().attack().active_size(); i++)
    {
    	if (objUserInfo.selected().attack().active(i))
    	{
    		stPlayerInfo.stFightSkills.stAttack.vActiveSkills.push_back(objUserInfo.selected().attack().active(i));
    	}
    }
    for (int i = 0; i < objUserInfo.selected().attack().passive_size(); i++)
    {
    	if (objUserInfo.selected().attack().passive(i))
    	{
    		stPlayerInfo.stFightSkills.stAttack.vPassiveSkills.push_back(objUserInfo.selected().attack().passive(i));
    	}
    }
    for (int i = 0; i < objUserInfo.selected().not_used_defense().active_size(); i++)
    {
    	if (objUserInfo.selected().not_used_defense().active(i))
    	{
    		stPlayerInfo.stFightSkills.stDefense.vActiveSkills.push_back(objUserInfo.selected().not_used_defense().active(i));
    	}
    }
    for (int i = 0; i < objUserInfo.selected().not_used_defense().passive_size(); i++)
    {
    	if (objUserInfo.selected().not_used_defense().passive(i))
    	{
    		stPlayerInfo.stFightSkills.stDefense.vPassiveSkills.push_back(objUserInfo.selected().not_used_defense().passive(i));
    	}
    }

    return 0;
}


//2011-06-09 ���� ¼���ȡ��web_api�ӿ�   ������������ by ghostzheng

const int REPLAY_APP_ID = 103;
const int REPLAY_SVR_ID = 4; //�������ļ� kongfu_replay.ini

/***********************************************************
  GetReplayList �� ��ȡ¼���б�
  ������
		uiUin ���û�id��Q�ţ�
		stReplayList�� �洢�������ص�¼���б�
  ����ֵ��
		0����ȷ
		����������
************************************************************/
int CWebApiKongfu::GetReplayList( unsigned int uiUin/*�û�id*/ , kongfupet::GetReplayListResponse& stReplayList)
{
	kongfupet::GetReplayListRequest  objRequest;
	objRequest.set_uin(uiUin);
	//start ��num ���Բ���䣬kongfu_replayû���õ�
	//objRequest.set_start(start);
	//objRequest.set_count(num);

	int iRetVal = SendAndRecv(uiUin, CMD_PET_FIGHT_GET_REPLAY_LIST, REPLAY_APP_ID, REPLAY_SVR_ID, objRequest, stReplayList);
	
	if (iRetVal != 0)
		return -1;
	if(0 == stReplayList.result())
		return -2;

	return 0;
}

/***********************************************************
  GetReplay �� ��ȡĳ��¼��
  ������
		uiUin ���û�id��Q�ţ�
		replay_id �� ¼��id
		fight_time�� ��ʱ��
		szReplay���洢�������ص�¼��
  ����ֵ��
		0����ȷ
		����������
  ˵����
	  replay_id��fight_time���Դ�¼���б��л�ȡ(����GetReplayList)
************************************************************/
int CWebApiKongfu::GetReplay(unsigned int uiUin,unsigned int replay_id, unsigned int fight_time ,kongfupet::GetReplayResponse& szReplay)
{	

	kongfupet::GetReplayRequest  objRequest;
	objRequest.set_uin(uiUin);
	objRequest.set_replay_id(replay_id);
	objRequest.set_fight_time(fight_time);

	int iRetVal = SendAndRecv(uiUin, CMD_PET_FIGHT_GET_REPLAY, REPLAY_APP_ID, REPLAY_SVR_ID, objRequest, szReplay);
	
	if (iRetVal != 0)
		return -1;
	if(0 == szReplay.result())//�˴�0Ϊ���� 
		return -2;

	return 0;
}

/***********************************************************
  GetFeedsList �� /��ȡ����Ϊtype ��feeds�б�
  ������
		uiUin ���û�id��Q�ţ�
		type �� feeds���ͣ���3�� 0���У�1��������2һ��
		stFeedsList���洢�������ص�feeds�б�
  ����ֵ��
		0����ȷ
		����������
************************************************************/
int CWebApiKongfu::GetFeedsList(unsigned int uiUin/*�û�id*/ , kongfupet::KongfuFeedsData::KongfuFeedType type,kongfupet::GetFeedsListResponse& stFeedsList)
{
	kongfupet::GetFeedsListRequest  objRequest;
	objRequest.set_uin(uiUin);
	//start ��num ���Բ���䣬KongFuPetFeedsû���õ�
	//objRequest.set_start(start);
	//objRequest.set_count(num);
	objRequest.set_type(type);  

	 const int FEEDS_APP_ID = 103;
	 const int FEEDS_SVR_ID = 3;

	int iRetVal = SendAndRecv(uiUin, CMD_PET_FIGHT_GET_FEEDS_LIST, FEEDS_APP_ID, FEEDS_SVR_ID, objRequest, stFeedsList);
	if (iRetVal != 0)
		return -1;
	if(0 != stFeedsList.result())//ע�� �� �˴���0Ϊ����
		return -2;

	return 0;
}


/***********************************************************
  GetFeedsList �� ��ȡ�ֶ�2 Feeds������Ϊһ�㣩 + ¼���б� 
  ������
		uiUin ���û�id��Q�ţ�
		stWebFeeds���洢�������ص�feeds
  ����ֵ��
		0����ȷ
		����������
************************************************************/
int CWebApiKongfu::GetWebFeeds(unsigned int uiUin/*�û�id*/ ,kongfupet::WebFeeds& stWebFeeds/*���ص�¼���б�*/)
{
	kongfupet::GetReplayListResponse stReplayList;
	kongfupet::GetFeedsListResponse stFeedsList;
	
	if( 0 !=  GetReplayList(uiUin , stReplayList))
		return -1;
	if( 0 != GetFeedsList(uiUin, kongfupet::KongfuFeedsData::NORMALFEED,stFeedsList))
		return -2;

	stWebFeeds.mutable_replay_list()->Clear();
	stWebFeeds.mutable_replay_list()->MergeFrom(stReplayList.replays());
	stWebFeeds.mutable_feeds()->Clear();
	stWebFeeds.mutable_feeds()->MergeFrom(stFeedsList.feeds());

   return 0;
}



