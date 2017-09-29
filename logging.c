//
// Created by andywei on 9/15/17.
//
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>

#include "logging.h"

static const char *level_names[] = {
        "INFO", "ERROR"
};

static struct{
    FILE *fp;
    int log_level;
    int mute;//if set to 0, print log into stderr; if set to 1, will not print log into stderr but still logging into file
}Logger;

void set_file(char *filepath){
    Logger.fp = fopen("log.txt", "w");
    if(Logger.fp == NULL){
        printf("error opening log file!\n");
        exit(EXIT_FAILURE);
    }
    setvbuf(fp, NULL, _IOLBF, 0);
}

void close_file(){
    if(Logger.fp){
        fclose(Logger.fp);
    }
}

void set_log_level(int log_level){
    Logger.log_level = log_level;
}

void set_log_mute(int enable){
    if(enable == 0){
        Logger.mute = 0;
    }
    else if(enable == 1){
        Logger.mute = 1;
    }
}

void echo_log(int log_level, const char *record, int line, const char *fmt, ...){
    if(log_level < Logger.log_level){
        return;
    }
    //Get time
    time_t t = time(NULL);
    struct tm *local_time = localtime(&t);

    //Log to file
    if(Logger.fp){
        va_list args;
        char time[32];
        time[strftime(time, sizeof(time), "%Y-%m-%d %H:%M:%S", local_time)] = '\0';
        fprintf(Logger.fp, "%-5s %s %s:%d: ", level_names[log_level], time, record, line);
        va_start(args, fmt);
        vfprintf(Logger.fp, fmt, args);
        va_end(args);
    }

    //Log to stderr
    if (!Logger.mute){
        va_list args;
        char time[16];
        time[strftime(time, sizeof(time), "%H:%M:%S", local_time)] = '\0';
        fprintf(stderr, "%-5s %s %s:%d: ", level_names[log_level], time, record, line);
        va_start(args, fmt);
        vfprintf(stderr, fmt, args);
        va_end(args);
    }
}