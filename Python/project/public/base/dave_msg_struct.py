# -*- coding: utf-8 -*-
#/*
# * Copyright (c) 2022 Renwei
# *
# * This is a free software; you can redistribute it and/or modify
# * it under the terms of the MIT license. See LICENSE for details.
# *
# * ############################# IMPORTANT INFORMATION ############################
# * The code of this file is automatically generated by tools(Tools/rpc),
# * please do not modify it manually!
# * ############################# IMPORTANT INFORMATION ############################
# * ================================================================================
# */

from ctypes import *
from .dave_enum import *
from .dave_msg_id import *
from .dave_struct import *
from .dave_define import *

class TESTMSG (Structure):
	_fields_ = [
		("test_msg", c_char * 4096),
]

class TIMERMSG (Structure):
	_fields_ = [
		("timer_id", c_longlong),
]

class WAKEUPMSG (Structure):
	_fields_ = [
		("null_msg", c_void_p),
		("some_msg", c_uint),
]

class RUNFUNCTIONMSG (Structure):
	_fields_ = [
		("thread_fun", c_void_p),
		("last_fun", c_void_p),
		("thread_dst", c_ulonglong),
		("initialization_flag", c_char),
]

class DebugReq (Structure):
	_fields_ = [
		("msg", c_char * 4096),
		("ptr", c_void_p),
]

class DebugRsp (Structure):
	_fields_ = [
		("msg", c_char * 16384),
		("ptr", c_void_p),
]

class RESTARTREQMSG (Structure):
	_fields_ = [
		("reason", c_char * 128),
		("times", c_ulonglong),
]

class RESTARTRSPMSG (Structure):
	_fields_ = [
		("wait_flag", c_char),
]

class POWEROFFMSG (Structure):
	_fields_ = [
		("reason", c_char * 128),
]

class ThreadRemoteReadyMsg (Structure):
	_fields_ = [
		("remote_thread_id", c_ulonglong),
		("remote_thread_name", c_char * 128),
]

class ThreadRemoteRemoveMsg (Structure):
	_fields_ = [
		("remote_thread_id", c_ulonglong),
		("remote_thread_name", c_char * 128),
]

class TraceSwitchMsg (Structure):
	_fields_ = [
		("thread_id", c_ulonglong),
		("trace_on", c_char),
]

class ProcessMsgTimerOutMsg (Structure):
	_fields_ = [
		("msg_id", c_ulonglong),
		("msg_len", c_ulonglong),
		("msg_body", c_void_p),
]

class TemporarilyDefineMessageMsg (Structure):
	_fields_ = [
		("parameter", c_void_p),
]

class SystemMount (Structure):
	_fields_ = [
		("socket", c_int),
		("verno", c_char * DAVE_VERNO_STR_LEN),
		("NetInfo", SocNetInfo),
]

class SystemDecoupling (Structure):
	_fields_ = [
		("socket", c_int),
		("verno", c_char * DAVE_VERNO_STR_LEN),
		("NetInfo", SocNetInfo),
]

class MemoryWarning (Structure):
	_fields_ = [
		("used_percentage", c_ulonglong),
]

class MsgIdEcho (Structure):
	_fields_ = [
		("echo_counter", c_ulonglong),
		("echo_time", c_ulonglong),
		("echo_multiple", c_char),
		("concurrency_flag", c_char),
		("msg", c_char * 256),
]

class InternalEvents (Structure):
	_fields_ = [
		("event_id", c_ulonglong),
		("ptr", c_void_p),
]

class ThreadBusy (Structure):
	_fields_ = [
		("thread_id", c_ulonglong),
		("thread_name", c_char * DAVE_THREAD_NAME_LEN),
		("msg_id", c_ulonglong),
		("msg_number", c_ulonglong),
]

class ThreadIdle (Structure):
	_fields_ = [
		("thread_id", c_ulonglong),
		("thread_name", c_char * DAVE_THREAD_NAME_LEN),
]

class ClientBusy (Structure):
	_fields_ = [
		("verno", c_char * DAVE_VERNO_STR_LEN),
		("ptr", c_void_p),
]

class ClientIdle (Structure):
	_fields_ = [
		("verno", c_char * DAVE_VERNO_STR_LEN),
		("ptr", c_void_p),
]

class ThreadRemoteIDReadyMsg (Structure):
	_fields_ = [
		("remote_thread_id", c_ulonglong),
		("remote_thread_name", c_char * 128),
		("globally_identifier", c_char * DAVE_GLOBALLY_IDENTIFIER_LEN),
]

class ThreadRemoteIDRemoveMsg (Structure):
	_fields_ = [
		("remote_thread_id", c_ulonglong),
		("remote_thread_name", c_char * 128),
		("globally_identifier", c_char * DAVE_GLOBALLY_IDENTIFIER_LEN),
]

class ThreadLocalReadyMsg (Structure):
	_fields_ = [
		("local_thread_id", c_ulonglong),
		("local_thread_name", c_char * 128),
]

class ThreadLocalRemoveMsg (Structure):
	_fields_ = [
		("local_thread_id", c_ulonglong),
		("local_thread_name", c_char * 128),
]

class RPCDebugMsg (Structure):
	_fields_ = [
		("ret_debug", c_ulonglong),
		("s8_debug", c_char),
		("u8_debug", c_char),
		("u16_debug", c_ushort),
		("s32_debug", c_int),
		("u32_debug", c_uint),
		("void_debug", c_void_p),
		("date_debug", DateStruct),
		("mbuf_debug", POINTER(MBUF)),
]

class CFGUpdate (Structure):
	_fields_ = [
		("cfg_name", c_char * DAVE_NORMAL_NAME_LEN),
		("cfg_length", c_ulonglong),
		("cfg_value", c_char * 8196),
]

class MsgBlocksReq (Structure):
	_fields_ = [
		("opt", c_int),
		("blocks_id_1", c_ulonglong),
		("blocks_id_2", c_ulonglong),
		("ptr", c_void_p),
]

class MsgBlocksRsp (Structure):
	_fields_ = [
		("ret", c_ulonglong),
		("opt", c_int),
		("blocks", BuildingBlocks * DAVE_BUILDING_BLOCKS_MAX),
		("ptr", c_void_p),
]

class MsgOSNotify (Structure):
	_fields_ = [
		("notify_info", c_ulonglong),
]

class SocketBindReq (Structure):
	_fields_ = [
		("NetInfo", SocNetInfo),
		("ptr", c_void_p),
]

class SocketBindRsp (Structure):
	_fields_ = [
		("socket", c_int),
		("NetInfo", SocNetInfo),
		("BindInfo", c_int),
		("thread_id", c_ulonglong),
		("ptr", c_void_p),
]

class SocketConnectReq (Structure):
	_fields_ = [
		("NetInfo", SocNetInfo),
		("ptr", c_void_p),
]

class SocketConnectRsp (Structure):
	_fields_ = [
		("socket", c_int),
		("NetInfo", SocNetInfo),
		("ConnectInfo", c_int),
		("thread_id", c_ulonglong),
		("ptr", c_void_p),
]

class SocketDisconnectReq (Structure):
	_fields_ = [
		("socket", c_int),
		("ptr", c_void_p),
]

class SocketDisconnectRsp (Structure):
	_fields_ = [
		("socket", c_int),
		("result", c_int),
		("ptr", c_void_p),
]

class SocketPlugIn (Structure):
	_fields_ = [
		("father_socket", c_int),
		("child_socket", c_int),
		("NetInfo", SocNetInfo),
		("thread_id", c_ulonglong),
		("ptr", c_void_p),
]

class SocketPlugOut (Structure):
	_fields_ = [
		("socket", c_int),
		("reason", c_int),
		("NetInfo", SocNetInfo),
		("thread_id", c_ulonglong),
		("ptr", c_void_p),
]

class SocketRead (Structure):
	_fields_ = [
		("socket", c_int),
		("IPInfo", IPBaseInfo),
		("data_len", c_ulonglong),
		("data", POINTER(MBUF)),
		("ptr", c_void_p),
]

class SocketWrite (Structure):
	_fields_ = [
		("socket", c_int),
		("IPInfo", IPBaseInfo),
		("data_len", c_ulonglong),
		("data", POINTER(MBUF)),
		("close_flag", c_int),
]

class SocketNotify (Structure):
	_fields_ = [
		("socket", c_int),
		("notify", c_int),
		("data", c_ulonglong),
		("ptr", c_void_p),
]

class SocketRawEvent (Structure):
	_fields_ = [
		("socket", c_int),
		("os_socket", c_int),
		("event", c_int),
		("NetInfo", SocNetInfo),
		("data", POINTER(MBUF)),
		("ptr", c_void_p),
]

class HTTPListenReq (Structure):
	_fields_ = [
		("listen_port", c_ulonglong),
		("rule", c_int),
		("type", c_int),
		("path", c_char * DAVE_PATH_LEN),
		("ptr", c_void_p),
]

class HTTPListenRsp (Structure):
	_fields_ = [
		("ret", c_ulonglong),
		("listen_port", c_ulonglong),
		("path", c_char * DAVE_PATH_LEN),
		("ptr", c_void_p),
]

class HTTPCloseReq (Structure):
	_fields_ = [
		("listen_port", c_ulonglong),
		("path", c_char * DAVE_PATH_LEN),
		("ptr", c_void_p),
]

class HTTPCloseRsp (Structure):
	_fields_ = [
		("ret", c_ulonglong),
		("listen_port", c_ulonglong),
		("path", c_char * DAVE_PATH_LEN),
		("ptr", c_void_p),
]

class HTTPRecvReq (Structure):
	_fields_ = [
		("listen_port", c_ulonglong),
		("remote_address", c_char * DAVE_URL_LEN),
		("remote_port", c_ulonglong),
		("method", c_int),
		("head", HttpKeyValue * DAVE_HTTP_HEAD_MAX),
		("content_type", c_int),
		("content", POINTER(MBUF)),
		("local_creat_time", c_ulonglong),
		("ptr", c_void_p),
]

class HTTPRecvRsp (Structure):
	_fields_ = [
		("ret", c_ulonglong),
		("content_type", c_int),
		("content", POINTER(MBUF)),
		("local_creat_time", c_ulonglong),
		("ptr", c_void_p),
]

