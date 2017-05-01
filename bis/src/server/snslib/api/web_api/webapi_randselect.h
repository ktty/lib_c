/***********************************************************
 * FileName:    webapi_randselect.h
 * Author:      smiyang
 * Date:		2010-9-10
 * Description:	Ϊ����Ƽ��ֶ����󼯺�
 * Version:		1.0
 * Function List:
 * 				1.	GetRecommendList()  //͸��WEBPCLΪ��ǰ��һ�ȡ�ֶ��б�
 * 				2.	UpdatePetLev()		//͸��WEBPCL������ҵȼ�
 ***********************************************************/
#ifndef _WEBAPI_RANDSELECT_H_
#define _WEBAPI_RANDSELECT_H_

#include <vector>
#include <map>

#include "webapi_base.h"

namespace snslib
{

class CWebApiRandSelect:public CWebApiBase
{
public:
	//���캯��
	CWebApiRandSelect();

	//��������
	~CWebApiRandSelect();

	/**
	 * @brief 	Ϊ��ǰ��һ�ȡ�ֶ�����
	 *
	 * @param 	uiUin���û���ʶ
	 * @param	usLev���û��ȼ�
	 * @param 	vBlackList��������-�����û��Ѿ��ֶ������û��б�
	 * @param 	vRecommendList��Ϊ��ǰ�û��Ƽ����ֶ��û��б�
	 * @return 	0��OK,������ERROR
	 */
	int GetRecommendList(unsigned int uiUin, unsigned short usLev, const std::vector<unsigned int> &vBlackList, std::vector<unsigned int> &vRecommendList);

	/**
	 * @brief 	Ϊ��ǰ��һ�ȡ�ֶ�����
	 *
	 * @param 	uiUin���û���ʶ
	 * @param	usLev���û��ȼ�
	 * @return 	0��OK,������ERROR
	 */
	int UpdatePetLev(unsigned int uiUin, unsigned short usLev);

private:
    char m_szSendBuff[MAX_WEBAPI_BUFF_LEN];
    int m_iSendLen;
};
}
#endif //_WEBAPI_RANDSELECT_H_
