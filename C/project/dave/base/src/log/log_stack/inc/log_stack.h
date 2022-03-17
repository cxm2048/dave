/*
 * Copyright (c) 2022 Renwei
 *
 * This is a free software; you can redistribute it and/or modify
 * it under the terms of the MIT license. See LICENSE for details.
 */

#ifndef __LOG_STACK_H__
#define __LOG_STACK_H__
#include "base_macro.h"

#define LOG_SERVICE_PORT 6000

#if defined(__BASE_PRODUCT_LOG__) || defined(__BASE_PRODUCT_BASE__)
#define LOG_STACK_SERVER
#endif
#if !defined(__BASE_PRODUCT_LOG__)
#define LOG_STACK_CLIENT
#endif

void log_stack_init(void);

void log_stack_exit(void);

#endif
