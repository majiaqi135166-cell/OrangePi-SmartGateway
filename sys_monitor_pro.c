#include <stdio.h>   // 标准输入输出库：处理 printf, fopen, fscanf 等
#include <stdlib.h>  // 标准工具库：处理一些通用工具函数
#include <time.h>    // 时间库：处理系统时间获取和转换
#include <unistd.h>  // Unix 标准库：处理 sleep 等系统调用

// 定义一个函数，专门负责“读取温度并写入日志”
void log_temperature() {
    // 【fopen】打开文件。"/sys..." 是 Linux 内核存放 CPU 温度的虚拟文件。
    // "r" 代表只读模式 (read)
    FILE *temp_fp = fopen("/sys/class/thermal/thermal_zone0/temp", "r");
    
    // 【fopen】打开日志文件。"a" 代表追加模式 (append)。
    // 如果文件不存在，它会自动创建；如果已存在，新内容会写在旧内容后面。
    FILE *log_fp = fopen("temp.log", "a");

    // 安全检查：如果两个文件都成功打开了（指针不为 NULL），才执行操作
    if (temp_fp && log_fp) {
        float temp;
        // 【fscanf】从文件中读取数据。就像 scanf 从键盘读一样，它从 temp_fp 指向的文件读。
        // 读取一个浮点数存入 temp 变量中。
        fscanf(temp_fp, "%f", &temp);
        
        // --- 处理时间逻辑 ---
        
        // 【time_t】这是一种数据类型，专门用来存储从1970年到现在经过的秒数（时间戳）。
        time_t rawtime; 
        
        // 【struct tm】这是一个结构体，里面包含年、月、日、时、分、秒等成员。
        struct tm *timeinfo; 
        
        // 【time()】函数：获取当前系统的原始时间戳，存入 rawtime。
        time(&rawtime); 
        
        // 【localtime()】函数：把原始秒数（时间戳）转换成我们人类能看懂的当地时间。
        timeinfo = localtime(&rawtime);
        
        // --- 写入与输出 ---
        
        // 【fprintf】文件打印。格式化输出到 log_fp 指向的日志文件中。
        // temp / 1000.0 是因为内核给的温度是整数（如45000），除以1000才是摄氏度。
        fprintf(log_fp, "[%02d:%02d:%02d] CPU Temp: %.2f°C\n", 
                timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec, temp / 1000.0);
        
        // 【printf】在你的香橙派终端窗口打印一行提示，告诉你程序还在正常跑。
        printf("Log updated at %02d:%02d:%02d\n", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
        
        // 【fclose】关闭文件。这是非常重要的习惯，就像用完水龙头要关水，防止内存泄漏。
        fclose(temp_fp);
        fclose(log_fp);
    }
}

int main() {
    // 启动提示
    printf("Logging temperature every 10s... Press Ctrl+C to stop.\n");
    
    // 【while(1)】死循环。让程序一直运行，直到你手动停止。
    while(1) {
        log_temperature(); // 调用上面写好的逻辑
        
        // 【sleep(10)】让程序“睡”10秒钟。
        // 如果不加这一行，CPU会全速运行，既浪费电也会让日志瞬间填满。
        sleep(10); 
    }
    
    return 0; // 程序的正常退出标志
}
