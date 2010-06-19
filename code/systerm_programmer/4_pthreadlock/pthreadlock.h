/*
 * =====================================================================================
 *
 *       Filename:  pthreadlock.h
 *
 *    Description:  pthread_lock
 *  				该线程锁用于双链表，由以下几个特点：
 *  				1.不依赖于具体操作系统平台				
 * 					2.支持多线程和单线程版本，有实现者进行加/解锁
 * 					3.区分多线程和单线程版本时，在运行时切换                  
 *        Version:  1.0
 *        Created:  2010年06月11日 00时29分50秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  
 *
 * =====================================================================================
 */

#ifndef PTHREADLOCK_H
#define PTHREADLOCK_H

#include <typedef.h>
#include <stdlib.h>

DECLS_BEGIN

typedef struct _Locker Locker;

typedef RET (*LockerFunlock)(Locker* thiz);
typedef RET (*LockerFununlock)(Locker* thiz);
typedef void (*LockerFunDestroy)(Locker* thiz);

struct _Locker
{
		LockerFunlock lock;
		LockerFununlock unlock;
		LockerFunDestroy destroy;

		char prvin[];
};

static inline RET Pthread_Lock(Locker* thiz)
{
		return_val_if_fail(NULL != thiz, RET_INVALID_PARAMS);

		return thiz->lock;
}

static inline RET Pthread_unLock(Locker* thiz)
{
		return_val_if_fail(NULL != thiz, RET_INVALID_PARAMS);

		return thiz->unlock;
}

static inline void Pthread_Dsestroy(Locker* thiz)
{
		return_val_if_fail(NULL != thiz, RET_INVALID_PARAMS);

		thiz->destroy;

		return;
}

DECLS_END

