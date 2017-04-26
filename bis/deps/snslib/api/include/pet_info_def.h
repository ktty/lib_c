/**
 * @file    pet_info_def.h
 * @brief   SESSION�洢��һЩ���ݽӿڣ�SESSIPN_API��������ݴ���������ø��ļ���������ݽṹ
 * @author  jamieli@tencent.com
 * @date    2009-03-09
 */

#ifndef _PET_INFO_DEF_H_
#define _PET_INFO_DEF_H_

#include "petlib_common.h"

namespace snslib
{

const int PET_NAME_LEN = 16;
const int COMMUNITY_INFO_NUM = 8;
const int PET_STATUS_DATA_LEN = 16;
const int MOBILE_NO_LEN = 20;
const int MAX_STUDIED_COURSE_INFO_NUM = 64;
const int MAX_AVATAR_INFO_NUM = 20;
/**
 * ����ID�Ľṹ����
 */
typedef union tagPetID
{
    unsigned long long ullID;
    struct
    {
        unsigned int uiUin;
        unsigned short ushSpec;
        unsigned char ucFamilyID;
        unsigned char ucSex;
    };
} PetID;

inline unsigned int GetUin(unsigned long long ullPetID)
{
    return ((PetID *)&ullPetID)->uiUin;
}

/**
 * ��ƷID�Ľṹ����
 */
typedef union tagGoodsID
{
    unsigned long long ullID;
    struct
    {
        //TODO ����ֻ��һ�����͵���Ʒ���������������͵���Ʒ���ṹ���岻ͬ����Ҫ��������
        unsigned short ushGoodsType;
        union
        {
            struct
            {
                unsigned char ucSex;
                unsigned char ucPetLevel;
                unsigned short ushGoodsGroup;
                unsigned short ushGoodsID;

            };
        };

    };
}CompoundGoodsID;

}

#endif

