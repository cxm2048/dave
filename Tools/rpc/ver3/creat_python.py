# -*- coding: utf-8 -*-
#/*
# * Copyright (c) 2022 Renwei
# *
# * This is a free software; you can redistribute it and/or modify
# * it under the terms of the MIT license. See LICENSE for details.
# */
from ver3.python.ver3_creat_pythonrpc_file import creat_pythonrpc_file


def creat_ver3_python(struct_total, struct_table, msg_struct_table, define_table, msg_id_table, enum_table):
    creat_pythonrpc_file(struct_total, struct_table, msg_struct_table, define_table, msg_id_table, enum_table)