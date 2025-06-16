#include <stdarg.h>
#include <stdio.h>
#include <time.h>

#include "logger.h"

void init_logger_file(const char *filename)
{
    if (log_file == NULL)
    {
        log_file = fopen(filename, "a");
        if (log_file == NULL)
        {
            fprintf(stderr, "ERROR: Could not open log file: %s\n", filename);
        }
    }
}

void close_logger_file()
{
    if (log_file != NULL)
    {
        fclose(log_file);
        log_file = NULL;
    }
}

void set_console_log_threshold(DEBUG_LOG_LEVEL level)
{
    if (level >= DEBUG_TRACE && level <= DEBUG_CRITICAL)
    {
        console_log_threshold = level;
        fprintf(stderr, "Console log threshold set to level %d\n", level);
    }
    else
    {
        fprintf(stderr, "WARNING: Invalid console log threshold level %d provided.\n", level);
    }
}

void set_file_log_threshold(DEBUG_LOG_LEVEL level)
{
    if (level >= DEBUG_TRACE && level <= DEBUG_CRITICAL)
    {
        file_log_threshold = level;
        if (log_file != NULL)
        {
            fprintf(log_file, "File log threshold set to level %d\n", level);
            fflush(log_file);
        }
    }
    else
    {
        fprintf(stderr, "WARNING: Invalid file log threshold level %d provided.\n", level);
    }
}

void dbg_log(const char *file, int line, const char *function, DEBUG_LOG_LEVEL level, const char *fmtstr, ...)
{
    va_list args_console; /*---for console log---*/
    va_list args_file;    /*---for file log---*/

    /*--- Get current time for timestamp ---*/
    time_t current_time;
    struct tm *local_time_info;
    char timestamp_buffer[30]; /*---Sufficient buffer for timestamp like "YYYY-MM-DD HH:MM:SS"---*/

    time(&current_time);
    local_time_info = localtime(&current_time);

    /*---Format the timestamp---*/
    strftime(timestamp_buffer, sizeof(timestamp_buffer), "%Y-%m-%d %H:%M:%S", local_time_info);

    /*---Prepare common log message prefix---*/
    char prefix_buffer[512];
    snprintf(prefix_buffer, sizeof(prefix_buffer), "[%s] [%s:%d] %s() [Level:%d]: ",
             timestamp_buffer, file, line, function, level);

    /*--- Console Sink ---*/
    if (level >= console_log_threshold)
    {
        va_start(args_console, fmtstr);
        fprintf(stderr, "%s", prefix_buffer);
        vfprintf(stderr, fmtstr, args_console);
        fprintf(stderr, "\n");
        va_end(args_console);
    }

    /*--- File Sink ---*/
    if (log_file != NULL && level >= file_log_threshold)
    {
        va_start(args_file, fmtstr);
        fprintf(log_file, "%s", prefix_buffer);
        vfprintf(log_file, fmtstr, args_file);
        fprintf(log_file, "\n");
        fflush(log_file);
        va_end(args_file);
    }
}