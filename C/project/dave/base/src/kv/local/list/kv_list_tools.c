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
#include "kv_list_slot.h"
#include "kv_log.h"

static KVSlot **
_kv_hash_to_slot(KVSlot **ppSlot, KVHash *pHash, dave_bool creat_flag)
{
	ub hash_index, hash_slot;
	KVSlot **up_ppslot;

	hash_index = 0;
	up_ppslot = NULL;

	while(hash_index < KV_SLOT_DEPTH)
	{
		hash_slot = pHash->hash_slot[hash_index];
		if(hash_slot >= KV_SLOT_NUM)
		{
			KVLOG("invalid hash_slot:%d", hash_slot);
			return NULL;
		}

		if(ppSlot[hash_slot] == NULL)
		{
			if(creat_flag == dave_false)
			{
				return NULL;
			}

			ppSlot[hash_slot] = kv_slot_malloc(up_ppslot);
		}

		if(((hash_index + 1) >= KV_SLOT_DEPTH) || ((pHash->hash_slot[hash_index + 1]) >= KV_SLOT_NUM))
		{
			return &(ppSlot[hash_slot]);
		}

		up_ppslot = &(ppSlot[hash_slot]);

		ppSlot = (KVSlot **)(ppSlot[hash_slot]->slot);

		hash_index ++;
	}

	KVLOG("invalid hash_index:%d", hash_index);

	return NULL;
}

static void
_kv_list_slot_show(KVSlot **slot)
{
	ub slot_index;

	if(slot == NULL)
		return;

	DAVELOG("%lx ", slot);
	for(slot_index=0; slot_index<KV_SLOT_NUM; slot_index++)
	{
		DAVELOG("%ld/%lx ", slot_index, slot[slot_index]);
	}
	DAVELOG("\n");
	for(slot_index=0; slot_index<KV_SLOT_NUM; slot_index++)
	{
		if(slot[slot_index] != NULL)
		{
			_kv_list_slot_show((KVSlot **)(slot[slot_index]->slot));
		}
	}
}

// ====================================================================

dave_bool
kv_is_my_key(KVData *pData, u8 *key_ptr, ub key_len)
{
	KVDEBUG("key_ptr:%x key_len:%d/%d", key_ptr, pData->key.key_len, key_len);

	if(pData->magic_data != KV_DATA_MAGIC_DATA)
	{
		KVLTRACE(60, 1, "invalid magic_data:%lx", pData->magic_data);
		return dave_false;
	}

	if(pData->key.key_len == key_len)
	{
		return dave_memcmp(pData->key.key_ptr, key_ptr, key_len);
	}

	return dave_false;
}

KVSlot *
kv_hash_to_slot(KVSlot **ppSlot, KVHash *pHash, dave_bool creat_flag)
{
	KVSlot **ppFindSlot;

	ppFindSlot = _kv_hash_to_slot(ppSlot, pHash, creat_flag);
	if(ppFindSlot == NULL)
		return NULL;

	return *ppFindSlot;
}

KVSlot **
kv_hash_to_pslot(KVSlot **ppSlot, KVHash *pHash, dave_bool creat_flag)
{
	return _kv_hash_to_slot(ppSlot, pHash, creat_flag);
}

void
kv_list_build_value_checksum(KVValue *pValue)
{
	pValue->value_checksum = pValue->value_len + ((ub)(pValue->value_ptr));
}

dave_bool
kv_list_check_value_checksum(KVValue *pValue)
{
	if(pValue->value_checksum != (pValue->value_len + ((ub)(pValue->value_ptr))))
	{
		return dave_false;
	}

	return dave_true;
}

void
__kv_list_show__(KVSlot **slot, s8 *fun, ub line)
{
	KVLOG("=============================%s:%d", fun, line);
	_kv_list_slot_show(slot);
	KVLOG("=============================");
}

#endif

