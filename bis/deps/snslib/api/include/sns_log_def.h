// =====================================================================================
//
//       Filename:  sns_log_def.h
//
//    Description:  ��־��ض���
//
//        Version:  1.0
//        Created:  08/10/2010 05:31:00 PM
//       Revision:  none
//       Compiler:  g++
//
//         Author:  MichaelZhao (Zhao Guangyu), michaelzhao@tencent.com
//        Company:  TENCENT
//
// =====================================================================================

#pragma once

namespace snslib{

// ������ENUM������Ч��ֹID�ظ�
//
// ҵ��ʹ�����ºŶι���
// APPID_000 - APPID_999

enum{

// PCL 1000-1999
LOG_PCL_STATISTICS			= 1001,		// ͳ����־
LOG_PCL_CONN				= 1011,		// ������
LOG_PCL_TERMINATE			= 1012,		// ���ӹر�
LOG_PCL_DATA_RECV			= 1021,		// �յ�����
LOG_PCL_DATA_SEND			= 1022,		// ��������
LOG_PCL_DATA_BUFFER_SEND	= 1023,		// ���淢������
LOG_PCL_CMD_RECV			= 1031,		// �յ�����
LOG_PCL_CMD_SEND			= 1032,		// ��������
LOG_PCL_CMD_BUS_SEND		= 1041,		// BUS��������
LOG_PCL_CMD_BUS_RECV		= 1042,		// BUS��������

LOG_PAY_INFO                = 10001,    // ?��??
LOG_PAY_GOODS               = 10002,    // ???��????
LOG_PAY_PROVIDE             = 10003,    // ����??????

// LOCK
LOG_LOCK_GET				= 2000,		// ��ȡ��
LOG_LOCK_SET				= 2001, 	// ������
LOG_LOCK_DEL				= 2002,		// ɾ����
LOG_LOCK_TIMEOUT			= 2010,		// ����ʱ���

// FLAG
LOG_FLAG_GET				= 2100,		// ��ȡ��־λ
LOG_FLAG_SET				= 2101,		// ���ñ�־λ
LOG_FLAG_DEL				= 2102,		// ɾ����־λ

// KONGFU CACHE
LOG_KF_CACHE_GET            = 1031300,  // ��������
LOG_KF_CACHE_SET            = 1031301,  // ��������
LOG_KF_CACHE_NEW            = 1031302,  // ������ɫ
LOG_KF_CACHE_DEL            = 1031303,  // ɾ����ɫ
LOG_KF_CACHE_DB             = 1031304,  // ��DB�м���

// KONGFU Gateway
LOG_KF_GW_GET_ADOPT_REQ     = 1031000,
LOG_KF_GW_GET_ADOPT_RSP     = 1031001,
LOG_KF_GW_GET_PET_REQ       = 1031002,
LOG_KF_GW_GET_PET_RSP       = 1031003,
LOG_KF_GW_UPDATE_PET_REQ    = 1031004,
LOG_KF_GW_UPDATE_PET_RSP    = 1031005,
LOG_KF_GW_MSG_CENTRE        = 1031006,

// KONGFU Dir
LOG_KF_DIR_REQ              = 1030900,
LOG_KF_DIR_RSP              = 1030901,
LOG_KF_DIR_OL               = 1030902,

// KONGFU App
LOG_KF_APP_LOGIN            = 1030000,
LOG_KF_APP_LOGOUT           = 1030002,
LOG_KF_APP_HELLO            = 1030003,
LOG_KF_APP_BUY              = 1030004,
LOG_KF_APP_EXCHANGE         = 1030005,
LOG_KF_APP_USE              = 1030006,
LOG_KF_APP_OL               = 1030007,
LOG_KF_APP_LEARN            = 1030008,
LOG_KF_APP_START_FIGHT      = 1030009,
LOG_KF_APP_END_FIGHT        = 1030010,
LOG_KF_APP_START_TASK       = 1030011,
LOG_KF_APP_END_TASK         = 1030012,
LOG_KF_APP_TASK_PRIZE       = 1030013,
LOG_KF_APP_FIGHT_ACTION     = 1030014,
LOG_KF_APP_PET_YUANBAO      = 1030015,
LOG_KF_APP_PET_GROWTH       = 1030016,

// LOADER WRITER
LOG_LW_LOAD_DATA			= 2200,		// ��������
LOG_LW_WRITE_DATA			= 2201, 	// ��д����
LOG_LW_DEL_DATA				= 2202,		// ɾ������

};// end of enum

}// end of namespace petlib

