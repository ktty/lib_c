// =====================================================================================
// 
//       Filename:  vip_proxy_proto.h
// 
//    Description:  Э��
// 
//        Version:  1.0
//        Created:  2010��11��30�� 10ʱ21��14��
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  MichaelZhao (Zhao Guangyu), michaelzhao@tencent.com
//        Company:  TENCENT
// 
// =====================================================================================

#pragma once

namespace snslib{

#pragma pack(1)

struct CmdGetVipDataReq{
	unsigned int dwUin;
};

struct CmdGetVipDataRsp{
	unsigned int dwUin;
	int dwResult;

	struct{
		unsigned int		growth;				// �ɳ�ֵ
		unsigned short		days_left_normal;	// ��ͨ�ɳ�
		unsigned short		days_left_medium;	// ���ٳɳ�
		unsigned short		days_left_fast;		// ���ٳɳ�
		unsigned short		days_left_rapid;	// ���ٳɳ�

		unsigned char		vip_flag;			// ������ 1�� 0����
		unsigned char		year_flag;			// ��ѱ��
		unsigned short		level;				// �ȼ�
		unsigned short		growth_rate;		// �ɳ��ٶ�
	};
};

#pragma pack()

}
