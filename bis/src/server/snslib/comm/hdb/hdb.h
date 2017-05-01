/*
 * hdb.h
 *
 *  Created on: 2010-10-17
 *      Author: rayxia
 */

#ifndef __HDB_H__
#define __HDB_H__

#include<stdint.h>
#include<stdio.h>
#include<unistd.h>

namespace snslib{
typedef struct {
	char			magic[16];				// ��ʶ��
	int				fd;						// �ļ�������
	uint64_t		bnum;					// ������
	uint64_t		fbnum;					// ���п�����
	uint16_t		bsiz;					// ���С
	uint32_t		hbnum;					// hashͰ����
	uint64_t		fboff;					// ���п�ƫ��
	uint64_t		fmaxsiz;				// mmapӳ��ռ��С
	uint64_t		fsiz;					// �ļ���С
	char * 			map;					// ӳ���ļ�ָ��
	uint16_t		fbaddnum;				// ÿ����ӵĿ��п����
}HDB_HEADER;

typedef struct {
	uint64_t 		key;
	uint16_t		size;					// ���С�����64k
	uint64_t		next;
	uint64_t		pre;
	uint64_t		down;
	char 			* data[0];
}HDB_BHEADER;

class HDB {
public:
	const static int SUCC		= 0;
	const static int ERROR		= -1;
	const static int NONODE		= -2;

public:
	HDB();
	~HDB();

	// init
	int init( char * fpath, uint16_t bsize, uint32_t hbnum, uint64_t xmsiz );
	// put reocrd
	int put(uint32_t key, char * buf, uint16_t len);
	// get record buf, 'len' must set to 'buf' length
	int get(uint32_t key, char * buf, uint16_t * len );
	// delete record
	int out(uint32_t key);

	char * getemsg(){
		return m_emsg;
	}

private:
	bool lock(int fd);
	bool unlock(int fd);
	bool seekwrite( uint64_t off, const void *buf, size_t size);
	bool seekread(uint64_t off, void *buf, size_t size);
	bool addfb();
	bool retfb(uint64_t off);
	uint32_t hash(uint32_t key);
	bool getfb(uint64_t *poff);

private:
	char 			m_emsg[1024];
	HDB_HEADER		* m_dbheader;
	char 			* m_prbuf;
};
}

#endif /* __HDB_H__ */
