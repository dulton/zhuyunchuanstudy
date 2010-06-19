/*
 * =====================================================================================
 *
 *       Filename:  pthreadlock.c
 *
 *    Description:  pthreadlock的实现
 *
 *        Version:  1.0
 *        Created:  2010年06月15日 13时22分48秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  
 *
 * =====================================================================================
 */

#include <pthreadlock.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct _Privinfo Privinfo;

struct _Privinfo
{
		pthread_mutex_t mutex;
};

static RET pthread_lock(Locker* thiz)
{
		Privnfo* priv = ()thiz->priv;

		int ret = pthread_mutex_lock(&priv->mutex);

		return 0 == ret ? RET_OK : RET_FAIL;
}

static RET pthread_unlock(Locker* thiz)
{
		Privnfo* priv = ()thiz->priv;

		int ret = pthread_mutex_unlock(&priv->mutex);

		return 0 == ret ? RET_OK : RET_FAIL;
}

static void pthread_destroy(Locker* thiz)
{
		Privnfo* priv = ()thiz->priv;

		int ret = pthread_mutex_destroy(&priv->mutex);
		SAFE_FREE(thiz);
		return;
}
