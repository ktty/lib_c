// =====================================================================================
// 
//       Filename:  vip_tool.h
// 
//    Description:	����
// 
//        Version:  1.0
//        Created:  2010��11��01�� 15ʱ08��04��
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  MichaelZhao (Zhao Guangyu), michaelzhao@tencent.com
//        Company:  TENCENT
// 
// =====================================================================================

#pragma once

#include <vector>
#include "vip_ttc.h"
#include "api/web_api/webapi.h"
#include "api/adopt_api/adopt_api.h"
#include "api/oidb_api/oidb_api.h"

// DB������
struct DB_CONF{
	DB_CONF() { memset(this, 0x0, sizeof(*this)); }

	char addr[128];
	int port;
	char user[128];
	char passwd[128];
	char db_name[128];
	int db_suffix_start;
	int db_suffix_end;
	char table_name[128];
};

// ȫ������
extern int db_num;
extern std::vector<DB_CONF> db_conf_vec;
extern char web_api_conf[128];
extern char adopt_api_conf[128];
extern char vip_ttc_conf[128];
extern char oidb_api_conf[128];

// �Ƿ�ͬ����OIDB�����Ի�������Ҫͬ����
extern int sync_oidb;
// �Ƿ�ͬ�������ϵͳ
extern int sync_penguin;
// ֻ������Ч�û�
extern int effective_only;

// ȫ�ֶ���
extern petlib::CAdoptAPI adopt_api;
extern petlib::CVipTTC vip_ttc;
extern petlib::COIDBProxyAPI oidb_api;

// ����Ƿ��ǹ�ȥ������
extern bool IsPastDay(time_t then);

// ����OIDB�ȼ�
extern int OIDBModifyLevel(unsigned int uiUin, unsigned short ushLevel);

// ����OIDB��ѱ��
extern int OIDBModifyYearFlag(unsigned int uiUin, char cYearFlag);

// ֪ͨ��Ϣ���ĺ�feedsvr
extern int Notify(unsigned int uiUin,
		std::vector<petlib::MsgCentreMsg> &vMsg,
		petlib::UpdatePetAttrReq &stUpdatePetAttrReq);

// ��ȡ�����ļ�
extern int LoadConf(const char *conf);
extern int LoadConfCustom(const char *conf);

// ��ʼ���ӿ�
extern int InitApi();
extern int InitApiCustom();

// Dump���ݿ�UIN
extern int DumpUin(const DB_CONF &conf, std::vector<unsigned int> &uin_vec);
