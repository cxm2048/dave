/*
 * Copyright (c) 2022 Renwei
 *
 * This is a free software; you can redistribute it and/or modify
 * it under the terms of the MIT license. See LICENSE for details.
 *
 * ############################# IMPORTANT INFORMATION ############################
 * The code of this file is automatically generated by tools(Tools/rpc),
 * please do not modify it manually!
 * ############################# IMPORTANT INFORMATION ############################
 * ================================================================================
 */

#ifndef _T_RPC_ENUMDATA_H__
#define _T_RPC_ENUMDATA_H__
#include "dave_base.h"
#include "base_enum.h"
#include "base_error_code.h"
#include "http_param.h"


void * t_rpc_ver3_zip_SOCTYPE(SOCTYPE zip_data);
dave_bool t_rpc_ver3_unzip_SOCTYPE(SOCTYPE *unzip_data, void *pArrayBson);

void * t_rpc_ver3_zip_NetAddrType(NetAddrType zip_data);
dave_bool t_rpc_ver3_unzip_NetAddrType(NetAddrType *unzip_data, void *pArrayBson);

void * t_rpc_ver3_zip_FixedPortFlag(FixedPortFlag zip_data);
dave_bool t_rpc_ver3_unzip_FixedPortFlag(FixedPortFlag *unzip_data, void *pArrayBson);

void * t_rpc_ver3_zip_EnableKeepAliveFlag(EnableKeepAliveFlag zip_data);
dave_bool t_rpc_ver3_unzip_EnableKeepAliveFlag(EnableKeepAliveFlag *unzip_data, void *pArrayBson);

void * t_rpc_ver3_zip_EnableNetCardBindFlag(EnableNetCardBindFlag zip_data);
dave_bool t_rpc_ver3_unzip_EnableNetCardBindFlag(EnableNetCardBindFlag *unzip_data, void *pArrayBson);

void * t_rpc_ver3_zip_SOCKETINFO(SOCKETINFO zip_data);
dave_bool t_rpc_ver3_unzip_SOCKETINFO(SOCKETINFO *unzip_data, void *pArrayBson);

void * t_rpc_ver3_zip_SOCEVENT(SOCEVENT zip_data);
dave_bool t_rpc_ver3_unzip_SOCEVENT(SOCEVENT *unzip_data, void *pArrayBson);

void * t_rpc_ver3_zip_IPVER(IPVER zip_data);
dave_bool t_rpc_ver3_unzip_IPVER(IPVER *unzip_data, void *pArrayBson);

void * t_rpc_ver3_zip_SOCDOMAIN(SOCDOMAIN zip_data);
dave_bool t_rpc_ver3_unzip_SOCDOMAIN(SOCDOMAIN *unzip_data, void *pArrayBson);

void * t_rpc_ver3_zip_IPProtocol(IPProtocol zip_data);
dave_bool t_rpc_ver3_unzip_IPProtocol(IPProtocol *unzip_data, void *pArrayBson);

void * t_rpc_ver3_zip_BuildingBlocksOpt(BuildingBlocksOpt zip_data);
dave_bool t_rpc_ver3_unzip_BuildingBlocksOpt(BuildingBlocksOpt *unzip_data, void *pArrayBson);

void * t_rpc_ver3_zip_ErrCode(ErrCode zip_data);
dave_bool t_rpc_ver3_unzip_ErrCode(ErrCode *unzip_data, void *pArrayBson);

void * t_rpc_ver3_zip_HTTPListenType(HTTPListenType zip_data);
dave_bool t_rpc_ver3_unzip_HTTPListenType(HTTPListenType *unzip_data, void *pArrayBson);

void * t_rpc_ver3_zip_HTTPMathcRule(HTTPMathcRule zip_data);
dave_bool t_rpc_ver3_unzip_HTTPMathcRule(HTTPMathcRule *unzip_data, void *pArrayBson);

void * t_rpc_ver3_zip_HttpMethod(HttpMethod zip_data);
dave_bool t_rpc_ver3_unzip_HttpMethod(HttpMethod *unzip_data, void *pArrayBson);

void * t_rpc_ver3_zip_HttpContentType(HttpContentType zip_data);
dave_bool t_rpc_ver3_unzip_HttpContentType(HttpContentType *unzip_data, void *pArrayBson);

#endif

