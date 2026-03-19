#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

int main() {
    // 1. 创建电话 (Socket)
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);

    // 2. 准备地址 (IP + Port)
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY; // 监听所有网卡
    address.sin_port = htons(8080);      // 绑定 8080 端口

    // 3. 绑定并监听
    bind(server_fd, (struct sockaddr *)&address, sizeof(address));
    listen(server_fd, 3);

    printf("Web 服务器已启动，请在浏览器访问 http://你的香橙派IP:8080\n");

    while(1) {
        // 4. 等待接客
        int new_socket = accept(server_fd, NULL, NULL);
        
        // 5. 准备要发送的 HTML 内容
        // 以后我们会把 state.current_temp 塞进这里
        char *response = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n"
                         "<h1>Hello Orange Pi!</h1>"
                         "<p>Current Status: System Running...</p>";

        // 6. 发送并挂断
        send(new_socket, response, strlen(response), 0);
        close(new_socket);
    }
    return 0;
}
