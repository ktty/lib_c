// =====================================================================================
// 
//       Filename:  vip_ttc.h
// 
//    Description:  VIP TTC����API
// 
//        Version:  1.0
//        Created:  2010��09��15�� 10ʱ08��42��
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  MichaelZhao (Zhao Guangyu), michaelzhao@tencent.com
//        Company:  TENCENT
// 
// =====================================================================================

#pragma once

#include <vector>

namespace TencentTableCache{
	class Server;
}

namespace snslib{

class CVipTTC{
	public:
		// VIPԭʼ����
		// GetVipDataʱ����ȡ������ΪDBԭʼ����
		// SetVipDataʱ��growth��days_left_xxx�ֶ�Ϊ��/��������
		// TTC��ִ��Add/Sub�����Ա�֤����ԭ���ԣ���Ҫ���óɾ�����!!!
		struct VipData{
			VipData() { memset(this, 0x0, sizeof(*this)); }

			int					mask;				// ��������

			int					growth;				// �ɳ�ֵ
			int					days_left_normal;	// ��ͨ�ɳ�
			int					days_left_medium;	// ���ٳɳ�
			int					days_left_fast;		// ���ٳɳ�
			int					days_left_rapid;	// ���ٳɳ�
			unsigned int		proc_timestamp;		// �ϴδ���ʱ��
			unsigned int		check_and_set;		// Ψһ��ʶ
			char				non_prepaid_flag;	// ���±��(0Ԥ����,1�ֻ�,3���)
		};

		// ��������
		enum{
			ADD_GROWTH = 1 << 0,
			ADD_DAYS_LEFT_NORMAL = 1 << 1,
			ADD_DAYS_LEFT_MEDIUM = 1 << 2,
			ADD_DAYS_LEFT_FAST = 1 << 3,
			ADD_DAYS_LEFT_RAPID = 1 << 4,
			SET_PROC_TIMESTAMP = 1 << 5,
			CHECK_AND_SET = 1 << 6,
			SET_NON_PREPAID_FLAG = 1 << 7,
		};

	public:
		// ���캯��
		CVipTTC();

		// ��������
		~CVipTTC();

		// ��ȡ�����ļ�����ʼ��TTCʵ��
		int Initialize(const char *conf);

		// �ͷ�TTCʵ��
		void Finalize();

		// ��ȡ����
		int GetVipData(unsigned int uin, VipData &vip_data);

		// ��������
		int SetVipData(unsigned int uin, const VipData &vip_data);

		// �õ���������
		const char* GetErrMsg();

	private:
		// ��ȡTTCʵ��
		TencentTableCache::Server* GetTTCServer(unsigned int uin);

		// ���û�и��û����ݣ��򴴽�һ�����û�����
		int TouchVipData(unsigned int uin);

	public:
		// �ṩһЩ��̬����ͳһ�����߼�
		// ����ȼ�
		static unsigned short GetLevel(const VipData &vip_data);
		// ����ɳ��ٶ�
		static unsigned short GetGrowthRate(const VipData &vip_data);
		// ������ѱ��
		static unsigned char GetYearFlag(const VipData &vip_data);
		// ���������
		static unsigned char GetVipFlag(const VipData &vip_data);
		// ����ʣ���������
		static unsigned short GetLeftDays(const VipData &vip_data);
		// ��ȡ�������������
		static void Adjust(VipData &vip_data, const VipData &update_data);

	private:
		// TTC�����ļ���
		struct TTC_CONF{
			// [40, 45) ע�����俪��
			int uin_range_start;
			int uin_range_end;
			// xxx.xxx.xxx.xxx:xxxxx
			char addr[32];
			char table_name[128];
		};

	private:
		char m_err_msg[1024];
		std::vector<TTC_CONF> m_ttc_conf;
		std::vector<TencentTableCache::Server *> m_ttc_server;
};

}
