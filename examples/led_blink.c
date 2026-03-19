#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    // 使用你看到的红灯路径
    const char *led_on  = "echo 1 | sudo tee /sys/class/leds/red:status/brightness > /dev/null";
    const char *led_off = "echo 0 | sudo tee /sys/class/leds/red:status/brightness > /dev/null";
    
    printf("Starting Red LED blink challenge on Orange Pi Zero3...\n");

    for(int i = 0; i < 10; i++) {
        system(led_on);
        printf("LED ON (Red)\n");
        sleep(1);

        system(led_off);
        printf("LED OFF (Red)\n");
        sleep(1);
    }

    printf("Challenge completed!\n");
    return 0;
}
