/*
 * =====================================================================================
 *
 *       Filename:  shm_queue_ex.h
 *
 *    Description:  �����ڴ����Ex
 *
 *        Version:  1.0
 *        Created:  2010��03��05�� 15ʱ04��31��
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  Michaelzhao (�Թ���), michaelzhao@tencent.com
 *        Company:  Tencent
 *
 * =====================================================================================
 *	�ڴ�ṹ����������
 *				
 *						 -->
 *			tail(pop)	 head(push)
 *			  |			  |
 *	|_________|_____|_____|_____|.....|_____|
 *	 Header   |data1 data2 data3	   dataN|
 *			  |								|
 *			  |_____________________________|
 *						<--
 *
 *
 * �ʵ䣺
 * capacity			���������ɵĽڵ�����
 * data_size		���ݽڵ㳤�ȣ��̶�Ϊsizeof(DataType)
 * Index			��������ڵ���������0��ʼ
 *
 * ����ж϶��пգ�	tail == headʱ���ж϶���Ϊ��
 * ����ж϶�������	���head����һ���ڵ�Ϊtail���ж϶���������ʱ������Push��ʹhead==tail��
 *					�����Ͳ�֪�������������ǿ��ˣ�����һ���ڵ�Ϊ�գ����ǻ�����������/�յ�һ������������
 *					����������Ҫ��Capacity��һ���ڵ����������,�������ڵ���Init������ʼʱcapacity++��ʵ�֡�
 *					�����ڵ��Ե�ʱ�򿴵�Capacity()�������õ�capacity+1��ǧ��Ҫ���ȣ�Ҳ��Ҫ������^_^
 *
 * �����е���std::queue��Ӧ�ö��ܿ���
 *
 */

#pragma once

#include    "comm/sem_lock/sem_lock.h"

#include    <stdlib.h>
#include    <stdio.h>
#include    <string.h>
#include    <sys/types.h>
#include    <sys/ipc.h>
#include    <sys/sem.h>
#include    <sys/shm.h>
#include    <errno.h>

namespace snslib
{

template<typename DataType>
class CShmQueueEx
{
public:
    const static int SUCCESS = 0;
    const static int ERROR = -1;

    const static int E_SHM_QUEUE_SEMLOCK = -601;
    const static int E_SHM_QUEUE_SHMGET = -602;
    const static int E_SHM_QUEUE_SHMCTL = -603;
    const static int E_SHM_QUEUE_SHMAT = -604;
    const static int E_SHM_QUEUE_SHM_MISMATCH = -605;
    const static int E_SHM_QUEUE_FULL = -606;
    const static int E_SHM_QUEUE_EMPTY = -607;

	typedef unsigned int Index;

	struct QueueHeader{
		unsigned char magic;
		unsigned int capacity;		// ��������
		unsigned int data_size;		// ���ݴ�С
		Index idx_head;				// ����ͷָ�룬д������
		Index idx_tail;				// ����βָ�룬��ȡ����
	};

	class Callback{
		public:
			virtual ~Callback() { }
			// return: true continue, false break
			virtual bool Exec(DataType &data) = 0;
	};

	class ScorpLock{
		public:
			ScorpLock(CSemLock *lock) : m_pLock(lock) { m_pLock->Lock(); }
			~ScorpLock() { m_pLock->Release(); }
		private:
			CSemLock *m_pLock;
	};

public:
    CShmQueueEx();
    virtual ~CShmQueueEx();

    inline const char *GetErrMsg() const{ return m_szErrMsg; }

    int Init(int shm_key, unsigned int capacity);

	// locked ops
    int Push(const DataType &data);
	int Front(DataType *data);
    int Pop(DataType *data = NULL);
    unsigned int Size();
	bool Empty();
	bool Full();
	void ForEach(Callback *callback);

protected:
	// raw ops
    int Push_Nolock(const DataType &data);
	int Front_Nolock(DataType *data);
    int Pop_Nolock(DataType *data = NULL);
    unsigned int Size_Nolock();
	bool Empty_Nolock();
	bool Full_Nolock();
	void ForEach_Nolock(Callback *callback);

	// lock ops
	inline void SemLock() { m_pSemLock->Lock(); }
	inline void SemRelease() { m_pSemLock->Release(); }

	// header ops
	inline QueueHeader& Header() { return *((QueueHeader *)m_pMem); }
	inline const unsigned int Capacity() { return Header().capacity; }
	inline const unsigned int DataSize() { return Header().data_size; }
	inline Index& IdxHead() { return Header().idx_head %= Header().capacity; }
	inline Index& IdxTail() { return Header().idx_tail %= Header().capacity; }

	// data ops
	inline DataType& DataAt(Index idx){
		return *((DataType *)(m_pMem + sizeof(QueueHeader) + sizeof(DataType) * (idx % Capacity())));
	}

protected:
    char *m_pMem;
    CSemLock * m_pSemLock;
    char m_szErrMsg[256];
};

template<typename DataType>
CShmQueueEx<DataType>::CShmQueueEx() : m_pMem(NULL), m_pSemLock(NULL){
	memset(m_szErrMsg, 0x0, sizeof(m_szErrMsg));
}

template<typename DataType>
CShmQueueEx<DataType>::~CShmQueueEx(){
	if(m_pSemLock){
		delete m_pSemLock;
		m_pSemLock = NULL;
	}
	m_pMem = NULL;
}

template<typename DataType>
int CShmQueueEx<DataType>::Init(int shm_key, unsigned int capacity){
	// ��Ҫһ������ڵ����ж϶�����
	++capacity;

	const unsigned int shm_size = capacity * sizeof(DataType) + sizeof(QueueHeader);

	// init semlock
	m_pSemLock = new CSemLock();
	int rv = m_pSemLock->Init(shm_key);
	if(rv != CSemLock::SUCCESS){
		snprintf(m_szErrMsg, sizeof(m_szErrMsg), "sem_lock init failed, key=%d, ret=%d", shm_key, rv );
		return E_SHM_QUEUE_SEMLOCK;
	}

	// ���ﻹ����Ҫ���ϣ������߼����п���������
	ScorpLock sl(m_pSemLock);

	// create shm
	int shmid = shmget(shm_key, shm_size, 0666 | IPC_CREAT | IPC_EXCL);
	if(shmid < 0){
		if(errno != EEXIST){
            snprintf(m_szErrMsg, sizeof(m_szErrMsg), "shmget create failed, key=%d, shm_size=%d, errno=%d",
					shm_key, shm_size, errno);
			return E_SHM_QUEUE_SHMGET;
		}

		// get exist shm
		shmid = shmget(shm_key, 0, 0666);
		if(shmid < 0){
			snprintf(m_szErrMsg, sizeof(m_szErrMsg), "shmget create exist, but shmget failed, key=%d, shm_size=%d, errno=%d",
					shm_key, shm_size, errno);
			return E_SHM_QUEUE_SHMGET;
		}

		// check exist queue
		m_pMem = (char *)shmat(shmid, NULL, 0);
		if(m_pMem == NULL){
			snprintf(m_szErrMsg, sizeof(m_szErrMsg), "shm attach failed, shmid=%d, key=%d, shm_size=%d",
					shmid, shm_key, shm_size);
			return E_SHM_QUEUE_SHMAT;
		}

		// check queue size
		if(Header().capacity != capacity){
			snprintf(m_szErrMsg, sizeof(m_szErrMsg),
					"exist shm capacity mismatch, key=%d, capacity=%d, exist capacity=%d",
					shm_key, capacity, Header().capacity);
			return E_SHM_QUEUE_SHM_MISMATCH;
		}

		// check data size
		if(Header().data_size != sizeof(DataType)){
			snprintf(m_szErrMsg, sizeof(m_szErrMsg),
					"exist shm data_size mismatch, key=%d, data_size=%lu, exist data_size=%d",
					shm_key, sizeof(DataType), Header().data_size);
			return E_SHM_QUEUE_SHM_MISMATCH;
		}
	}
	else{	// new shm
		m_pMem = (char *)shmat(shmid, NULL, 0);
		if(m_pMem == NULL){
			snprintf(m_szErrMsg, sizeof(m_szErrMsg), "shm attach failed, shmid=%d, key=%d, shm_size=%d",
					shmid, shm_key, shm_size);
			return E_SHM_QUEUE_SHMAT;
		}

		Header().capacity = capacity;
		Header().data_size = sizeof(DataType);
	}

	return SUCCESS;
}

template<typename DataType>
int CShmQueueEx<DataType>::Push(const DataType &data){
	SemLock();
	int rv = Push_Nolock(data);
	SemRelease();
	return rv;
}

template<typename DataType>
int CShmQueueEx<DataType>::Front(DataType *data){
	SemLock();
	int rv = Front_Nolock(data);
	SemRelease();
	return rv;
}

template<typename DataType>
int CShmQueueEx<DataType>::Pop(DataType *data){
	SemLock();
	int rv = Pop_Nolock(data);
	SemRelease();
	return rv;
}

template<typename DataType>
unsigned int CShmQueueEx<DataType>::Size(){
	SemLock();
	unsigned int rv = Size_Nolock();
	SemRelease();
	return rv;
}

template<typename DataType>
bool CShmQueueEx<DataType>::Empty(){
	SemLock();
	bool rv = Empty_Nolock();
	SemRelease();
	return rv;
}

template<typename DataType>
bool CShmQueueEx<DataType>::Full(){
	SemLock();
	bool rv = Empty_Nolock();
	SemRelease();
	return rv;
}

template<typename DataType>
void CShmQueueEx<DataType>::ForEach(Callback *callback){
	SemLock();
	ForEach_Nolock(callback);
	SemRelease();
}

template<typename DataType>
int CShmQueueEx<DataType>::Push_Nolock(const DataType &data){
	if(Full_Nolock())
		return E_SHM_QUEUE_FULL;
	DataAt(IdxHead()) = data;
	++IdxHead();
	return SUCCESS;
}

template<typename DataType>
int CShmQueueEx<DataType>::Front_Nolock(DataType *data){
	if(Empty_Nolock())
		return E_SHM_QUEUE_EMPTY;
	*data = DataAt(IdxTail());
	return SUCCESS;
}

template<typename DataType>
int CShmQueueEx<DataType>::Pop_Nolock(DataType *data){
	if(Empty_Nolock())
		return E_SHM_QUEUE_EMPTY;
	if(data)
		*data = DataAt(IdxTail());
	++IdxTail();
	return SUCCESS;
}

template<typename DataType>
unsigned int CShmQueueEx<DataType>::Size_Nolock(){
	return ( Capacity() + IdxHead() - IdxTail() ) % Capacity();
}

template<typename DataType>
bool CShmQueueEx<DataType>::Empty_Nolock(){
	return IdxTail() == IdxHead();
}

template<typename DataType>
bool CShmQueueEx<DataType>::Full_Nolock(){
	return (IdxHead() + 1 ) % Capacity() == IdxTail();
}

template<typename DataType>
void CShmQueueEx<DataType>::ForEach_Nolock(Callback *callback){
	for(Index idx = IdxTail(); idx != IdxHead(); idx = ( idx + 1 ) % Capacity()){
		if(false == callback->Exec(DataAt(idx)))
			break;
	}
}

} // end of namespace snslib
