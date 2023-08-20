#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

int main() {
    int client_socket;
    struct sockaddr_in server_addr;
    char buffer[1024];

    // 创建客户端Socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // 设置服务器地址
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("192.168.0.109"); // 服务器IP
    server_addr.sin_port = htons(12345); // 服务器端口

    // 连接到服务器
    if (connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }

    printf("Connected to server\n");

    while (1) {
        // 发送消息
        printf("Client: ");
        fgets(buffer, sizeof(buffer), stdin);
        if (send(client_socket, buffer, strlen(buffer), 0) == -1) {
            perror("Sending failed");
            exit(EXIT_FAILURE);
        }

        // 接收消息
        memset(buffer, 0, sizeof(buffer));
        if (recv(client_socket, buffer, sizeof(buffer), 0) == -1) {
            perror("Receiving failed");
            exit(EXIT_FAILURE);
        }

        printf("Server: %s", buffer);
    }

    // 关闭套接字
    close(client_socket);

    return 0;
}

