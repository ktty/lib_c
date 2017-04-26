/**
 * @file    petlib_common.h
 * @brief   petlib���һЩ������������
 * @author  jamieli@tencent.com
 * @date    2008-08-12
 *
 * @note
 */
#ifndef _PETLIB_COMMON_H_
#define _PETLIB_COMMON_H_

namespace snslib
{

/*
 * ����һЩͨ�õ��ַ������ȡ���ʹ����Щ���������ַ�����ʱ�򣬶���������Ȼ�����+1
 * ͳһʹ�����·�ʽ�����磺
 * char szIP[MAX_IP_LEN + 1];
 * strncpy(szIP, szTemp, MAX_IP_LEN);
 */
const int MAX_IP_LEN            = 15;       // IP��ַ��󳤶�
const int MAX_NAME_LEN          = 64;       // ģ���������ö��������ñ�����
const int MAX_FILE_PATH_LEN     = 1024;     // ·�����ߴ�·�����ļ�����󳤶�
const int MAX_ERR_MSG_LEN       = 256;      // һ�������Ϣ�ַ�����󳤶�
const int MAX_LINE_LEN          = 1024;     // ��ȡ�ļ���һ�е�buff

}

#endif

