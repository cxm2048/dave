/*
 * Copyright (c) 2022 Renwei
 *
 * This is a free software; you can redistribute it and/or modify
 * it under the terms of the MIT license. See LICENSE for details.
 */

#include "base_macro.h"
#ifdef __DAVE_BASE__
#include "dave_base.h"
#include "dave_tools.h"
#include "cfg_log.h"

#define CONFIG_FILE_VALUE_BUF_NUM (4)
#define CONFIG_FILE_VALUE_BUF_MAX (16384)
#define CONFIG_FILE_MAGIC_DATA (0xffffffff3b4a12ee)
#define CONFIG_FILE_NAME_LENGTH (32)

#define CONFIG_OPTION_MAX (128)

typedef enum {
	ConfigFileType_ini = 0,
	ConfigFileType_mdf,
	ConfigFileType_bak,
	ConfigFileType_max
} ConfigFileType;

typedef struct {
	ub magic_data;
	ub value_len;
	ub crc_check;
	ub type;
	s8 name[CONFIG_FILE_NAME_LENGTH];
} ConfigFileValueHeadData;

typedef struct {
	ConfigFileValueHeadData data;
	u8 reserve[256 - sizeof(ConfigFileValueHeadData)];
} ConfigFileValueHead;

typedef struct {
	ConfigFileValueHead head;
	u8 value[CONFIG_FILE_VALUE_BUF_MAX];
} ConfigFileValue;

typedef struct {
	ub use_times;
	ConfigFileValue value;
} ConfigValueBuf;

#define BASE_CFG_NAME "config"

static volatile sb _config_init_ = 0;
static DaveLock _config_option_pv;
static ConfigValueBuf _value_buffer[CONFIG_FILE_VALUE_BUF_NUM];

static void
_base_cfg_reset_buffer(ConfigValueBuf *pBuf)
{
	dave_memset(pBuf, 0x00, sizeof(ConfigValueBuf));

	pBuf->use_times = 0;
}

static void
_base_cfg_reset_all_buffer(void)
{
	ub buffer_index;

	for(buffer_index=0; buffer_index<CONFIG_FILE_VALUE_BUF_NUM; buffer_index++)
	{
		_base_cfg_reset_buffer(&_value_buffer[buffer_index]);
	}
}

static void
_base_cfg_value_insert_end_flag(ConfigFileValue *pValue)
{
	u8 end_flag[5] = {0xff, 'E', 'N', 'D', 0xff};
	ub value_len, index;

	value_len = pValue->head.data.value_len;

	for(index=0; index<5; index++)
	{
		if(value_len >= CONFIG_FILE_VALUE_BUF_MAX)
		{
			break;
		}

		pValue->value[value_len ++] = end_flag[index];
	}
}

static void
_base_cfg_des_key(u8 *key)
{
	ub key_index;

	key_index = 0;

	key[key_index ++] = 0x35;
	key[key_index ++] = 0x19;
	key[key_index ++] = 0x87;
	key[key_index ++] = 0x65;
	key[key_index ++] = 0x34;
	key[key_index ++] = 0xae;
	key[key_index ++] = 0x9b;
	key[key_index ++] = 0xab;
}

static ConfigFileValue *
_base_cfg_insert_buffer(s8 *name, dave_bool find_new, ConfigFileValue *pValue)
{
	ub buffer_index, name_index, safe_counter;
	ConfigValueBuf *pBuf;
	ub smallest_use_times, smallest_index;

	buffer_index = name_index = 0;

	while((name[name_index] != '\0') && (name_index < 128))
	{
		buffer_index *= 10;

		buffer_index += name[name_index ++];
	}

	buffer_index %= CONFIG_FILE_VALUE_BUF_NUM;

	if(name_index >= 128)
	{
		CFGABNOR("config name:%s too long!", name);
	}

	pBuf = NULL;
	smallest_use_times = 0xffffffff;
	smallest_index = CONFIG_FILE_VALUE_BUF_NUM;

	if(find_new == dave_false)
	{
		for(safe_counter=0; safe_counter<CONFIG_FILE_VALUE_BUF_NUM; safe_counter++)
		{
			if(buffer_index >= CONFIG_FILE_VALUE_BUF_NUM)
			{
				buffer_index = 0;
			}

			if(_value_buffer[buffer_index].use_times > 0)
			{
				if(t_stdio_strcmp(_value_buffer[buffer_index].value.head.data.name, name) == dave_true)
				{
					pBuf = &_value_buffer[buffer_index];

					break;
				}
			}

			buffer_index ++;
		}
	}
	else
	{
		for(safe_counter=0; safe_counter<CONFIG_FILE_VALUE_BUF_NUM; safe_counter++)
		{
			if(buffer_index >= CONFIG_FILE_VALUE_BUF_NUM)
			{
				buffer_index = 0;
			}

			if((_value_buffer[buffer_index].use_times == 0)
				|| (t_stdio_strcmp(_value_buffer[buffer_index].value.head.data.name, name) == dave_true))
			{
				pBuf = &_value_buffer[buffer_index];

				break;
			}
			else
			{
				if(smallest_use_times > _value_buffer[buffer_index].use_times)
				{
					smallest_use_times = _value_buffer[buffer_index].use_times;
					smallest_index = buffer_index;
				}
			}

			buffer_index ++;
		}

		if(pBuf == NULL)
		{
			if(smallest_index < CONFIG_FILE_VALUE_BUF_NUM)
			{
				pBuf = &_value_buffer[smallest_index];

				pBuf->use_times = 0;
			}
		}
	}

	if(pBuf != NULL)
	{
		CFGDEBUG("name:%s times:%d find:%s replace:%s<%s>",
			name, pBuf->use_times, find_new==dave_true?"new":"old",
			smallest_index>=CONFIG_FILE_VALUE_BUF_NUM?"no":"yes",
			pBuf->value.head.data.name);

		pBuf->use_times ++;

		if(pValue != NULL)
		{
			pBuf->value = *pValue;
		}

		return &pBuf->value;
	}
	else
	{
		return NULL;
	}
}

static void
_base_cfg_name_ini(s8 *name, s8 *file_name)
{
	dave_sprintf(file_name, "config/%s/ini", name);
}

static void
_base_cfg_name_mdf(s8 *name, s8 *file_name)
{
	dave_sprintf(file_name, "config/%s/mdf", name);
}

static void
_base_cfg_name_bak(s8 *name, s8 *file_name)
{
	dave_sprintf(file_name, "config/%s/bak", name);
}

static sb
_base_cfg_open(s8 *dir, s8 *name, ConfigFileType type)
{
	s8 file_name[1024], file_path[1024];
	sb file_id;
	FileOptFlag flag;

	switch(type)
	{
		case ConfigFileType_ini:
				_base_cfg_name_ini(name, file_name);
			break;
		case ConfigFileType_mdf:
				_base_cfg_name_mdf(name, file_name);
			break;
		case ConfigFileType_bak:
				_base_cfg_name_bak(name, file_name);
			break;
		default:
				file_name[0] = '\0';
			break;
	}

	if(file_name[0] != '\0')
	{
		if(dir == NULL)
		{
			flag = CREAT_READ_WRITE_FLAG;
			dave_snprintf(file_path, sizeof(file_path), "%s", file_name);
		}
		else
		{
			flag = DIRECT_FLAG | CREAT_READ_WRITE_FLAG;
			dave_snprintf(file_path, sizeof(file_path), "%s/%s", dir, file_name);
		}

		file_id = dave_os_file_open(flag, file_path);
	}
	else
	{
		file_id = -1;
	}

	if(file_id < 0)
	{
		CFGABNOR("open file:%s<%s> failed:%d!", file_name, name, file_id);
	}

	return file_id;
}

static void
_base_cfg_close(sb file_id)
{
	if(file_id >= 0)
	{
		dave_os_file_close(file_id);
	}
}

static ErrCode
_base_cfg_read(sb file_id, ConfigFileValue *pValue)
{
	sb load_length, decode_length;
	u32 crc_check;
	u8 des_key[DAVE_DES_KEY_LEN];

	if(file_id < 0)
	{
		return ERRCODE_file_open_failed;
	}

	load_length = dave_os_file_load(file_id, 0, sizeof(ConfigFileValue), (u8 *)(pValue));
	if(load_length <= sizeof(ConfigFileValueHead))
	{
		CFGDEBUG("empty file:%d/%d", load_length, sizeof(ConfigFileValueHead));
		return ERRCODE_invalid_file;
	}

	if(pValue->head.data.magic_data != CONFIG_FILE_MAGIC_DATA)
	{
		CFGABNOR("invalid magic:%x", pValue->head.data.magic_data);
		return ERRCODE_invalid_magic;
	}

	if((pValue->head.data.value_len == 0) || (pValue->head.data.value_len > CONFIG_FILE_VALUE_BUF_MAX))
	{
		CFGABNOR("invalid length:%d", pValue->head.data.value_len);
		return ERRCODE_Invalid_data;
	}

	if(pValue->head.data.type >= ConfigFileType_max)
	{
		CFGABNOR("invalid type:%d", pValue->head.data.type);
	}

	_base_cfg_des_key(des_key);

	decode_length = dave_des_decode(des_key, DAVE_DES_KEY_LEN, pValue->value, pValue->head.data.value_len, dave_true);

	crc_check = t_crypto_crc32(pValue->value, decode_length);
	if((ub)crc_check != pValue->head.data.crc_check)
	{
		CFGABNOR("name:%s load_length:%d value_length:%d decode_length:%d invalid crc:%x,%x",
			pValue->head.data.name, load_length, pValue->head.data.value_len, decode_length,
			crc_check, pValue->head.data.crc_check);

		return ERRCODE_Invalid_data_crc_check;
	}

	pValue->head.data.value_len = decode_length;

	return ERRCODE_OK;
}

static ErrCode
_base_cfg_write(sb file_id, ConfigFileValue *pValue)
{
	ErrCode ret = ERRCODE_OK;
	s8 *back_value;
	ub back_value_len;
	u8 des_key[DAVE_DES_KEY_LEN];
	sb save_length;

	if(file_id < 0)
	{
		return ERRCODE_file_open_failed;
	}

	if(pValue->head.data.value_len > CONFIG_FILE_VALUE_BUF_MAX)
	{
		CFGABNOR("invalid length:%d", pValue->head.data.value_len);
		return ERRCODE_Invalid_data;
	}

	back_value = dave_malloc(pValue->head.data.value_len);
	dave_memcpy(back_value, pValue->value, pValue->head.data.value_len);
	back_value_len = pValue->head.data.value_len;

	pValue->head.data.magic_data = CONFIG_FILE_MAGIC_DATA;

	pValue->head.data.crc_check = (ub)t_crypto_crc32(pValue->value, pValue->head.data.value_len);

	if(pValue->head.data.type >= ConfigFileType_max)
	{
		CFGABNOR("invalid type:%d", pValue->head.data.type);
	}

	_base_cfg_des_key(des_key);

	pValue->head.data.value_len = dave_des_encode(des_key, DAVE_DES_KEY_LEN, pValue->value, pValue->head.data.value_len, dave_true);

	if(pValue->head.data.value_len > CONFIG_FILE_VALUE_BUF_MAX)
	{
		CFGABNOR("value length<%d> too long!", pValue->head.data.value_len);

		pValue->head.data.value_len = CONFIG_FILE_VALUE_BUF_MAX;
	}

	_base_cfg_value_insert_end_flag(pValue);

	save_length = dave_os_file_save(file_id, 0, sizeof(ConfigFileValue), (u8 *)pValue);

	if(save_length != sizeof(ConfigFileValue))
	{
		CFGABNOR("save file failed:%d,%d", save_length, sizeof(ConfigFileValue));
	}

	pValue->head.data.value_len = back_value_len;
	dave_memcpy(pValue->value, back_value, pValue->head.data.value_len);

	dave_free(back_value);

	return ret;
}

static ErrCode
_base_cfg_read_with_type(s8 *dir, s8 *name, ConfigFileValue *pValue, ConfigFileType type)
{
	ErrCode ret;
	sb file_id;

	file_id = _base_cfg_open(dir, name, type);

	if(file_id < 0)
	{
		CFGABNOR("file:%s open failed:%d!", name, file_id);
		return ERRCODE_file_open_failed;
	}

	ret = _base_cfg_read(file_id, pValue);

	if(ret == ERRCODE_OK)
	{
		if(pValue->head.data.type != (ub)type)
		{
			CFGABNOR("type:%d,%d mismatch!", pValue->head.data.type, type);
		}
	}

	_base_cfg_close(file_id);

	return ret;
}

static ErrCode
_base_cfg_write_with_type(s8 *dir, s8 *name, ConfigFileValue *pValue, ConfigFileType type)
{
	ErrCode ret;
	sb file_id;

	file_id = _base_cfg_open(dir, name, type);

	if(file_id < 0)
	{
		CFGABNOR("file:%s open failed:%d!", name, file_id);
		return ERRCODE_file_open_failed;
	}

	pValue->head.data.type = (ub)type;

	dave_strcpy(pValue->head.data.name, name, CONFIG_FILE_NAME_LENGTH);

	ret = _base_cfg_write(file_id, pValue);

	_base_cfg_close(file_id);

	return ret;
}

static ErrCode
_base_cfg_setup(s8 *dir, s8 *name, ConfigFileValue *pValue, ErrCode ret)
{
	ErrCode setup_ret;

	setup_ret = _base_cfg_write_with_type(dir, name, pValue, ConfigFileType_ini);

	if(setup_ret != ERRCODE_OK)
		return setup_ret;

	setup_ret = _base_cfg_write_with_type(dir, name, pValue, ConfigFileType_mdf);

	if(setup_ret != ERRCODE_OK)
		return setup_ret;

	if(ret == ERRCODE_invalid_file)
	{
		setup_ret = _base_cfg_write_with_type(dir, name, pValue, ConfigFileType_bak);
	}

	return setup_ret;
}

static ErrCode
_base_cfg_load(s8 *dir, s8 *name, ConfigFileValue *pValue)
{
	ErrCode ret;

	ret = _base_cfg_read_with_type(dir, name, pValue, ConfigFileType_ini);

	if(ret == ERRCODE_invalid_file)
	{
		return ret;
	}

	if(ret != ERRCODE_OK)
	{
		CFGABNOR("ini config file:%s was destroyed! <%s>", name, errorstr(ret));

		ret = _base_cfg_read_with_type(dir, name, pValue, ConfigFileType_mdf);

		if(ret == ERRCODE_OK)
		{
			_base_cfg_write_with_type(dir, name, pValue, ConfigFileType_ini);
		}
		else
		{
			CFGABNOR("mdf config file:%s was destroyed! <%s>", name, errorstr(ret));

			ret = _base_cfg_read_with_type(dir, name, pValue, ConfigFileType_bak);

			if(ret == ERRCODE_OK)
			{
				_base_cfg_write_with_type(dir, name, pValue, ConfigFileType_ini);

				_base_cfg_write_with_type(dir, name, pValue, ConfigFileType_mdf);
			}
			else
			{
				CFGABNOR("all config file:%s was destroyed! <%s>", name, errorstr(ret));
			}
		}
	}

	return ret;
}

static ErrCode
__base_cfg_set(s8 *dir, s8 *name, u8 *value_ptr, ub value_len)
{
	ConfigFileValue *pValue;
	dave_bool from_buffer;
	ErrCode ret = ERRCODE_OK;

	pValue = _base_cfg_insert_buffer(name, dave_false, NULL);
	if(pValue == NULL)
	{
		pValue = dave_ralloc(sizeof(ConfigFileValue));

		from_buffer = dave_false;
	}
	else
	{
		from_buffer = dave_true;
	}

	ret = _base_cfg_read_with_type(dir, name, pValue, ConfigFileType_ini);

	if(value_len > CONFIG_FILE_VALUE_BUF_MAX)
	{
		CFGABNOR("too length:%d/%d", value_len, CONFIG_FILE_VALUE_BUF_MAX);
		value_len = CONFIG_FILE_VALUE_BUF_MAX;
	}

	pValue->head.data.value_len = value_len;
	dave_strcpy(pValue->head.data.name, name, CONFIG_FILE_NAME_LENGTH);
	dave_memcpy(pValue->value, value_ptr, value_len);

	if(ret != ERRCODE_OK)
	{
		CFGDEBUG("name:%s ret:%s", name, errorstr(ret));

		ret = _base_cfg_setup(dir, name, pValue, ret);
	}
	else
	{
		ret = _base_cfg_write_with_type(dir, name, pValue, ConfigFileType_mdf);

		if(ret == ERRCODE_OK)
		{
			ret = _base_cfg_write_with_type(dir, name, pValue, ConfigFileType_ini);
		}
	}

	if(from_buffer == dave_false)
	{
		_base_cfg_insert_buffer(name, dave_true, pValue);

		dave_free(pValue);
	}

	return ret;
}

static ErrCode
__base_cfg_get(s8 *dir, s8 *name, u8 *value_ptr, ub *value_len)
{
	ConfigFileValue *pValue;
	dave_bool from_buffer;
	ErrCode ret = ERRCODE_OK;

	pValue = _base_cfg_insert_buffer(name, dave_false, NULL);
	if(pValue == NULL)
	{
		pValue = dave_ralloc(sizeof(ConfigFileValue));

		from_buffer = dave_false;

		ret = _base_cfg_load(dir, name, pValue);
	}
	else
	{
		from_buffer = dave_true;

		ret = ERRCODE_OK;
	}

	if(ret == ERRCODE_OK)
	{
		if((value_ptr != NULL) && (value_len != NULL) && (*value_len > 0))
		{
			if(*value_len >= pValue->head.data.value_len)
			{
				*value_len = pValue->head.data.value_len;
			}

			dave_memcpy(value_ptr, pValue->value, *value_len);
		}
	}
	else
	{
		if((value_ptr != NULL) && (value_len != NULL) && (*value_len > 0))
		{
			dave_memset(value_ptr, 0x00, *value_len);
			*value_len = 0;
		}
	}

	if(from_buffer == dave_false)
	{
		_base_cfg_insert_buffer(name, dave_true, pValue);

		dave_free(pValue);
	}

	return ret;
}

static inline void
_base_cfg_booting(void)
{
	t_lock_spin(NULL);

	if(_config_init_ != 0x89807abcd)
	{
		_config_init_ = 0x89807abcd;

		dave_lock_reset(&_config_option_pv);

		_base_cfg_reset_all_buffer();
	}

	t_unlock_spin(NULL);
}

static void
_base_cfg_update(s8 *name, u8 *value_ptr, ub value_len)
{
	CFGUpdate *pUpdate = thread_msg(pUpdate);

	dave_strcpy(pUpdate->cfg_name, name, sizeof(pUpdate->cfg_name));
	if(value_len > sizeof(pUpdate->cfg_value))
	{
		CFGABNOR("name:%s has value_len:%d/%d is long!", name, value_len, sizeof(pUpdate->cfg_value));
		value_len = sizeof(pUpdate->cfg_value);
	}
	pUpdate->cfg_length = value_len;
	dave_memcpy(pUpdate->cfg_value, value_ptr, value_len);

	broadcast_total(MSGID_CFG_UPDATE, pUpdate);
}

static ErrCode
_base_cfg_set(s8 *dir, s8 *name, u8 *value_ptr, ub value_len)
{
	ErrCode ret = ERRCODE_OK;

	if((name == NULL) || (value_ptr == NULL))
	{
		CFGABNOR("invalid ptr:%x,%x", name, value_ptr);
		return ERRCODE_Invalid_parameter;
	}

	if((value_len == 0) || (value_len > CONFIG_FILE_VALUE_BUF_MAX))
	{
		CFGABNOR("name:%s invalid value_len:%d", name, value_len);
		return ERRCODE_Invalid_parameter;
	}

	_base_cfg_booting();

	SAFEZONEv3(_config_option_pv, {

		ret = __base_cfg_set(dir, name, value_ptr, value_len);

	} );

	if(ret == ERRCODE_OK)
	{
		_base_cfg_update(name, value_ptr, value_len);
	}

	return ret;
}

static ub
_base_cfg_get(s8 *dir, s8 *name, u8 *value_ptr, ub value_len)
{
	ErrCode ret = ERRCODE_OK;

	_base_cfg_booting();

	SAFEZONEv3(_config_option_pv, {

		ret = __base_cfg_get(dir, name, value_ptr, &value_len);

	} );

	if(ret != ERRCODE_OK)
	{
		CFGDEBUG("name:%s value_len:%d failed:%s!", name, value_len, errorstr(ret));
		return 0;
	}

	return value_len;
}

// =====================================================================

ErrCode
base_cfg_dir_set(s8 *dir, s8 *name, u8 *value_ptr, ub value_len)
{
	return _base_cfg_set(dir, name, value_ptr, value_len);
}

dave_bool
base_cfg_dir_get(s8 *dir, s8 *name, u8 *value_ptr, ub value_len)
{
	ub get_len;

	get_len = _base_cfg_get(dir, name, value_ptr, value_len);
	if(get_len < value_len)
		value_ptr[get_len] = '\0';
	if(get_len == 0)
		return dave_false;

	return dave_true;
}

ErrCode
base_cfg_set_ub(s8 *cfg_name, ub ub_value)
{
	s8 value_ptr[128];
	ub value_len;

	value_len = t_a2b_digital_to_string(value_ptr, sizeof(value_ptr), ub_value);

	return base_cfg_set(cfg_name, (u8 *)value_ptr, value_len);
}

ub
base_cfg_get_ub(s8 *cfg_name)
{
	s8 value_ptr[128];
	ub ub_data;

	if(base_cfg_get(cfg_name, (u8 *)value_ptr, sizeof(value_ptr)) == dave_true)
	{
		ub_data = t_a2b_string_to_digital(value_ptr);
	}
	else
	{
		ub_data = 0;
	}

	return ub_data;
}

#endif

