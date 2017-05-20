/*
 * webapi_xboy.h
 *
 *  Created on: 2010-12-8
 *      Author: smiyang
 */

#ifndef WEBAPI_XBOY_H_
#define WEBAPI_XBOY_H_

#include <string>
#include <sstream>
#include "webapi_base.h"

namespace snslib
{
typedef struct tagMoneyUnit
{
    int iMoneyType;       //��ҵ��ͺ�
    int iMoneyCount;      //��ҵ�����
} MoneyUnit;

typedef struct tagBaseInfo
{
    unsigned int uiUin;   //QQ
    int exp;              //����ֵ
    int lev;              //�ȼ�
    int ryz;              //����ֵ
    int hzs;              //����ʯ
} BaseInfo;

typedef struct tagTeamInfo
{
    unsigned int uiUin;   //QQ
    std::string name;     //ս������
    int super_teamer_size;//�߼�̽Ա����
} TeamInfo;

typedef struct tagGoodsUnit
{
    int iGoodsID;          //����ID
    int iGoodsCount;       //��ʾ���ߵ�������> 0 ��ʾ���ӣ�< 0 ��ʾ����
} GoodsUnit;

typedef struct tagHonorTitleUnit
{
    int iHonorID;          //����ѫ�µ�ID
    long lTime;            //��ø�ѫ�µ�ʱ��
} HonorTitleUnit;

typedef struct tagFightUnit
{
    unsigned int iPlayerID;
    int iWonCount;
    int iLostCount;
    long lLastTime;
} FightUnit;

typedef struct tagRandomUnit
{
    unsigned int iPlayerID;
    int iPlayerLev;
} RandomUnit;

typedef struct tagSimpleInfoUnit
{
    unsigned int iPlayerID;
    int iLev;
    int iSize;
} SimpleInfo;

typedef struct tagBaseAttrUnit
{
    int iCurLife;
    int iCurEnergy;
    int iCurEndurance;
    int iCurRyz;
    int iCurExp;
    int iCurHzs;
} BaseAttrUnit;

typedef struct tagTaskUnit
{
    int task_id;
    int cur_step;
    int cur_state;
    int series_task_id;
    int series_task_level;
} TaskUnit;

typedef struct tagSeriesTaskUnit
{
    int series_task_id;
    int series_task_level;
} SeriesTaskUnit;

typedef struct tagDzdUnit
{
    unsigned int iPlayerID;
    long lCreateTime;
} DzdUnit;

typedef struct tagBossUnit
{
    int boss_id;
    int win_count;
    int lost_count;
    int last_won_time;
    int fighting;
    int cur_life;
    int last_ft_time;
} BossUnit;

class CWebApiXBoy : public CWebApiBase
{
public:
    CWebApiXBoy();
    ~CWebApiXBoy();

    /********************************ҵ������Ҫ���API�ӿ�********************************/
    //���, ���ʱ��Ϊ�ӵ�ǰʱ��+iFrozeHours(iFrozenHours����Ϊ������)
    //uiFrozeTypeΪ���ԭ��Ϊ������
    int XBoyFroze(const unsigned uiUin, const unsigned uiFrozeType, const int iFrozenHours);

    //���
    int XBoyUnFroze(const unsigned uiUin);

    //��ӡ��ҵ�Ǯ��
    int XBoyPrintPlayerMoneyBag(unsigned uiUin, std::vector<snslib::MoneyUnit>& vItems);

    //����������ҵ�Ǯ��
    int XBoyModifyPlayerMoneyBag(unsigned uiUin, std::vector<snslib::MoneyUnit>& vItems);

    //��ӡ��ҵĻ�����Ϣ��QQ������ֵ���ȼ�������ֵ������ʯ
    int XBoyPrintPlayerBaseInfo(unsigned uiUin, snslib::BaseInfo &stBaseInfo);

    //�޸���ҵĻ�����Ϣ��QQ������ֵ(��̨����ݾ����Զ�������ҵȼ����ȼ���0)������ֵ������ʯ
    int XBoyModifyPlayerBaseInfo(unsigned uiUin, snslib::BaseInfo &stBaseInfo);

    //��ӡ��ҵ�ս����Ϣ��ս�����ơ��߼�̽Ա����
    int XBoyPrintPlayerTeamInfo(unsigned uiUin, snslib::TeamInfo &stTeamInfo);

    //��ӡ��ҵĵ���
    int XBoyPrintPlayerGoods(unsigned uiUin, std::vector<snslib::GoodsUnit>& vItems);

    //��ӻ���ٵ���
    int XBoyModifyPlayerGoods(unsigned uiUin, std::vector<snslib::GoodsUnit>& vItems);
    /*****************************ҵ������Ҫ���API�ӿ�**********************************/



    /**********************************����ʹ�õ�һЩ�ӿ�API*****************************/
    //���ע��
    int XBoyRegister(unsigned uiUin);

    //��ӡ��ҵĻ������ԣ�����������������ֵ������ֵ
    int XBoyPrintPlayerBaseAttr(unsigned uiUin, BaseAttrUnit& stBaseAttrUnit);

    //����������ҵĻ������ԣ�����������������ֵ������ֵ,��������ĵ�����Ϊ0
    int XBoyModifyPlayerBaseAttr(unsigned uiUin, BaseAttrUnit& stBaseAttrUnit);

    //��ӡ��ҵĽ�����ս�б�
    int XBoyPrintTodayFightList(unsigned uiUin, std::vector<snslib::FightUnit>& vItems);

    //������ҵĽ�����ս�б�-���õ�Ԫ��д,time�ֶ�-1��ʾɾ����Ԫ��,0��ʾ����ԭ��ʱ�䣬�������������ʱ��
    int XBoyModifyTodayFightList(unsigned uiUin, std::vector<snslib::FightUnit>& vItems);

    //��ӡ��ҵ���ʷ��ս�б�
    int XBoyPrintHistoryBeFightList(unsigned uiUin, std::vector<snslib::FightUnit>& vItems);

    //������ҵ���ʷ��ս�б�-���õ�Ԫ��д,time�ֶ�-1��ʾɾ����Ԫ��,0��ʾ����ԭ��ʱ�䣬�������������ʱ��
    int XBoyModifyHistoryBeFightList(unsigned uiUin, std::vector<snslib::FightUnit>& vItems);

    //��ӡ��ҵ������б�
    int XBoyPrintPlayerTaskList(unsigned uiUin, std::vector<snslib::TaskUnit>& vItems);

    //�޸���ҵ������б�step�ֶ�Ϊ-1��ʾҪ��������б��еĸ������������б�û�и�������ô�����Ӹ�����
    int XBoyModifyPlayerTaskList(unsigned uiUin, std::vector<snslib::TaskUnit>& vItems);

    //��ӡ��ҵ�ϵ�������б�
    int XBoyPrintPlayerSeriesTaskList(unsigned uiUin, std::vector<snslib::SeriesTaskUnit>& vItems);

    //�޸���ҵ�ϵ�������б�-level�ֶ�Ϊ-1����ɾ����������ϵ�������б���û�и�������ô�����Ӹ�ϵ������
    int XBoyModifyPlayerSeriesTaskList(unsigned uiUin, std::vector<snslib::SeriesTaskUnit>& vItems);

    //��ӡ�������
    int XBoyPrintPlayerDzdCount(unsigned uiUin, unsigned& uiCount);

    //��������޸�
    int XBoyModifyPlayerDzdCount(unsigned uiUin, unsigned uiCount);

    //��ӡ�����Ϣ
    int XBoyPrintPlayerDzdInfo(unsigned uiUin, std::vector<snslib::DzdUnit>& vItems);

    //���ļ����Ϣ-ʱ���ֶ�Ϊ-1��ʶɾ���ü��������Ϊ��д�����ڸ�id�ļ���������Ӽ����ʱ���ֶ�Ϊ0��ʾ����ϵͳʱ�䡣
    int XBoyModifyPlayerDzdInfo(unsigned uiUin, std::vector<snslib::DzdUnit>& vItems);

    //��ӡBOSS����
    int XBoyPrintPlayerBossBag(unsigned uiUin, std::vector<snslib::BossUnit>& vItems);

    //�޸�BOSS��Ϣ-ʱ���ֶ�Ϊ-1��ʶɾ����BOSS������Ϊ��д�����ڸ�id��BOSS��������BOSS��ʱ���ֶ�Ϊ0��ʾ����ϵͳʱ�䡣
    int XBoyModifyPlayerBossBag(unsigned uiUin, std::vector<snslib::BossUnit>& vItems);

    //���£���һ�θ���Ϊ��ӣ�RandomSvr�е�һ������
    int XBoyRefreshRandomSvrPlayerInfo(unsigned uiUin, RandomUnit& Item);

    //Ϊһ������������ս����
    int XBoyRandomFightPlayer(unsigned uiUin, RandomUnit& Item, std::vector<unsigned int>& vBlackList,
            std::vector<unsigned int>& vRandomList);

    //��ȡ��ҵļ���Ϣ
    int XBoyPrintSimpleInfo(unsigned uiUin, snslib::SimpleInfo& stItems);

    //�޸���ҵļ���Ϣ
    int XBoyModifySimpleInfo(unsigned uiUin, snslib::SimpleInfo& stItems);

    //��ȡ��ҵ���ս������ʤ����ʧ�ܳ���
    int XBoyPrintFightCount(unsigned uiUin, int &iWonCount, int &iLostCount);

    //�޸���ҵ���ս������ʤ����ʧ�ܳ���
    int XBoyModifyFightCount(unsigned uiUin, int iWonCount, int iLostCount);

    //��ӡ��һ�õ�ѫ���б�
    int XBoyPrintHonorTitle(unsigned uiUin, std::vector<HonorTitleUnit> &vHonorTitle);

    //�޸���ҵ�ѫ�¡�time=-1��ʾɾ����ѫ�£�time=0��ʶ����ѫ�²�����õ�ʱ����Ϊϵͳʱ�䣬time=other��ʶ����������Ϊ���ʱ��
    int XBoyModifyHonorTitle(unsigned uiUin, int iHonorID, long lTime);

    //�������0
    int XBoyResetPlayerInfo(unsigned int uiUin);
};
}
#endif /* WEBAPI_XBOY_H_ */
