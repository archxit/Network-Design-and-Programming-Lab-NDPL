#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUF_SIZE 1024

int main() {
    int sock;
    struct sockaddr_in server_addr;
    char input[BUF_SIZE], buffer[BUF_SIZE];

    sock = socket(AF_INET, SOCK_STREAM, 0);

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr));

    printf("Input string: ");
    scanf("%s", input);

    write(sock, input, BUF_SIZE);

    for (int i = 0; i < 2; i++) {
        bzero(buffer, BUF_SIZE);
        read(sock, buffer, BUF_SIZE);
        printf("%s\n", buffer);
    }

    close(sock);
    return 0;
}
