/*
 * Copyright (c) 2022 Renwei
 *
 * This is a free software; you can redistribute it and/or modify
 * it under the terms of the MIT license. See LICENSE for details.
 */

#include "base_macro.h"
#if defined(__DAVE_BASE__)
#include "dave_os.h"
#include "dave_tools.h"
#include "kv_param.h"
#include "kv_struct.h"
#include "kv_api.h"
#include "kv_local_api.h"
#include "kv_remote_api.h"
#include "kv_log.h"

#define KV_MAX_LINE 512
#define KV_ACCUACY_RANGE 30	// SECOND
#define KV_MAX_TIME 60 * 60 * 24 * 31

static inline KVTimerKeyList *
_kv_timer_key_malloc(u8 *key_ptr, ub key_len)
{
	KVTimerKeyList *pKeyList;

	if(key_len >= KV_KEY_MAX)
	{
		KVLOG("too longer key_len:%d(%d)", key_len, KV_KEY_MAX);
		return NULL;
	}

	pKeyList = dave_malloc(sizeof(KVTimerKeyList));

	pKeyList->key_len = key_len;
	dave_memcpy(pKeyList->key_data, key_ptr, key_len);
	pKeyList->key_data[key_len] = '\0';

	pKeyList->pTimerLine = NULL;

	pKeyList->up = pKeyList->next = NULL;

	return pKeyList;
}

static inline void
_kv_timer_key_free(KVTimerKeyList *pKeyList)
{
	if(pKeyList != NULL)
	{
		dave_free(pKeyList);
	}
}

static KVTimerKeyList *
_kv_timer_key_clone(KVTimerKeyList *pKeyList)
{
	KVTimerKeyList *pCloneHead, *pCloneTail, *pCloneList;

	pCloneHead = pCloneTail = NULL;

	while(pKeyList != NULL)
	{
		pCloneList = _kv_timer_key_malloc(pKeyList->key_data, pKeyList->key_len);
		if(pCloneList == NULL)
			break;

		pCloneList->pTimerLine = pKeyList->pTimerLine;

		if(pCloneHead == NULL)
		{
			pCloneHead = pCloneTail = pCloneList;
		}
		else
		{
			pCloneList->up = pCloneTail;
			pCloneTail->next = pCloneList;
			pCloneTail = pCloneList;
		}

		pKeyList =  pKeyList->next;
	}

	return pCloneHead;
}

static inline void
_kv_timer_key_clean(KVTimerKeyList *pKeyList)
{
	KVTimerKeyList *pTempList;

	while(pKeyList != NULL)
	{
		pTempList = pKeyList->next;
	
		_kv_timer_key_free(pKeyList);

		pKeyList = pTempList;
	}
}

static inline KVTimerLineList *
_kv_timer_line_malloc(ub current_times)
{
	KVTimerLineList *pLineList = dave_ralloc(sizeof(KVTimerLineList));

	pLineList->current_times = (sb)current_times;
	if(pLineList->current_times < 0)
	{
		KVLOG("get invalid times:%d/%d", pLineList->current_times, current_times);
		pLineList->current_times = 1;
	}
	pLineList->pKeyHead = pLineList->pKeyTail = NULL;
	pLineList->next = NULL;

	return pLineList;
}

static inline void
_kv_timer_line_free(KVTimerLineList *pLineList)
{
	if(pLineList != NULL)
	{
		dave_free(pLineList);
	}
}

static inline void
_kv_timer_line_clean(KVTimerLineList *pLineList, dave_bool clean_current)
{
	KVTimerLineList *pTempList;

	while(pLineList != NULL)
	{
		_kv_timer_key_clean(pLineList->pKeyHead);

		pLineList->pKeyHead = pLineList->pKeyTail = NULL;

		pTempList = pLineList->next;

		_kv_timer_line_free(pLineList);

		pLineList = pTempList;

		if(clean_current == dave_true)
			break;
	}
}

static inline KVTimerLineList *
_kv_timer_copy_line(KVTimerLineList *pSrcTimerLine, KVTimerLineList *pDstTimerLine)
{
	KVTimerKeyList *pKeyList;

	if(pSrcTimerLine == NULL)
	{
		pSrcTimerLine = pDstTimerLine;
	}
	else if(pSrcTimerLine->pKeyHead == NULL)
	{
		pSrcTimerLine->pKeyHead = pDstTimerLine->pKeyHead;
		pSrcTimerLine->pKeyTail = pDstTimerLine->pKeyTail;

		_kv_timer_line_free(pDstTimerLine);
	}
	else
	{
		if(pDstTimerLine->pKeyHead != NULL)
		{
			pSrcTimerLine->pKeyTail->next = pDstTimerLine->pKeyHead;
			pDstTimerLine->pKeyHead->up = pSrcTimerLine->pKeyTail;
			pSrcTimerLine->pKeyTail = pDstTimerLine->pKeyTail;
		}

		_kv_timer_line_free(pDstTimerLine);
	}

	pKeyList = pSrcTimerLine->pKeyHead;
	while(pKeyList != NULL)
	{
		pKeyList->pTimerLine = pSrcTimerLine;
	
		pKeyList = pKeyList->next;
	}	

	return pSrcTimerLine;
}

static inline KVTimerLineList *
_kv_timer_insert_line(KVTimerLineList *pSrcTimerLine, KVTimerLineList *pDstTimerLine)
{
	KVTimerLineList *pTempList;

	if(pSrcTimerLine == NULL)
	{
		pSrcTimerLine = pDstTimerLine;
	}
	else
	{
		pTempList = pSrcTimerLine;

		while(pTempList->next != NULL)
		{
			pTempList = pTempList->next;
		}

		pTempList->next = pDstTimerLine;
	}

	return pSrcTimerLine;
}

static inline KVTimerLineList *
_kv_timer_load_valid_line(KVTimer *pKV, ub *pout_times, s8 *fun, ub line)
{
	KVTimerLineList *timer_line;
	sb out_times;
	ub safe_counter;

	*pout_times = 0;

	timer_line = pKV->timer_line;
	if(timer_line == NULL)
		return NULL;

	out_times = timer_line->current_times;

	safe_counter = 0;

	/*
	 * 定时精度不要求那么高，在一个基础时间范围左右就可以了。
	 */
	while((timer_line != NULL) && (out_times < (sb)(pKV->out_times)))
	{
		if(timer_line->current_times < 0)
		{
			KVABNOR("invalid current_times:%d safe_counter:%d <%s:%d>",
				timer_line->current_times, safe_counter, fun, line);
			timer_line->current_times = 1;
		}

		timer_line = timer_line->next;
		if(timer_line != NULL)
		{
			if((out_times + timer_line->current_times) >= (sb)(pKV->out_times))
			{
				break;
			}
	
			out_times += timer_line->current_times;
		}

		safe_counter ++;
	}

	*pout_times = (ub)out_times;

	if((ub)out_times > pKV->out_times)
	{
		KVLOG("invalid times:%d/%d", out_times, pKV->out_times);
	}

	return timer_line;
}

static inline void
_kv_timer_copy_key(KVTimerLineList *pTimerLine, KVTimerKeyList *pTimeKey)
{
	pTimeKey->pTimerLine = pTimerLine;

	KVDEBUG("%d/%s", pTimeKey->key_len, pTimeKey->key_data);

	pTimeKey->up = pTimeKey->next = NULL;

	if(pTimerLine->pKeyHead == NULL)
	{
		pTimerLine->pKeyHead = pTimerLine->pKeyTail = pTimeKey;
	}
	else
	{
		if((pTimerLine->pKeyHead == NULL) || (pTimerLine->pKeyTail == NULL))
		{
			KVABNOR("Arithmetic error! %x/%x", pTimerLine->pKeyHead, pTimerLine->pKeyTail)
		}
		else if(pTimerLine->pKeyTail->next != NULL)
		{
			KVABNOR("Arithmetic error! the next is not NULL(%x/%x/%x/%x/%x)!",
				pTimerLine,
				pTimerLine->pKeyHead, pTimerLine->pKeyTail,
				pTimerLine->pKeyTail->next, pTimeKey);
		}

		pTimeKey->up = pTimerLine->pKeyTail;
		pTimerLine->pKeyTail->next = pTimeKey;
		pTimerLine->pKeyTail = pTimeKey;
	}
}

static inline void
_kv_timer_insert_timer_line(KVTimer *pKV, KVTimerLineList *pTimerLine)
{
	KVTimerLineList *timer_line;
	ub out_times;

	if(pTimerLine->pKeyHead == NULL)
	{
		KVABNOR("Arithmetic error!");
		return;
	}

	timer_line = _kv_timer_load_valid_line(pKV, &out_times, (s8 *)__func__, (ub)__LINE__);
	if(timer_line != NULL)
	{
		_kv_timer_copy_line(timer_line, pTimerLine);
	}
	else
	{
		pTimerLine->current_times = (sb)(pKV->out_times - out_times);
		if(pTimerLine->current_times <= 0)
		{
			KVABNOR("invalid current_times:%d", pTimerLine->current_times);
		}

		pKV->timer_line = _kv_timer_insert_line(pKV->timer_line, pTimerLine);
	}
}

static inline void
_kv_timer_insert_key_to_line(KVTimer *pKV, KVTimerKeyList *pTimeKey)
{
	KVTimerLineList *timer_line;
	ub out_times;

	pTimeKey->up = pTimeKey->next = NULL;

	timer_line = _kv_timer_load_valid_line(pKV, &out_times, (s8 *)__func__, (ub)__LINE__);
	if(timer_line != NULL)
	{
		_kv_timer_copy_key(timer_line, pTimeKey);
	}
	else
	{
		if(pKV->out_times > out_times)
		{
			timer_line = _kv_timer_line_malloc(pKV->out_times - out_times);
		}
		else
		{
			KVLOG("invalid timers:%d/%d", pKV->out_times, out_times);
			timer_line = _kv_timer_line_malloc(pKV->out_times);
		}

		pTimeKey->pTimerLine = timer_line;

		timer_line->pKeyHead = timer_line->pKeyTail = pTimeKey;

		pKV->timer_line = _kv_timer_insert_line(pKV->timer_line, timer_line);
	}
}

static inline void
_kv_timer_del_head(KVTimerLineList *my_timer_line, KVTimerKeyList *pTimeKey)
{
	if(my_timer_line->pKeyHead == pTimeKey)
	{
		if(my_timer_line->pKeyHead == my_timer_line->pKeyTail)
		{
			if(my_timer_line->pKeyTail != pTimeKey)
			{
				KVABNOR("Arithmetic error! %lx/%lx", my_timer_line->pKeyTail, pTimeKey);
			}
			my_timer_line->pKeyHead = my_timer_line->pKeyTail = NULL;
		}
		else
		{
			my_timer_line->pKeyHead = my_timer_line->pKeyHead->next;
			if(my_timer_line->pKeyHead == NULL)
			{
				KVABNOR("Arithmetic error! %lx/%lx", my_timer_line->pKeyTail, pTimeKey);
				my_timer_line->pKeyHead = my_timer_line->pKeyTail = NULL;
			}
			else
			{
				if(my_timer_line->pKeyHead->up != pTimeKey)
				{
					KVABNOR("Arithmetic error! %lx/%lx", my_timer_line->pKeyHead->up, pTimeKey);
				}
				my_timer_line->pKeyHead->up = NULL;
			}
		}
	}
}

static inline void
_kv_timer_del_tail(KVTimerLineList *my_timer_line, KVTimerKeyList *pTimeKey)
{
	if(my_timer_line->pKeyTail == pTimeKey)
	{
		if(my_timer_line->pKeyHead == my_timer_line->pKeyTail)
		{
			if(my_timer_line->pKeyHead != pTimeKey)
			{
				KVABNOR("Arithmetic error! %lx/%lx", my_timer_line->pKeyHead, pTimeKey);
			}
			my_timer_line->pKeyHead = my_timer_line->pKeyTail = NULL;
		}
		else
		{
			my_timer_line->pKeyTail = my_timer_line->pKeyTail->up;
			if(my_timer_line->pKeyTail == NULL)
			{
				KVABNOR("Arithmetic error! %lx/%lx", my_timer_line->pKeyHead, pTimeKey);
				my_timer_line->pKeyHead = my_timer_line->pKeyTail = NULL;
			}
			else
			{
				if(my_timer_line->pKeyTail->next != pTimeKey)
				{
					KVABNOR("Arithmetic error! %lx/%lx", my_timer_line->pKeyTail->next, pTimeKey);
				}
				my_timer_line->pKeyTail->next = NULL;
			}
		}
	}
}

static inline void
_kv_timer_del_mid(KVTimerLineList *my_timer_line, KVTimerKeyList *pTimeKey)
{
	if(pTimeKey->up != NULL)
	{
		((KVTimerKeyList *)(pTimeKey->up))->next = pTimeKey->next;
	}
	else
	{
		KVABNOR("Arithmetic error! %lx/%lx", pTimeKey->up, pTimeKey->next);
	}
	if(pTimeKey->next != NULL)
	{
		((KVTimerKeyList *)(pTimeKey->next))->up = pTimeKey->up;
	}
	else
	{
		KVABNOR("Arithmetic error! %lx/%lx", pTimeKey->up, pTimeKey->next);
	}
}

static inline void
_kv_timer_del(KVTimer *pKV, KVTimerKeyList *pTimeKey)
{
	KVTimerLineList *my_timer_line = pTimeKey->pTimerLine;

	KVDEBUG("%d/%s", pTimeKey->key_len, pTimeKey->key_data);

	if(my_timer_line == NULL)
	{
		KVABNOR("find Arithmetic error! up:%x next:%x",
			pTimeKey->up, pTimeKey->next);
	}
	else
	{
		KVDEBUG("pKeyHead:%x pTimeKey:%x", my_timer_line->pKeyHead, pTimeKey);

		if(my_timer_line->pKeyHead == pTimeKey)
		{
			_kv_timer_del_head(my_timer_line, pTimeKey);
			if((my_timer_line->pKeyHead != NULL) && (my_timer_line->pKeyHead->up != NULL))
			{
				KVABNOR("Arithmetic error! %lx/%lx", my_timer_line->pKeyHead, my_timer_line->pKeyHead->up);
			}
		}
		else if(my_timer_line->pKeyTail == pTimeKey)
		{
			_kv_timer_del_tail(my_timer_line, pTimeKey);
			if((my_timer_line->pKeyTail != NULL) && (my_timer_line->pKeyTail->next != NULL))
			{
				KVABNOR("Arithmetic error! %lx/%lx", my_timer_line->pKeyTail, my_timer_line->pKeyTail->next);
			}
		}
		else
		{
			_kv_timer_del_mid(my_timer_line, pTimeKey);
			if((my_timer_line->pKeyHead == NULL) || (my_timer_line->pKeyTail == NULL))
			{
				KVABNOR("Arithmetic error! %lx/%lx", my_timer_line->pKeyHead, my_timer_line->pKeyTail);
			}
		}

		pTimeKey->pTimerLine = NULL;
		pTimeKey->up = pTimeKey->next = NULL;
	}
}

static inline void
_kv_timer_add(KVTimer *pKV, KVTimerKeyList *pTimeKey)
{
	if(pTimeKey->pTimerLine != NULL)
	{
		_kv_timer_del(pKV, pTimeKey);
		pTimeKey->pTimerLine = NULL;
	}

	_kv_timer_insert_key_to_line(pKV, pTimeKey);
}

static KVTimerKeyList *
_kv_timer_out_clone_notify(KV *pKV, dave_bool *timer_out_flag)
{
	KVTimerLineList *pLineList = pKV->kv_timer.timer_line;

	if(pLineList == NULL)
		return NULL;

	if((-- pLineList->current_times) > 0)
		return NULL;

	*timer_out_flag = dave_true;

	return _kv_timer_key_clone(pLineList->pKeyHead);
}

static inline void
_kv_timer_out_notify_user(KV *pKV, KVTimerKeyList *pCloneKeyHead)
{
	kv_out_callback callback_fun = pKV->kv_timer.callback_fun;
	KVTimerKeyList *pNotifyKeyHead;

	if(callback_fun == NULL)
		return;

	pNotifyKeyHead = pCloneKeyHead;

	while(pNotifyKeyHead != NULL)
	{
		callback_fun(pKV, (s8 *)(pNotifyKeyHead->key_data));
	
		pNotifyKeyHead = pNotifyKeyHead->next;
	}
}

static inline void
_kv_timer_out_update_line(KVTimer *pKV)
{
	KVTimerLineList *current_timer_line;

	if(pKV->timer_line == NULL)
		return;

	/*
	 * 保存当前时间线，进入下一个时间线。
	 */
	current_timer_line = pKV->timer_line;
	pKV->timer_line = current_timer_line->next;
	current_timer_line->next = NULL;

	if(current_timer_line->pKeyHead == NULL)
	{
		_kv_timer_line_clean(current_timer_line, dave_true);
	}
	else
	{
		_kv_timer_insert_timer_line(pKV, current_timer_line);
	}
}

static void
_kv_timer_out(KV *pKV)
{
	KVTimerKeyList *pCloneKeyHead;
	dave_bool timer_out_flag = dave_false;

	if(pKV->kv_timer.timer_line == NULL)
		return;

	pCloneKeyHead = NULL;

	SAFEZONEv5R(pKV->kv_pv, pCloneKeyHead = _kv_timer_out_clone_notify(pKV, &timer_out_flag););

	_kv_timer_out_notify_user(pKV, pCloneKeyHead);

	if(timer_out_flag == dave_true)
	{
		/*
		 * 在经历过用户侧的提醒后（_kv_timer_out_notify_user）
		 * 如果用户未完全删除KV记录（current_timer_line时间线有KVTimerKeyList），
		 * 那么把KV记录加入下一轮超时时间线。
		 */
		SAFEZONEv5W(pKV->kv_pv, _kv_timer_out_update_line(&(pKV->kv_timer)););
	}

	_kv_timer_key_clean(pCloneKeyHead);
}

static inline ErrCode
_kv_timer_recycle_key_kv(void *kv, s8 *key)
{
	KVTimerKeyList *pTimeKey = base_kv_inq_key_ptr(kv, key);

	if(pTimeKey == NULL)
		return ERRCODE_empty_data;

	if(base_kv_del_bin_ptr(kv, pTimeKey->key_data, pTimeKey->key_len) == NULL)
	{
		KVABNOR("invalid del bin option! key_len:%d", pTimeKey->key_len);
		return ERRCODE_invalid_option;
	}

	return ERRCODE_OK;
}

static inline ub
_kv_timer_base_time(KV *pKV, ub out_second)
{
	ub accuacy_range, time_line, base_time;

	accuacy_range = out_second / KV_ACCUACY_RANGE;
	if(accuacy_range == 0)
	{
		accuacy_range = 1;
	}
	else if(accuacy_range > KV_ACCUACY_RANGE)
	{
		accuacy_range = KV_ACCUACY_RANGE;
	}

	time_line = 1;
	base_time = accuacy_range + 1;
	while((time_line < KV_MAX_LINE) && (base_time > accuacy_range))
	{
		base_time = out_second / (time_line ++);
		if(base_time == 0)
		{
			base_time = 1;
		}
	}

	return base_time;
}

static inline void
_kv_timer_init(s8 *thread_name, s8 *name, KV *pKV, ub out_second, kv_out_callback callback_fun)
{
	s8 name_buffer[256];

	if((out_second == 0) || (callback_fun == NULL))
	{
		return;
	}
	if(out_second > KV_MAX_TIME)
	{
		KVDEBUG("%s creat %s has too large out_second:%d(%d), creat failed!",
			thread_name, name, out_second, KV_MAX_TIME);
		return;
	}
	if(pKV->kv_timer.timer_id != INVALID_TIMER_ID)
	{
		KVLOG("%s repeat init %s, why?", thread_name, name);
		return;
	}

	dave_snprintf(name_buffer, sizeof(name_buffer), "kvtimer_%s", name);

	/*
	 * KV_BASH_TIME 可以改造成安装out_second来自动配置
	 * 比如如果精度允许，只考虑有16个时间序列（KVTimerLineList）。
	 */
	pKV->kv_timer.out_second = out_second;
	pKV->kv_timer.base_timer = _kv_timer_base_time(pKV, out_second);
	pKV->kv_timer.out_times = out_second / pKV->kv_timer.base_timer;
	if(pKV->kv_timer.out_times == 0)
	{
		pKV->kv_timer.out_times = 1;
	}
	pKV->kv_timer.callback_fun = callback_fun;
	pKV->kv_timer.timer_id = base_timer_param_creat(name_buffer, kv_timer, pKV, pKV->kv_timer.base_timer * 1000);
	pKV->kv_timer.timer_line = NULL;
	pKV->kv_timer.key_kv = __base_kv_malloc__(dave_false, name_buffer, KVAttrib_list, 0, NULL, (s8 *)__func__, (ub)__LINE__);

	KVDEBUG("thread:%s out_second:%d base_time:%d out_times:%d timer_name:%s/%s timer_id:%d",
		thread_name,
		out_second, pKV->kv_timer.base_timer, pKV->kv_timer.out_times,
		name_buffer, name, pKV->kv_timer.timer_id);
}

static inline void
_kv_timer_exit(KVTimer *pKV)
{
	if(pKV->timer_id != INVALID_TIMER_ID)
	{
		base_timer_die(pKV->timer_id);
		pKV->timer_id = INVALID_TIMER_ID;
	}

	_kv_timer_line_clean(pKV->timer_line, dave_false);

	if(pKV->key_kv != NULL)
	{
		__base_kv_free__(dave_false, pKV->key_kv, _kv_timer_recycle_key_kv, (s8 *)__func__, (ub)__LINE__);
		pKV->key_kv = NULL;
	}

	pKV->out_times = 0;
	pKV->callback_fun = NULL;
	pKV->timer_line = NULL;
	pKV->key_kv = NULL;
}

static inline ub
_kv_timer_info(KVTimer *pKV, s8 *info_ptr, ub info_len)
{
	ub info_index = 0;
	KVTimerLineList *timer_line;
	KVTimerKeyList *timer_key;

	info_index += dave_snprintf(&info_ptr[info_index], info_len-info_index,
		"out_second:%d base_timer:%d out_times:%d\n", pKV->out_second, pKV->base_timer, pKV->out_times);
	timer_line = pKV->timer_line;
	while(timer_line != NULL)
	{
		info_index += dave_snprintf(&info_ptr[info_index], info_len-info_index,
			"\tcurrent_times:%d\n", timer_line->current_times);
		timer_key = timer_line->pKeyHead;
		while(timer_key != NULL)
		{
			info_index += dave_snprintf(&info_ptr[info_index], info_len-info_index,
				"\tkey:%d/%s\n", timer_key->key_len, timer_key->key_data);
			timer_key = timer_key->next;
		}
		info_index += dave_snprintf(&info_ptr[info_index], info_len-info_index , "\n");
		timer_line = timer_line->next;
	}

	return info_index;
}

// ====================================================================

void
kv_timer_init(KV *pKV, ub out_second, kv_out_callback callback_fun)
{
	dave_memset(&(pKV->kv_timer), 0x00, sizeof(KVTimer));

	pKV->kv_timer.out_times = 0;
	pKV->kv_timer.inq_update_timer = dave_false;
	pKV->kv_timer.callback_fun = NULL;
	pKV->kv_timer.timer_id = INVALID_TIMER_ID;
	pKV->kv_timer.timer_line = NULL;
	pKV->kv_timer.key_kv = NULL;

	SAFEZONEv5W(pKV->kv_pv, _kv_timer_init(pKV->thread_name, pKV->name, pKV, out_second, callback_fun););
}

void
kv_timer_exit(KV *pKV)
{
	SAFEZONEv5W( pKV->kv_pv, _kv_timer_exit(&(pKV->kv_timer)); );

	dave_memset(&(pKV->kv_timer), 0x00, sizeof(KVTimer));
}

void
kv_timer_add(KV *pKV, u8 *key_ptr, ub key_len)
{
	KVTimerKeyList *pMemTimeKey, *pTimeKey;

	if((pKV->kv_timer.out_times == 0) || (pKV->kv_timer.callback_fun == NULL))
		return;

	pMemTimeKey = _kv_timer_key_malloc(key_ptr, key_len);

	SAFEZONEv5W(pKV->kv_pv, {

		pTimeKey = base_kv_inq_bin_ptr(pKV->kv_timer.key_kv, key_ptr, key_len);
		if(pTimeKey == NULL)
		{
			pTimeKey = pMemTimeKey;
			pMemTimeKey = NULL;

			base_kv_add_bin_ptr(pKV->kv_timer.key_kv, key_ptr, key_len, pTimeKey);
		}

		if(pTimeKey != NULL)
		{
			_kv_timer_add(&(pKV->kv_timer), pTimeKey);
		}

	} );

	if(pMemTimeKey != NULL)
	{
		_kv_timer_key_free(pMemTimeKey);
	}
}

void
kv_timer_inq(KV *pKV, u8 *key_ptr, ub key_len)
{
	KVTimerKeyList *pTimeKey;

	if((pKV->kv_timer.out_times == 0) || (pKV->kv_timer.callback_fun == NULL))
		return;

	pTimeKey = base_kv_inq_bin_ptr(pKV->kv_timer.key_kv, key_ptr, key_len);
	if(pTimeKey != NULL)
	{
		SAFEZONEv5W(pKV->kv_pv, _kv_timer_add(&(pKV->kv_timer), pTimeKey); );
	}
}

void
kv_timer_del(KV *pKV, u8 *key_ptr, ub key_len, s8 *fun, ub line)
{
	KVTimerKeyList *pTimeKey;

	if((key_ptr == NULL) || (key_len == 0))
		return;

	if((pKV->kv_timer.out_times == 0) || (pKV->kv_timer.callback_fun == NULL))
		return;

	pTimeKey = base_kv_del_bin_ptr(pKV->kv_timer.key_kv, key_ptr, key_len);
	if(pTimeKey != NULL)
	{
		SAFEZONEv5W(pKV->kv_pv, _kv_timer_del(&(pKV->kv_timer), pTimeKey); );

		_kv_timer_key_free(pTimeKey);
	}
	else
	{
		KVTRACE("%s can't find key len:%d/! <%s:%d>", pKV->name, key_len, fun, line);
	}
}

void
kv_timer_out(KV *pKV)
{
	_kv_timer_out(pKV);
}

ub
kv_timer_info(KV *pKV, s8 *info_ptr, ub info_len)
{
	return _kv_timer_info(&(pKV->kv_timer), info_ptr, info_len);
}

#endif

