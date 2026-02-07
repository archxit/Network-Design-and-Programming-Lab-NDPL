// client2.c
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080

int main() {
    int sock;
    struct sockaddr_in server_addr;
    char buffer[1024];

    sock = socket(AF_INET, SOCK_STREAM, 0);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);

    connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr));

    send(sock, "Technology", 10, 0);

    recv(sock, buffer, sizeof(buffer), 0);
    if (strlen(buffer) > 0)
        printf("Server: %s\n", buffer);

    close(sock);
    return 0;
}
