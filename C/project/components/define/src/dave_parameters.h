/*
 * Copyright (c) 2022 Renwei
 *
 * This is a free software; you can redistribute it and/or modify
 * it under the terms of the MIT license. See LICENSE for details.
 */

#ifndef __DAVE_PARAMETERS_H__
#define __DAVE_PARAMETERS_H__

#define DAVE_MAC_ADDR_LEN (6)

#define DAVE_IP_V4_ADDR_LEN (4)

#define DAVE_IP_V6_ADDR_LEN (16)

#define DAVE_SIP_URI_MAX_LEN (64)

#define DAVE_URL_LEN (128)

#define DAVE_TYPE_LEN (32)

#define DAVE_BIRTH_LEN (32)

#define DAVE_DOMAIN_LEN (128)

#define DAVE_FETCH_ROW_TABLE (512)

#define DAVE_REGEXP_LEN (128)

#define DAVE_REGEXP_MAX (32)

#define DAVE_AES_KEY_LEN (8)

#define DAVE_DES_KEY_LEN (8)

#define DAVE_MD5_HASH_LEN (16)

#define DAVE_MD5_HASH_STR_LEN (DAVE_MD5_HASH_LEN * 2 + 1)

#define DAVE_SHA1_HASH_LEN (20)

#define DAVE_IP_STR_MAX_LEN (128)

#define DAVE_BILLING_SMS_SERVICE_ID (128)

#define DAVE_BILLING_SMS_PHYSICS_ID (128)

#define DAVE_BILLING_SMS_EVENT_ID (64)

#define DAVE_BILLING_SMS_USER_ID (64)

#define DAVE_BILLING_MCC_MNC (64)

#define DAVE_BILLING_SMS_ORDER_BS (128)

#define DAVE_BILLING_SMS_NODE_NAME (128)

#define DAVE_BILLING_SMS_STR_LENGTH (256)

#define DAVE_BILLING_SMS_RULES_LENGTH (64)

#define DAVE_BILLING_SMS_DB_ADD   (0)

#define DAVE_BILLING_SMS_DB_INQ   (1)

#define DAVE_BILLING_SMS_DB_UPDATE (2)

#define DAVE_BILLING_SMS_DB_ADD_RULES (3)

#define DAVE_BILLING_SMS_DB_INQ_TABLE (4)

#define DAVE_BILLING_SMS_EARLY_WARNING_PUSH (5)

#define DAVE_BILLING_SMS_DB_ASYNC_CHGUSER_ADD (6)

#define DAVE_BILLING_SMS_DB_COUNT_INQ   (7)

#define DAVE_BILLING_SMS_DB_POSTPAIDMAX     (8)

#define DAVE_BILLING_SMS_DB_RULES_ARRAY_INQ (9)

#define DAVE_BILLING_SMS_DB_FIND_BILLING    (10)

#define DAVE_BILLING_SMS_DB_POSTPAIDMAX_TIMER (11)

#define DAVE_BUSINESS_AUDIO_DB_ADD_RULES (12)

#define DAVE_BILLING_SMS_DB_CLEAR           (0)

#define DAVE_BILLING_SMS_DB_SAVE            (1)

#define DAVE_BILLING_SMS_DB_USER_PUSH       (2)

#define DAVE_BILLING_SMS_DB_RULES_PUSH      (3)

#define DAVE_BILLING_SMS_DB_ADDRESS_PUSH    (4)

#define DAVE_BILLING_DB_CDR_INIT            (5)

#define DAVE_BILLING_DB_CDR_REG             (6)

#define DAVE_BILLING_SMS_DB_PUSH_USER_INFO  (7)

#define DAVE_BILLING_SMS_DB_PUSH_RULES_INFO (8)

#define DAVE_BILLING_SMS_DB_RULES_ADD_CALLBACK      (10)

#define DAVE_BILLING_SMS_DB_CHGUSER_ADD_CALLBACK    (11)

#define DAVE_BILLING_SMS_MEM_ADDRESS_EFFECTIVENESS  (12)

#define DAVE_BILLING_SMS_DB_USER_TABLE_INQ          (13)

#define DAVE_BILLING_SMS_DB_BILLING_BILLING_INFO    (14)

#define DAVE_BILLING_SMS_DB_NOT_ENOUGH              (2)

#define DAVE_BILLING_TOOLS_TYPE_INTERCEPTION        (0)

#define CALLBACK_DB_CHGUSER_OK                  (0)

#define CALLBACK_DB_CHGUSER_NOT_FIND            (1) 

#define DAVE_VERNO_STR_LEN (64)

#define DAVE_USER_NAME_LEN (128)

#define DAVE_PASSWORD_LEN (64)

#define DAVE_UUID_LEN (64)

#define DAVE_DEVICE_NAME_LEN (128)

#define DAVE_DEVICE_MAC_LEN (16)

#define DAVE_DEVICE_IMEI_LEN (32)

#define DAVE_DEVICE_IMSI_LEN (32)

#define DAVE_DEVICE_MODEL_LEN (128)

#define DAVE_DEVICE_IP_LEN	(128)

#define DAVE_DEVICE_MCC_LEN	(8)

#define DAVE_DEVICE_MNC_LEN	(8)

#define DAVE_COUNTRY_NAME_LEN (128)

#define DAVE_VENDER_NAME_LEN (128)

#define DAVE_E164_NUMBER_MAX (32)

#define DAVE_E164_NUMBER_LEN (32)

#define DAVE_COUNTRY_PREFIX_LEN (8)

#define DAVE_LOG_DEVICE_NAME_LEN (6)

#define DAVE_UI_TEXT_MAX_LEN (16384)

#define DAVE_NORMAL_NAME_LEN (128)

#define DAVE_IMS_SUB_IDENTITY_LEN (64)

#define DAVE_IMPI_LEN DAVE_IMS_SUB_IDENTITY_LEN

#define DAVE_DIAMETER_EVENT_ID_LEN (128)

#define DAVE_DIAMETER_ICID_LEN (64)

#define DAVE_OCS_MIN_PRICE_TIME (10)

#define DAVE_OCS_MAX_PRICE_TIME (60 * 30)

#define DAVE_TOB_MIN_TIME (0)

#define DAVE_TOB_MAX_TIME (60 * 60 * 24 * 7)

#define DAVE_TOB_MAX_BALANCE (3600 * 24 * 365 * 68)//max:68year

#define DAVE_BILLING_SRC_ARRAY_MAX (1)

#define DAVE_BILLING_DST_ARRAY_MAX (256)

#define DAVE_BILLING_ADDRESS_LEN (32)

#define DAVE_BILLING_SMS_NODE_COUNT (32)

#define DAVE_BILLING_SMS_NODE_LEN (128)

#define DAVE_BILLING_SMS_DB_LEN (4096) 

#define DAVE_BILLING_SMS_SQL_LEN (20480) 

#define DAVE_BILLING_MAX_RULES_IN_PACKAGE (16)

#define DAVE_BILLING_MAX_RULES_IN_USER (48)

#define DAVE_AUDIO_BS_PACKAGE_LEN   (128)

#define DAVE_AUDIO_BS_RULES_ID_LEN  (128)

#define DAVE_AUDIO_BS_LEN  (128)

#define DAVE_AUDIO_OLD_BILLING_TYPE    (0)

#define DAVE_AUDIO_BUSINESS_BILLING	   (1)

#define DAVE_AUDIO_SERVICE_ID_LEN (128)

#define DAVE_AUDIO_PHYSICS_LEN (128)

#if DAVE_BILLING_MAX_RULES_IN_PACKAGE > DAVE_BILLING_MAX_RULES_IN_USER
#error DAVE_BILLING_MAX_RULES_IN_PACKAGE must be less than DAVE_BILLING_MAX_RULES_IN_USER!!!
#endif

#define DAVE_BILLING_MAX_PACKAGE_IN_USER (32)

#define DAVE_SEARCH_INDEX_MAX (32)

#define DAVE_IMSI_LEN (15)

#define DAVE_ICCID_LEN (20)

#define DAVE_MSISDN_LEN (DAVE_E164_NUMBER_LEN)

#define DAVE_SPECIFIC_PTL_LEN (64)

#define DAVE_SMS_ROUTE_ID_LEN	(288)

#define DAVE_SMS_MESSAGE_LEN 	(2048)

#define DAVE_SMS_SIGN_LEN 	(64)

#define DAVE_SMPP_MSG_RECORD_LEN (256)

#define DAVE_SMS_7BIT_MAX_LEN 160
#define DAVE_SMS_8BIT_MAX_LEN 140
#define DAVE_SMS_UCS2_MAX_LEN 70

#define DAVE_SMS_TENANT_PRIVATE_LEN (512)

#define DAVE_SMS_COMM_TYPE_LEN (16)

#define DAVE_SMS_TASK_NAME_LEN (32)

#define DAVE_SMS_PROTOCOL_LEN 	(32)

#define DAVE_SMS_TENANT_NAME_LEN (64)

#define DAVE_SMS_PRICE_NAME_LEN	(64)

#define DAVE_SMS_PROPOSAL_LEN 	(64)

#define DAVE_SMS_ROUTE_PTL_CFG_NUM (10)

#define DAVE_SMS_UID_LEN 		(32)

#define DAVE_SMS_AMOUNT_LEN	(32)

#define DAVE_SMS_TYPE_LEN	(32)

#define DAVE_SMS_REPORT_MSG_LEN (128)

#define DAVE_SMS_String_LEN_16 (16)

#define DAVE_SMS_String_LEN_32 (32)

#define DAVE_SMS_TRANSACTION_TYPE_LEN (32)

#define DAVE_SMS_String_len_64 (64)

#define DAVE_SMS_String_LEN_128 (128)

#define DAVE_SMS_String_LEN_256 (256)

#define DAVE_SMS_String_LEN_1024 (1024)

#define DAVE_SMS_BROADCAST_DATA_LEN (4096)

#define DAVE_SMS_STATIS_NAME	(DAVE_SMS_PROTOCOL_LEN + 24)

#define DAVE_SMS_STATIS_DATA_LEN (4096)

#define DAVE_SMS_STATIS_NODE_MAX (60)

#define DAVE_SMS_BROADCAST_MSG_LEN (128)

#define DAVE_SMS_AMOUNT_TYPE_LEN (32)

#define DAVE_MULTI_SMS_SEND_MAX (1)

#define DAVE_SMS_MSG_TYPE (32)

#define DAVE_SMS_MSG_TYPE_3 (128)

#define DAVE_SMS_PRIORITY (32)

#define DAVE_SMS_STATE (32)

#define DAVE_SMS_RSP_TYPE (32)

#define DAVE_SMS_REGISTER_NUM (4096)

#define DAVE_SMS_DEST_REGIONS 	(32)

#define DAVE_SMS_PRICE_DETAIL_MAX (16)

#define DAVE_SMS_ROUTER_REDIRECT_MAX (10)

#define DAVE_SMS_PRICE_REGIONS_MAX (300)

#define DAVE_SMS_VERSION_LEN	 (32)

#define DAVE_SMS_FILE_NAME_LEN	(64)

#define DAVE_SMS_ACCOUNT_LEN (32)

#define DAVE_SMS_PASSWORD_LEN (32)

#define DAVE_SMS_UID_64 (64)

#define DAVE_SMS_RECEIVED_LEN (32)

#define DAVE_SMS_SYSTEM_ID (32)

#define DAVE_REG_VERSION_LEN (64)

#define DAVE_SMPP_STANDARD_COUNT_MAX (16)

#define DAVE_ALISMS_FORMAT_LEN	(8192)

#define DAVE_ALISMS_CODE_LEN	(16)

#define DAVE_ALISMS_RESULT_LEN	(1024)

#define DAVE_ALISMS_TEMPLAT_LEN	(128)

#define DAVE_HTTP_ACCOUNT_LEN_MAX	(64)

#define DAVE_HTTP_PASSWD_LEN_MAX	(32)

#define DAVE_SMPP_SYSTEM_ID_LEN_MAX (15)

#define DAVE_SMPP_SYSTEM_PASSWD_LEN_MAX (9)

#define DAVE_SMPP_SERVER_IP_LEN (128)

#define DAVE_SMPP_SERVER_ID_LEN (128)

#define DAVE_SMPP_SERVER_RULES_MAX (2048)//32768

#define DAVE_SMPP_RULES_MAX (128)

#define DAVE_SMPP_LONG_MSG_INDEX_MAX (12)

#define DAVE_SMPP_SYSTEM_ID_LEN (16)

#define DAVE_SMPP_SERVICE_TYPE_LEN (6)

#define DAVE_SMPP_ADDRESS_LEN (21)

#define DAVE_SMPP_DELIVERY_TIME_LEN (17)

#define DAVE_SMPP_VALIDITY_PERIOD_LEN (17)

#define DAVE_SMPP_MESSAGE_ID_LEN (65)

#define DAVE_SMPP_BLB_UID_LEN (64)

#define DAVE_SMPP_SHORT_MESSAGE_RESOUR (255)

#define SMPP_DR_DECIMALISM_FORMAT 10

#define SMPP_DR_HEXADECIMAL_FORMAT 16

#define DAVE_NUMBER_RECORD_ID_LEN_MAX (64)

#define DAVE_IMPU_PREFIX_LEN (16)

#define DAVE_BUSINESS_SERIAL_LEN (128)

#define DAVE_BILLING_BUSINESS_SERIAL_LEN (64)

#define DAVE_TOKEN_LEN (128)

#define DAVE_TOKEN_KEY_LEN (64)

#define DAVE_ERROR_DESC_LEN (128)

#define DAVE_NICKNAME_LEN (128)

#define DAVE_UIP_CMD_STR_LEN (2048)

#define DAVE_PACKAGE_NAME_LEN (128)

#define DAVE_VALIDITY_UNIT_STR_LEN (8)

#define DAVE_MAX_PACKAGE_NUMBER (48)

#define DAVE_GROUP_ADDRESS_MAX (8)

#define DAVE_NOSQL_SEARCH_MAX (1024)

#define DAVE_NOSQL_KEY_STR_MAX (64)

#define DAVE_NOSQL_KEY_MAX (12)

#define DAVE_BDATA_TRACE_MSG_MAX (4096)

#define DAVE_BDATA_STATISTICS_MSG_MAX (4096)

#define DAVE_BLACKLIST_MAX (DAVE_MSISDN_LEN * 128)

#define DAVE_WHITELIST_MAX (DAVE_MSISDN_LEN)

#define DAVE_POI_NAME_MAX (128)

#define DAVE_POI_ADDRESS_MAX (256)

#define DAVE_CONF_USER_MAX (32)

#define DAVE_FORBIDDEN_NUMBER_ARRAY_LEN (16)

#define DAVE_FORBIDDEN_NUMBER_LEN (16)

#define DAVE_REDIS_RET_STR_MAX (6144)

#define DAVE_REDIS_RET_ARRAYS_MAX (1024)

#define DAVE_REDIS_BIN_MAX (32768)

#define DAVE_PROTOCOL_LEN (32)

#define DAVE_DELIVER_TIME (32)

#define DAVE_FORMAT_TIME_LEN (32)

#define DAVE_TIME_STAMP_LEN (32)

#define DAVE_PATH_LEN (256)

#define DAVE_HTTP_QUERY_STRING_LEN (256)

#define DAVE_PASSWORD_DIGEST_MAX (256)

#define DAVE_REDIS_SET_NAME_MAX (32)

#define DAVE_REDIS_SET_VALUE_MAX (1024)

#define DAVE_MAIL_SUBJECT_LENGTH (128)

#define DAVE_IM_LENGTH (2048)

#define DAVE_ALLOWED_CODEC_MAX (3)

#define DAVE_THREAD_NAME_LEN (32)

#define DAVE_GERENAL_CONFING_LEN (128)

#define DAVE_LARGE_CONFING_LEN (128)

#define DAVE_GERENAL_CONFING_DATA (409600)

#define DAVE_ES_ID_LEN (128)

#define DAVE_ES_INDEX_NAME_LEN (32)

#define DAVE_REDIS_BIN_ARRAY_MAX (10000)

#define DAVE_REDIS_RET_BIN_ARRAY_MAX DAVE_REDIS_BIN_ARRAY_MAX*2

#define DAVE_REDIS_BIN_ARRAY_DATA_LEN DAVE_REDIS_RET_STR_MAX

#define DAVE_CALL_ID_NUMBER_LEN (64)

#define DAVE_SIP_TAG_LEN (64)

#define DAVE_MAX_AUX_PKG_EFFECTIVE_MONTHS (36)

#define DAVE_MAX_AUX_PKG_EFFECTIVE_WEEKS (100)

#define DAVE_MAX_AUX_PKG_EFFECTIVE_DAYS (360)

#define DAVE_MAX_AUX_PKG_FROZEN_DAYS (30)

#define DAVE_MAX_RECORD_ID_LEN (128)

#define DAVE_AUTH_NONCE_MAX_LEN (128)

#define DAVE_NLP_MAX_SEG_CHAIN 1024

#define DAVE_NLP_MAX_NER_CHAIN 1024

#define DAVE_AUTHORIZATION_LEN (128)

#define DAVE_AUTHORIZATION_STR_LEN (DAVE_AUTHORIZATION_LEN * 2 + 1)

#define DAVE_LABEL_STR_MAX (256)

#define DAVE_BUILDING_BLOCKS_MAX (255)

#define DAVE_LABEL_EXTRA_INFO_MAX (128)

#define DAVE_REDIS_THREAD_MAX (32)

#define DAVE_MODEL_TIME_CONSUMING "__TIME_CONSUMING__"

#define DAVE_MAX_JUPHOON_NOTICE_USER_NUMBER (32)

#define DAVE_SHA1_IMAGE_ID (DAVE_SHA1_HASH_LEN * 3 + 1)

#define DAVE_KEY_LEN_MAX (128)

#define DAVE_KEY_OPT_MAX (32)

#define DAVE_TENANT_NAME_LEN (128)

#define DAVE_POST_DEFAULT_TIME_OUT (15)

#define DAVE_VGG_FEATURE_LEN (1024)

#define DAVE_DOUBLE_STR_MAX (128)

#define DAVE_IMAGE_TITLE_LEN (256)

#define DAVE_CDR_DOMAIN_ADDR_LEN (64)

#define DAVE_CDR_DOMAIN_USER_LEN (32)

#define DAVE_CDR_DOMAIN_PWD_LEN (32)

#define DAVE_DBA_PAGE_MAX 32

#define DAVE_RMG_CLIENT_NAME_LEN (32)

#define DAVE_RMG_ID_KEY_VALUE_LEN (32)

#define DAVE_RMG_GROUP_NAME_LEN (32)

#define DAVE_RMG_COLLECT_NAME_LEN	(32)

#define DAVE_RMG_INDEXES_NAME_LEN	(64)

#define DAVE_FILE_NAME_DELIMITER '_'

#define DAVE_UIP_METHOD_MAX_NUM (256)

#define DAVE_UIP_METHOD_MAX_LEN (64)

#define DAVE_MAX_FORWARD_USER_NUMBER (16)

#define DAVE_MOBILE_BIZ_CODE_LEN (8)

#define DAVE_MOBILE_BIZ_DESC_LEN (128)

#define DAVE_RESULT_DESC_LEN (128)

#define DAVE_UIP_JSON_BODY_DATA "__BODY_DATA__"

#define DAVE_MOBILE_MIN_EFFECTIVE_MIN (1)

#define DAVE_MOBILE_MAX_EFFECTIVE_MIN (1*60*24*30)

#define DAVE_SUPPLIER_NAME_LEN (128)

#define DAVE_SUPPLIER_CHANNEL_ID_LEN (128)

#define DAVE_MDM_SEND_ID_LEN (128)

#define DAVE_MDM_MESSAGE_ID_LEN 128

#define DAVE_MAX_CHARGE_USER_NUMBER 32

#define DAVE_CALL_ID_LEN 128

#define DAVE_WORKMATE_RET_CODE_LEN 8

#define DAVE_WORKMATE_RET_MSG_LEN 128

#define DAVE_COMMENT_MCARD_ARRAY_MAX 16

#define DAVE_BILLING_SMS_CDR_LEN 40960

#define DAVE_AWS_SMS_LOG_LEN 40960

#define DAVE_BILLING_SMS_PHONE_NUMBER 32

#define DAVE_MAX_MEDIA_NUM_BIND_TIMES 16

#define DAVE_MAX_MEDIA_NUM_BIND_SECONDS 60*60*24*7

#define DAVE_PROVIDER_DATA_LEN (2048)

#define DAVE_PROVIDER_MCCMNC_LEN (8)

#define DAVE_MAX_PHY_CHANNEL_IP_NUM 64

#define DAVE_MAX_DST_PREFIX_NUM 1700

#define DAVE_DST_PREFIX_LEN 10

#define DAVE_DA_PREFIX_LEN 16

#define DAVE_MAX_SER_PHY_CHN_NUM 1050

#define DAVE_MAX_SER_CHN_NUM 16

#define DAVE_MAX_ENT_IP_NUM 64

#define DAVE_PROVIDER_NAME_MAX_LEN 256

#define DAVE_PROVIDER_ID_MAX_LEN 6

#define DAVE_MAX_PHY_NAME_LEN 25

#define DAVE_PROVIDER_STRING_32 (32)

#define DAVE_PROVIDER_STRING_64 (64)

#define DAVE_PROVIDER_STRING_128 (128)

#define DAVE_MAX_FORWARD_COUNTRY_NUMBER (64)

#endif

