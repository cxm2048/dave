/*
* Tencent is pleased to support the open source community by making Libco available.

* Copyright (C) 2014 THL A29 Limited, a Tencent company. All rights reserved.
*
* Licensed under the Apache License, Version 2.0 (the "License"); 
* you may not use this file except in compliance with the License. 
* You may obtain a copy of the License at
*
*	http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, 
* software distributed under the License is distributed on an "AS IS" BASIS, 
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. 
* See the License for the specific language governing permissions and 
* limitations under the License.
*/

#ifndef __CO_ROUTINE_H__
#define __CO_ROUTINE_H__

#include <stdint.h>
#include <sys/poll.h>
#include <pthread.h>

//1.struct

struct stCoRoutine_t;
struct stShareStack_t;

struct stCoRoutineAttr_t
{
	int stack_size;
	stShareStack_t*  share_stack;
	stCoRoutineAttr_t()
	{
		stack_size = 128 * 1024;
		share_stack = NULL;
	}
}__attribute__ ((packed));

typedef int (*pfn_co_eventloop_t)(void *);
typedef void *(*pfn_co_routine_t)( void * );

//2.co_routine

extern "C" int 	co_create( stCoRoutine_t **co,const stCoRoutineAttr_t *attr,void *(*routine)(void*),void *arg );
extern "C" void    co_resume( stCoRoutine_t *co );
extern "C" void    co_yield( stCoRoutine_t *co );
extern "C" void    co_yield_ct(); //ct = current thread
extern "C" void    co_release( stCoRoutine_t *co );

stCoRoutine_t *co_self();

//3.specific

int 	co_setspecific( pthread_key_t key, const void *value );
void *	co_getspecific( pthread_key_t key );

//6.sync
struct stCoCond_t;

//7.share stack
stShareStack_t* co_alloc_sharestack(int iCount, int iStackSize);

//8.init envlist for hook get/set env
void co_log_err( const char *fmt,... );

#endif

