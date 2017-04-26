// =====================================================================================
// 
//       Filename:  vip_def.cpp
// 
//    Description:  VIP��ض���
// 
//        Version:  1.0
//        Created:  2010��10��17�� 13ʱ51��05��
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  MichaelZhao (Zhao Guangyu), michaelzhao@tencent.com
//        Company:  TENCENT
// 
// =====================================================================================

#include "vip_def.h"

#include <map>
#include <vector>

namespace snslib{
namespace vip{

static const unsigned short _StatusTimes[][LV_MAX] = {
	{ 13,	6,7,7,7,8,8,10 },	// ��ѩ
	{ 17,	4,4,5,5,6,6,8  },	// ����
	{ 18,	4,4,5,5,6,6,8  },	// ��������
	{ 19,	4,4,5,5,6,6,8  },	// �߶���
	{ 20,	4,4,5,5,6,6,8  },	// ���Ļ���
};

unsigned short StatusTimes(unsigned int status, unsigned short vip_level){
	if(vip_level < LV1)
		vip_level = LV1;

	if(vip_level > LV7)
		vip_level = LV7;

	for(size_t i = 0; i < sizeof(_StatusTimes) / sizeof(_StatusTimes[0]); ++i){
		if(_StatusTimes[i][0] == status){
			return _StatusTimes[i][vip_level];
		}
	}

	return 0;
}

}
}
