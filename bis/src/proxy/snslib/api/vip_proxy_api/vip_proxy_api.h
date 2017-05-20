// =====================================================================================
// 
//       Filename:  vip_proxy_api.h
// 
//    Description:  ������ϢAPI
// 
//        Version:  1.0
//        Created:  2010��09��08�� 10ʱ07��51��
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  MichaelZhao (Zhao Guangyu), michaelzhao@tencent.com
//        Company:  TENCENT
// 
// =====================================================================================

#pragma once

namespace snslib{

class CSafeTcpClient;

class CVipProxyApi{
	public:
		struct VipData{
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

	public:
		/*
		 * ���캯��
		 */
		CVipProxyApi();

		/*
		 * ��������
		 */
		~CVipProxyApi();

		/*
		 * ��ʼ��API
		 * param:	[IN] conf	�����ļ�
		 */
		int Init(const char *conf);

		/*
		 * ��ȡ��������
		 * param:	[IN]	uin			�û�UIN
		 * param:	[OUT]	vip_data	��������
		 * return:	���ش����룬��������enum
		 */
		int GetVipData(unsigned int uin, VipData &vip_data);

		/*
		 * ��ȡ���һ�δ�����Ϣ
		 * return:	��������
		 */
		const char* GetErrMsg();

	private:
		/*
		 * ������Ϣ
		 */
		char m_err_msg[1024];

		/*
		 * ��ȫTCP�ͻ���
		 */
		CSafeTcpClient *m_safe_tcp_client;
};

}// end of namespace snslib

