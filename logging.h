//
// Created by andywei on 9/15/17.
//

#ifndef SIMPLE_HTTP_SERVER_LOGGING_H
#define SIMPLE_HTTP_SERVER_LOGGING_H

#include <stdio.h>
#include <stdarg.h>

enum{LOG_INFO, LOG_ERROR};

#define log_info(...) echo_log(LOG_INFO, __FILE__, __LINE__, __VA_ARGS__)
#define log_error(...) echo_log(LOG_ERROR, __FILE__, __LINE__, __VA_ARGS__)

void set_file(FILE *fp);
void set_log_level(int log_level);
void echo_log(int log_level, const char *record, int line, const char *fmt, ...);

#endif //SIMPLE_HTTP_SERVER_LOGGING_H