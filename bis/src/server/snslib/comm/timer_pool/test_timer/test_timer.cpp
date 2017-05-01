/**
 * @file    test_timer.cpp
 * @brief   ����timer_pool
 * @author  smiyang@tencent.com
 * @date    2010-08-18
  */
//============================================================================
#include <stdio.h>
#include <errno.h>
#include <time.h>
#include <iostream>
#include <fstream>

#include "comm/timer_pool/timer_pool.h"

using namespace std;
using namespace snslib;

typedef struct tagTimerParamInfo
{
    unsigned int uiUin;
    unsigned int uiModuleID;    //����ģ���������ݸ�ֵ��ȷ�����Ǹ��������õĶ�ʱ��(APP or TRANSACTION)
    unsigned int uiParam;       //4�ֽڲ���
    char szUnUsed[28];
}TimerParamInfo;

int main(int argc, char* argv[])
{
     //OpenPetLog("timer");

	//timer_pool�ڴ��ʼ��
    // �����ﲢû��ʹ�õ������ڴ�
    int iTimerShmKey = 13333;
    int iTimerShmSize = 80*1024*1024;
    int iRetVal = 0;

    CTimerPool<TimerParamInfo> * m_pobjTimerPool = new CTimerPool<TimerParamInfo>();
    if (m_pobjTimerPool == NULL)
    {
        printf( "new CTimerPool failed, errno=%d, errmsg=%s", errno, strerror(errno));
        return -1;
    }

    //��ʼ����ʱ����
    void *pvMem = malloc(iTimerShmSize);
    iRetVal = m_pobjTimerPool->Init(pvMem, iTimerShmSize, 1);
    if(iRetVal != 0){
    	printf("TimerPool Init failed, ret=%d\n", iRetVal);
    	return -1;
    }

    //����1���ܹ���Ӷ��ٽڵ�
    clock_t start = clock();
    double  dDuration = 0;
    unsigned long long ullCount = 0, ullLostCount = 0;
    while( dDuration < 1 ){
    	TimerParamInfo stTimerParamInfo;
    	memset(&stTimerParamInfo, 0x0, sizeof(TimerParamInfo));
    	stTimerParamInfo.uiUin = 0;
    	stTimerParamInfo.uiModuleID = 1;
    	stTimerParamInfo.uiParam = 2;

    	unsigned long long ullTimerID = 0;

    	iRetVal = m_pobjTimerPool->AddTimer( 100*10, stTimerParamInfo, &ullTimerID);
    	if(iRetVal != 0){
    		printf("add TimerNode failed, ret=%d\n", iRetVal);
    		++ullLostCount;
    	}else{
    		++ullCount;
    	}

    	clock_t end = clock();
    	dDuration = (double)(end-start)/CLOCKS_PER_SEC;
    }

    cout << "ʱ��γ�Ϊ�� " << dDuration << "�ɹ���ӽڵ����Ϊ�� " << ullCount << "��ӽڵ�ʧ�ܸ���Ϊ��"<< ullLostCount<< endl;


    //ȡ���ڵ�
    unsigned long long ullGetCount = 0;
    ofstream outfile("test_timer.txt", iostream::app);
    std::vector<TimerParamInfo> vstTimerParam;
    std::vector<unsigned long long> vullTimerID;
    while(ullCount > 0){
    	vstTimerParam.clear();
        vullTimerID.clear();
        m_pobjTimerPool->GetTimer(vullTimerID, vstTimerParam);
        unsigned int uiH, uiM, uiS;
        //�˺����õ�ʱ���ֵ�3��ָ��ƫ������Ϊ��ԭ����Ļ����Ϻ���ӵĺ�����
        //�������޷�ͨ���ģ���Ϊ�ú����Ѿ�ɾ����������Ӹú���������⣬�Ա�۲�ָ���ƫ����
        m_pobjTimerPool->GetHMS(uiH, uiM, uiS);
        if(vstTimerParam.size() != 0){
           outfile << "ʱ��ƫ������" << uiH << " ����ƫ������" << uiM << " J����ƫ������" << uiS <<endl;
           outfile << "���δ���������" << vstTimerParam.size() << endl;
           ullCount -= vstTimerParam.size();
           ullGetCount += vstTimerParam.size();
        }
    }
    cout << "��ȡ���ڵ����Ϊ��" << ullGetCount << endl;

    return 0;
}
