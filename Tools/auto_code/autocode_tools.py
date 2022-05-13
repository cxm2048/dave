# -*- coding: utf-8 -*-
#/*
# * Copyright (c) 2022 Renwei
# *
# * This is a free software; you can redistribute it and/or modify
# * it under the terms of the MIT license. See LICENSE for details.
# */
import re
import sys


_copyright_message_for_c_and_go = "\
/*\n\
 * Copyright (c) 2022 Renwei\n\
 *\n\
 * This is a free software; you can redistribute it and/or modify\n\
 * it under the terms of the MIT license. See LICENSE for details.\n\
 *\n\
 * ############################# IMPORTANT INFORMATION ############################\n\
 * The code of this file is automatically generated by tools(Tools/auto_code),\n\
 * please do not modify it manually!\n\
 * ############################# IMPORTANT INFORMATION ############################\n\
 * ================================================================================\n\
 */\n\n"\

_copyright_message_for_python = "\
#/*\n\
# * Copyright (c) 2022 Renwei\n\
# *\n\
# * This is a free software; you can redistribute it and/or modify\n\
# * it under the terms of the MIT license. See LICENSE for details.\n\
# *\n\
# * ############################# IMPORTANT INFORMATION ############################\n\
# * The code of this file is automatically generated by tools(Tools/auto_code),\n\
# * please do not modify it manually!\n\
# * ############################# IMPORTANT INFORMATION ############################\n\
# * ================================================================================\n\
# */\n\n"\


def _get_struct_body_data(body):
    data_types = ""
    data_name = ""
    data_dimension = ""

    # get data_types
    body_index = 0
    while body_index < len(body):
        if body[body_index] != ' ':
            break
        body_index += 1
    while body_index < len(body):
        if body[body_index] == ' ' or body[body_index] == '*':
            break
        body_index += 1
    while body_index < len(body):
        if body[body_index] == ' ' or body[body_index] == '*':
            body_index += 1
        else:
            data_types = body[:body_index]
            break

    # get data_name
    body = body[body_index:]
    body_index = 0
    while body_index < len(body):
        if body[body_index] == ' ' or body[body_index] == '[' or (body_index + 1) == len(body):
            if (body_index + 1) == len(body):
                data_name = body[:body_index+1]
                body_index += 1
            else:
                data_name = body[:body_index]
            break
        body_index += 1

    # get data_dimension
    body = body[body_index:]
    body_index = 0
    while body_index < len(body):
        if body[body_index] == '[':
            data_dimension = body[:len(body)]
            break
        body_index += 1

    if data_types != None:
        data_types = data_types.replace(" ", "")
    if data_name != None:
        data_name = data_name.replace(" ", "")
    if data_dimension != None:
        data_dimension = data_dimension.replace(" ", "").replace("][", "*").replace("[", "").replace("]", "")

    return data_name, data_types, data_dimension


def _struct_data_to_dict(data_name, data_types, data_dimension, struct_line):
    if (data_types != "") and (not re.search('[A-Z,a-z,0-9,"_"]+?', data_types)):
        print(f"1 _struct_data_to_dict data_name:{data_name} data_types:{data_types} struct_line:{struct_line}")
    if data_types == "struct":
        print(f"2 _struct_data_to_dict data_name:{data_name} data_types:{data_types} struct_line:{struct_line}")

    base_data = {}

    types_len = len(data_types)
    if types_len == 0:
        return base_data

    base_data["n"] = data_name

    if data_types[types_len-1] == '*':
        base_data["p"] = True
        data_types = data_types[0:types_len-1]
    else:
        base_data["p"] = False

    base_data["t"] = data_types

    if data_dimension != "":
        base_data["d"] = data_dimension
    else:
        base_data["d"] = None
    return base_data


#
# 目前还不支持结构体内，直接定义嵌套结构体，
# 这个函数主要检测是否有这种定义发生，
# 如果有，就告警！！！
#
def _check_struct_data_has_invalid_define(struct_data):
    if struct_data.count('{') > 1:
        print(f"find unsupport define:{struct_data}")
    return


def _check_struct_list_has_invalid_define(struct_list):
    if struct_list != None:
        for struct_data in struct_list:
            _check_struct_data_has_invalid_define(struct_data)
    return


def _get_struct_name_and_body(struct_data):
    struct_name = re.findall("}(.*?);", struct_data)
    if len(struct_name) == 0:
        struct_name = re.findall("typedef struct(.*?){", struct_data)
    if len(struct_name) == 0:
        struct_name = re.findall("typedef union(.*?){", struct_data)
    struct_body = re.findall("typedef struct.*?\{(.*?)\}.*?;", struct_data)
    if len(struct_body) == 0:
        struct_body = re.findall("typedef union.*?\{(.*?)\}.*?;", struct_data)
    if struct_name == None or struct_name == "" or struct_body == None or struct_body == "":
        print(f"struct_name:{struct_name} struct_body:{struct_body} struct_data:{struct_data}")
    if len(struct_body) == 0:
        print(f"the struct_body is empty:{struct_data} struct_name:{struct_name}")
    return get_array_data(struct_name, 0).replace(" ", ""), struct_body


def _struct_sort_by_before_and_after_calls(struct_data):
    new_struct_data = {}

    for struct_name in struct_data.keys():
        for struct_member in struct_table_get(struct_data[struct_name]):
            member_name = struct_member['t']
            member_data = struct_data.get(member_name, None)
            if member_data != None:
                new_struct_data[member_name] = member_data
        new_struct_data[struct_name] = struct_data.get(struct_name, None)

    return new_struct_data


def _leaving_only_the_data_structure_string(string_ptr):
    new_string = ''

    string_len = len(string_ptr)
    str_temp_buf = ''
    find_left_brace = False
    find_right_brace = False
    for string_index in range(string_len):
        str_temp_buf += string_ptr[string_index]

        reset_temp_buf = False

        if len(str_temp_buf) <= 14:
            if str_temp_buf != 'typedef struct'[0:len(str_temp_buf)]:
                str_temp_buf = ''
                find_left_brace = False
                find_right_brace = False
        elif len(str_temp_buf) > 14:
            if string_ptr[string_index] == '{':
                if find_left_brace == False:
                    find_left_brace = True
                else:
                    reset_temp_buf = True
            elif string_ptr[string_index] == '}':
                find_right_brace = True
            elif string_ptr[string_index] == ';':
                if find_left_brace == True:
                    if find_right_brace == True:
                        if str_temp_buf.count('struct') == 1:
                            new_string += str_temp_buf + ' '
                        reset_temp_buf = True
                else:
                    reset_temp_buf = True
            elif string_ptr[string_index] != ' ':
                if find_left_brace == False:
                    reset_temp_buf = True

        if reset_temp_buf == True:
            str_temp_buf = ''
            find_left_brace = False
            find_right_brace = False

    return new_string


def _remove_annotation_data(data_ptr):
    data_len = len(data_ptr)
    new_data = ''

    get_data_flag = True
    for data_index in range(data_len):
        if ((data_len - data_index) >= 2)\
            and (data_ptr[data_index] == '/')\
            and (data_ptr[data_index + 1] == '*'):
            get_data_flag = False
            continue
        if (get_data_flag == False)\
            and (data_ptr[data_index - 1] == '*')\
            and (data_ptr[data_index] == '/'):
            get_data_flag = True
            continue
        if get_data_flag == True:
            new_data += data_ptr[data_index]

    return new_data


# =====================================================================


def remove_invalid_data(file_data):
    #
    # 需要按步骤去除
    #
    file_data = re.sub("//.*?\r\n", "", file_data)
    file_data = re.sub("//.*?\n", "", file_data)
    file_data = re.sub("\n|\r\n|\t", " ", file_data)
    return file_data


def remove_annotation_data(file_data):
    file_data = _remove_annotation_data(file_data)
    return remove_invalid_data(file_data)


def get_array_data(array_list, index):
    counter = 0
    ret_array_data = None
    for array_data in array_list:
        if counter >= index:
            ret_array_data = array_data
            break
    return ret_array_data


def get_struct_data(struct_data):
    struct_data = remove_annotation_data(struct_data)
    _check_struct_data_has_invalid_define(struct_data)
    struct_name, struct_body = _get_struct_name_and_body(struct_data)

    base_array = []
    for struct_line in struct_body:
        body_list = re.findall("(.*?);", struct_line)

        for body in body_list:
            data_name, data_types, data_dimension = _get_struct_body_data(body)
            base_dict = _struct_data_to_dict(data_name, data_types, data_dimension, struct_line)
            base_array.append(base_dict)
    return struct_name, base_array


def get_struct_list(file_data):
    file_data = remove_annotation_data(file_data)
    file_data = _leaving_only_the_data_structure_string(file_data)
    struct_list = re.findall("typedef struct.*?\{.*?\}.*?;", file_data)
    _check_struct_list_has_invalid_define(struct_list)
    return struct_list


def get_enum_id_table(enum_content):
    enum_content = str(re.findall("typedef enum.*?\{(.*?)\}.*?;", enum_content))
    enum_content = enum_content.replace(' ', '').replace('[', '').replace(']', '').replace('\'', '') + ','
    enum_content = str(re.findall('([A-Z,a-z,0-9,_]*)=.*?,|([A-Z,a-z,0-9,_]*),', enum_content))
    enum_content = enum_content.replace('[', '').replace(']', '').replace('(', '').replace(')', '').replace('\'', '').replace(', ,', ',').replace(' ', '')
    enum_content = re.findall("(.*?),", enum_content)
    return enum_content


def copyright_message(file_id, copy_flag='c'):
    if copy_flag == 'c':
        file_id.write(_copyright_message_for_c_and_go)
    elif copy_flag == 'python':
        file_id.write(_copyright_message_for_python)
    else:
        file_id.write(_copyright_message_for_c_and_go)


def dividing_line(file_id, type_flag='c'):
    if type_flag == 'c' or type_flag == 'go':
        file_id.write("// =====================================================================\n")
    elif type_flag == 'python':
        file_id.write("# =====================================================================\n")


def include_message(file_id, include_list):
    include_list.sort()
    head_list2 = []
    for include_file in include_list:
        if include_file in head_list2:
            continue
        head_list2.append(include_file)
        include_file = include_file.rsplit("/", 1)[-1]
        file_id.write("#include \"" + include_file + "\"\n")
    file_id.write("\n")


def struct_table_set(msg_name, base_array):
    return { 'msg_name': msg_name, 'base_array': base_array }


def struct_table_get(struct_table, get_who='base_array'):
    return struct_table.get(get_who, None)


def struct_on_the_table(struct_name, struct_table):
    if struct_table.get(struct_name, None) != None:
        return True
    return False


def struct_sorted(struct_data):  
    sorted_keys = sorted(struct_data)
    new_struct_data = {}
    for sorted_key in sorted_keys:
        new_struct_data[sorted_key] = struct_data[sorted_key]
    return new_struct_data


def type_on_the_table(type_name, struct_table):
    for struct_key in struct_table:
        msg_struct = struct_table_get(struct_table[struct_key])
        for msg_member in msg_struct:
            if msg_member['t'] == type_name:
                return True
    return False


def define_on_the_table(define_name, struct_table):
    for struct_key in struct_table:
        msg_struct = struct_table_get(struct_table[struct_key])
        for msg_member in msg_struct:
            if msg_member['d'] != None:
                if define_name in msg_member['d']:
                    return True
    return False


def struct_dimension_decomposition(struct_dimension):
    high_struct_dimension = struct_dimension.split('*', 1)[0]
    low_struct_dimension = struct_dimension.split('*', 1)[-1]
    if high_struct_dimension == low_struct_dimension:
        return str(1), low_struct_dimension
    else:
        return high_struct_dimension, low_struct_dimension


def struct_sort_by_before_and_after_calls(struct_data):
    for loop_multiple_times_to_fully_sort in range(3):
        struct_data = _struct_sort_by_before_and_after_calls(struct_data)
    return struct_data


def is_digital_string(string_data):
    for string_index in range(len(string_data)):
        find_digital_string = False
        if ((string_data[string_index] >= '0') and (string_data[string_index] <= '9')) \
            or ((string_data[string_index] >= 'a') and (string_data[string_index] <= 'f')) \
            or ((string_data[string_index] >= 'A') and (string_data[string_index] <= 'F')) \
            or ((string_index == 0) and (string_data[string_index] == '-')) \
            or ((string_index != 0) and (string_data[string_index] == '*')) \
            or (string_data[string_index] == 'x') \
            or (string_data[string_index] == 'X'):
            find_digital_string = True
        if find_digital_string == False:
            return False
    return True