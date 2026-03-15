#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    FILE *fp;
    // 红色状态灯的路径
    const char *path = "/sys/class/leds/red:status/brightness";
    int brightness = 0;
    int direction = 1; // 1 代表变亮，-1 代表变暗

    printf("Starting Breath LED... Press Ctrl+C to stop.\n");

    while(1) {
        fp = fopen(path, "w");
        if (fp == NULL) {
            perror("Error opening brightness file");
            return 1;
        }

        fprintf(fp, "%d", brightness);
        fclose(fp);

        // 改变亮度数值
        brightness += direction * 5; 

        // 边界判断：到达最亮或最暗时调转方向
        if (brightness >= 255) {
            brightness = 255;
            direction = -1;
        } else if (brightness <= 0) {
            brightness = 0;
            direction = 1;
        }

        // 控制呼吸频率（20毫秒更新一次）
        usleep(20000); 
    }

    return 0;
}
