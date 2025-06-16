/*
 * Copyright 2024 Kamlesh Singh
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>

typedef enum
{
    DEBUG_TRACE = 0,
    DEBUG_INFO = 1,
    DEBUG_WARNING = 2,
    DEBUG_ERROR = 3,
    DEBUG_CRITICAL = 4
} DEBUG_LOG_LEVEL;

#define DEBUG_LOG(LEVEL, ...) dbg_log(__FILE__, __LINE__, __FUNCTION__, LEVEL, __VA_ARGS__)

static FILE *log_file = NULL;
static DEBUG_LOG_LEVEL console_log_threshold = DEBUG_INFO; /*---show on console---*/
static DEBUG_LOG_LEVEL file_log_threshold = DEBUG_TRACE;   /*---write to file---*/

void init_logger_file(const char *filename);
void close_logger_file();
void set_console_log_threshold(DEBUG_LOG_LEVEL level);
void set_file_log_threshold(DEBUG_LOG_LEVEL level);
void dbg_log(const char *file, int line, const char *function, DEBUG_LOG_LEVEL level, const char *fmtstr, ...);

#endif