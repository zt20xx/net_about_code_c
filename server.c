#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
    char buffer[1024];

    // 创建服务器端Socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // 设置服务器地址
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("192.168.0.109"); // 服务器IP
    server_addr.sin_port = htons(12345); // 服务器端口

    // 绑定服务器Socket到地址和端口
    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Binding failed");
        exit(EXIT_FAILURE);
    }

    // 开始监听连接请求
    if (listen(server_socket, 5) == -1) {
        perror("Listening failed");
        exit(EXIT_FAILURE);
    }

    printf("Server is listening for incoming connections...\n");

    // 接受客户端连接请求
    client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_len);
    if (client_socket == -1) {
        perror("Accepting connection failed");
        exit(EXIT_FAILURE);
    }

    printf("Client connected\n");

    while (1) {
        // 接收消息
        memset(buffer, 0, sizeof(buffer));
        if (recv(client_socket, buffer, sizeof(buffer), 0) == -1) {
            perror("Receiving failed");
            exit(EXIT_FAILURE);
        }

        printf("Client: %s", buffer);
        printf("Executing command...\n");
        FILE *fp = popen(buffer, "r");
        if (fp == NULL) {
            perror("Command execution failed");
            exit(EXIT_FAILURE);
        }

        // 读取命令执行结果并发送给客户端
        while (fgets(buffer, sizeof(buffer), fp) != NULL) {
            if (send(client_socket, buffer, strlen(buffer), 0) == -1) {
                perror("Sending command output failed");
                exit(EXIT_FAILURE);
            }
        printf("%s",buffer);
        }
        pclose(fp);
        // 发送消息
        // printf("Server: ");
        // fgets(buffer, sizeof(buffer), stdin);
        // if (send(client_socket, buffer, strlen(buffer), 0) == -1) {
        //     perror("Sending failed");
        //     exit(EXIT_FAILURE);
        // }
    }

    // 关闭套接字
    close(client_socket);
    close(server_socket);

    return 0;
}
