# -*- coding: utf-8 -*-
#/*
# * Copyright (c) 2022 Renwei
# *
# * This is a free software; you can redistribute it and/or modify
# * it under the terms of the MIT license. See LICENSE for details.
# */
import dave


def fun_MSGID_REMOTE_THREAD_ID_READY(src_name, src_id, msg_len, msg_body):
    pReady = dave.struct_copy(dave.ThreadRemoteIDReadyMsg, msg_body, msg_len)
    dave.DAVELOG(f'{pReady.remote_thread_id} {pReady.remote_thread_name} {pReady.globally_identifier}')
    return


def fun_MSGID_REMOTE_THREAD_ID_REMOVE(src_name, src_id, msg_len, msg_body):
    pRemove = dave.struct_copy(dave.ThreadRemoteIDRemoveMsg, msg_body, msg_len)
    dave.DAVELOG(f'{pRemove.remote_thread_id} {pRemove.remote_thread_name} {pRemove.globally_identifier}')
    return


# =====================================================================


def product_init():
    dave.dave_system_function_table_add(dave.MSGID_REMOTE_THREAD_ID_READY, fun_MSGID_REMOTE_THREAD_ID_READY)
    dave.dave_system_function_table_add(dave.MSGID_REMOTE_THREAD_ID_REMOVE, fun_MSGID_REMOTE_THREAD_ID_REMOVE)
    return


def product_exit():
    dave.dave_system_function_table_del(dave.MSGID_REMOTE_THREAD_ID_READY)
    dave.dave_system_function_table_del(dave.MSGID_REMOTE_THREAD_ID_REMOVE)
    return