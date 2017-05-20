/*
����: ��һ��map�����ݳ־û���share memory�С�

��ʽ: 
1.��Ϊ�˱��ڲ��ң�ʹ��std::map
2.�־û��Ĺ��̣�ÿ�εĲ���ɾ�����������ڳ�����map�����⣬��Ҫ�޸�shm�е�����
3.�ṩ��shm���������ݵ�map�ķ���

*/

#ifndef  MAP_SERIALIZE_H_
#define MAP_SERIALIZE_H_

#include "comm/log/pet_log.h"
#include "fixedsize_mem_pool.h"
#include <tr1/unordered_map>
#include <iostream>
//#include <map>

using namespace std;

namespace snslib
{

template<class DATA_TYPE> 
class CMapSerialize
{

    typedef struct
    {
        unsigned int uiUin;
        DATA_TYPE stData;
    } DATASAVE;


public:

    //typedef std::map<unsigned int,long> hash_map; //uin�������ڴ�ƫ�Ƶ�ӳ��
    typedef std::tr1::unordered_map<unsigned int,long> hash_map; //uin�������ڴ�ƫ�Ƶ�ӳ��

    int init(char *pvMem, int iMemSize, int iClearFlag = 0)
    {
        PetLog(0, 0, PETLOG_TRACE, "%s|pvMem=%p,iMemSize=%d,iClearFlag=%d ", __func__, pvMem, iMemSize, iClearFlag);

        m_pvMem = pvMem;
        m_pMemPool = new CFixedsizeMemPool<DATASAVE>();
        int iRet = m_pMemPool->Init(pvMem, iMemSize, 0, iClearFlag);
        if (0 != iRet)
            return iRet;

        //���m_pMemPool�������ݣ���������
        DATASAVE* pData = m_pMemPool->GetNextNode(1);
        for (int i = 0; i < m_pMemPool->GetUsedNodeNum(); i++)
        {
            if (NULL != pData)
            {
                m_oUinMap.insert(make_pair(pData->uiUin, (char*)(pData) - m_pvMem));
              //  PetLog(0, 0, PETLOG_DEBUG, "%s|uin=%u,offset=%l", __func__, pData->uiUin, (long)((char*)(pData) - m_pvMem));

            }
            pData = m_pMemPool->GetNextNode();
        }

        PetLog(0, 0, PETLOG_DEBUG, "%s|Load from MemPool succ. total records=%lu", __func__, m_oUinMap.size());

        return 0;
    }

    int get_need_memsize(int iNum)
    {
        return m_pMemPool->GetNeedMemSize(iNum);
    }

    //return:0�ɹ�    <0ʧ��
    int insert(unsigned int uiKey, DATA_TYPE& data)
    {
        DATASAVE* pData = NULL;
        //����Ѿ�������map�С�����֮. ��������ڣ��򴴽����
        typename hash_map::iterator it = m_oUinMap.find(uiKey);
        if (it != m_oUinMap.end())
        {
            PetLog(0, 0, PETLOG_DEBUG, "%s|find [uiKey=%u] in map, update it", __func__, uiKey);
            pData = (DATASAVE*)(m_pvMem + m_oUinMap[uiKey]);
        }
        else
        {
            PetLog(0, 0, PETLOG_DEBUG, "%s|cannot find [uiKey=%u] in map, create in shm and add it into map", __func__,
                   uiKey);
            pData = m_pMemPool->AllocateNode();
            if (NULL == pData) //���Ʋ����ڵ������
            {
                PetLog(0, 0, PETLOG_WARN, "%s|AllocateNode failed.[uiKey=%u] ", __func__, uiKey);
                return -1;
            }
            else
            {
                m_oUinMap[uiKey] = (char*)(pData) - m_pvMem;
            }

        }

        pData->uiUin = uiKey;
        memcpy(&(pData->stData ), &data, sizeof(DATA_TYPE));

        PetLog(0, 0, PETLOG_DEBUG, "%s|add [uiKey=%u] in map succ", __func__, uiKey);
        return 0;

    }

    //return:0 �ɹ�  -1û������  <0ɾ��ʧ��
    int erase(unsigned int uiKey, DATA_TYPE& data)
    {
        //��map���Ҳ������Ͳ�ɾ����

        typename hash_map::iterator it = m_oUinMap.find(uiKey);
        if (it == m_oUinMap.end())
        {
            PetLog(0, 0, PETLOG_WARN, "%s|cannot find [uiKey=%u] in map ", __func__, uiKey);
            return -1;
        }

        //�Ƴ�m_pMemPool�е�����
        DATASAVE* pData = (DATASAVE*)(m_pvMem + m_oUinMap[uiKey]);
        memcpy(&data, &pData->stData, sizeof(data));
        m_pMemPool->ReleaseNode(pData);

        //�Ƴ�map������
        m_oUinMap.erase(it);

        PetLog(0, 0, PETLOG_DEBUG, "%s|erase [uiKey=%u] from map succ", __func__, uiKey);
        return 0;
    }

 
    //return:0 �ɹ�  -1û������  <0ɾ��ʧ��
    int erase(unsigned int uiKey)
    {
        typename hash_map::iterator it = m_oUinMap.find(uiKey);
        if (it == m_oUinMap.end())
        {
            PetLog(0, 0, PETLOG_WARN, "%s|cannot find [uiKey=%u] in map ", __func__, uiKey);
            return -1;
        }

        return erase(it);

    }

    int erase(typename hash_map::iterator it)
    {
        //�Ƴ�m_pMemPool�е�����
        DATASAVE* pData = (DATASAVE*)(m_pvMem + it->second);
        //memcpy(&data, &pData->stData, sizeof(data));
        m_pMemPool->ReleaseNode(pData);

        PetLog(0, 0, PETLOG_DEBUG, "%s|erase [uiKey=%u] from map succ", __func__,it->first);
        //�Ƴ�map������
        m_oUinMap.erase(it);

        return 0;

    }

    template <typename CallBack>
    int foreach(CallBack cb, void* arg)
    {
        typename hash_map::iterator it = m_oUinMap.begin();
        for (; it != m_oUinMap.end(); it++)
            cb(it->first, ((DATASAVE*)(m_pvMem + it->second))->stData, arg);

        return 0;
    } 


    template < typename Key, typename Value, typename Comp, typename Alloc, typename Pred>
        void zorch_if(tr1::unordered_map<Key,Value,Comp,Alloc>& m , Pred p)
    {
            typedef typename tr1::unordered_map<Key,Value,Comp,Alloc>::iterator iter_t;

            for(iter_t i = m.begin(); i != m.end(); )
            {
                if(p(*i))
                {
                    iter_t dommed = i;
                    ++i;
                    m.erase(dommed);
                }
                else
                {
                    ++i;
                }
            }

    }


    template <typename Pred>
        void remove_if_m(Pred p)
    {
            typedef typename hash_map::iterator iter_t;

            for(iter_t i = m_oUinMap.begin(); i != m_oUinMap.end(); )
            {
                PetLog(0, 0, PETLOG_DEBUG, "%s|key:%u data:%ld", 
                        __func__, i->first,i->second);
                if(p(i->first,((DATASAVE*)(m_pvMem + i->second))->stData))
                {
                    PetLog(0, 0, PETLOG_DEBUG, "%s|%u|need erase",__func__, i->first);
                    iter_t dommed = i;
                    ++i;
                    erase(dommed);
                    //m.erase(dommed);
                }
                else
                {
                    ++i;
                }
            }

    }



    //return:0 ���ҵ�  -1 δ�ҵ���¼
    int find(unsigned int uiKey, DATA_TYPE& data)
    {

        memset(&data, 0, sizeof(data));
        typename hash_map::iterator it = m_oUinMap.find(uiKey);

        if (it == m_oUinMap.end())
        {
            PetLog(0, 0, PETLOG_DEBUG, "%s|cannot find [uiKey=%u] in map ", __func__, uiKey);
            return -1;
        }
        else
        {

            DATASAVE* pData = (DATASAVE*)(m_pvMem + m_oUinMap[uiKey]);
            memcpy(&data, &pData->stData, sizeof(data));
            PetLog(0, 0, PETLOG_DEBUG, "%s|find [uiKey=%u] from map succ", __func__, uiKey);
        }
        return 0;
    }

    inline size_t size()
    {
        return m_oUinMap.size();
    }

public:

    CMapSerialize():m_pMemPool(NULL),m_pvMem(NULL)
    {

    }
    ~CMapSerialize()
    {
        if (m_pMemPool)
            delete m_pMemPool;

    }


private:
    hash_map m_oUinMap;
    CFixedsizeMemPool<DATASAVE>* m_pMemPool;
    char* m_pvMem; //�����ݽ������λ�õ�ƫ������ڴ�

};
}



#endif
