#include "monitor.h"
#include <time.h>

// 1. 硬件控制函数
void control_hardware(struct SystemState* s) {
    // 修正路径拼写 green，修正返回值
    FILE* led_file = fopen("/sys/class/leds/green:power/brightness", "w"); 
    if (led_file == NULL) return; 

    if (s->current_temp < s->warm) { // 修正为 ->
        printf("System Cool: %.2f ℃\n", s->current_temp);
        fprintf(led_file, "0");
    } 
    else if (s->current_temp >= s->warm && s->current_temp < s->hot) { // 修正为 ->
        printf("System Warm: %.2f ℃\n", s->current_temp);
        fprintf(led_file, "1");
    } 
    else {
        printf(RED_TEXT "System Hot: %.2f ℃" RESET_TEXT "\n", s->current_temp);
        fprintf(led_file, "1");
        usleep(500000); // 0.5秒
        fprintf(led_file, "0");
        usleep(500000);
    }
    fclose(led_file);
}

// 2. 读取温度函数
float log_temperature() {
    FILE* log_tem = fopen("/sys/class/thermal/thermal_zone0/temp", "r");
    if (log_tem == NULL) return -1.0;

    float temp;
    fscanf(log_tem, "%f", &temp);
    fclose(log_tem);
    return temp / 1000.0;
}

// 3. 写日志函数
void write_log(struct SystemState* s, const char *status) {
    FILE* log_fp = fopen("gateway.log", "a");
    if (log_fp == NULL) return;

    time_t now;
    time(&now);
    struct tm *p = localtime(&now);
    
    // 修正 tm_mday，并在末尾加上分号
    fprintf(log_fp, "%04d-%02d-%02d %02d:%02d:%02d Temp: %.2f℃ | Status: %s\n", 
            p->tm_year + 1900, p->tm_mon + 1, p->tm_mday, 
            p->tm_hour, p->tm_min, p->tm_sec, 
            s->current_temp, status);
            
    fclose(log_fp);
}

// 4. 信号处理函数
void handle_sigint(int sig) { // 补上大括号
    keep_running = 0;
    printf("\n检测到退出信号，正在关闭系统...\n");
    
    FILE* log_fp = fopen("gateway.log", "a");
    if (log_fp) {
        fprintf(log_fp, "[SHUTDOWN] System exited safely\n");
        fclose(log_fp);
    }
    
    FILE* led_file = fopen("/sys/class/leds/green:power/brightness", "w");
    if (led_file) {
        fprintf(led_file, "0");
        fclose(led_file);
    }
}

// 5. 加载配置函数
void load_config(struct SystemState *s) {
    FILE* fp = fopen("config.txt", "r");
    if (fp == NULL) {
        s->hot = 60.0;  // 修正 s->s.hot 为 s->hot
        s->warm = 45.0;
        return;
    }
    fscanf(fp, "limit_s.hot=%f\nlimit_s.warm=%f", &s->hot, &s->warm);
    fclose(fp);
}

// 6. Web 线程函数
void *web_server_thread(void* arg) {
    struct SystemState *s = (struct SystemState *)arg;
    
    while (keep_running) {
        int client_sock = accept(s->server_fd, NULL, NULL);
        if (client_sock < 0) continue;

        printf("[Web] 收到访问！当前发送温度: %.2f\n", s->current_temp);

        char response[2048]; // 稍微开大一点点，防止溢出
        
        // 注意看这里的 \r\n 布局
        snprintf(response, sizeof(response),
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/html; charset=UTF-8\r\n"
            "Connection: close\r\n"  // 必须在这里！
            "\r\n"                   // 这是 Header 和 Body 的分界线，必须有两个 \r\n
            "<html><head><meta charset='utf-8'><meta http-equiv='refresh' content='2'></head>"
            "<body style='text-align:center; font-family:sans-serif;'>"
            "<h1>Orange Pi 实时监控</h1>"
            "<div style='font-size:60px; color:#e74c3c;'>%.2f ℃</div>"
            "<p>报警阈值: %.1f | 状态: 运行中</p>"
            "</body></html>",
            s->current_temp, s->hot);

        send(client_sock, response, strlen(response), 0);
        close(client_sock); // 配合 Connection: close，完美闭环
    }
    return NULL;
}
