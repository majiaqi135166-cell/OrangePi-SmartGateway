
#ifndef MONITOR_H
#define MONITOR_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <string.h>
// 1. 定义结构体 (必须在函数声明之前)
struct SystemState {
    float hot;
    float warm;
    float last_temp;
    float current_temp;
    int server_fd;

};

// 2. 外部变量声明
extern int keep_running;

// 3. 函数原型统一：全部接收结构体指针
void load_config(struct SystemState* s);
void control_hardware(struct SystemState* s);
float log_temperature();
void write_log(struct SystemState* s, const char* status);
void handle_sigint(int sig);
void* web_server_thread(void* arg);

// 颜色宏定义
#define RED_TEXT "\033[31m"
#define RESET_TEXT "\033[0m"

#endif



