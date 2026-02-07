#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUF_SIZE 2048

int main() {
    int sock;
    struct sockaddr_in server_addr;
    char buffer[BUF_SIZE];
    char result[BUF_SIZE * 2];

    sock = socket(AF_INET, SOCK_STREAM, 0);

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr));
    printf("Connected to server.\n");

    while (1) {
        printf("\nEnter string (or stop): ");
        scanf("%s", buffer);

        write(sock, buffer, BUF_SIZE);

        if (strcmp(buffer, "stop") == 0)
            break;

        bzero(result, sizeof(result));
        read(sock, result, sizeof(result));

        printf("\nPermutations received from server:\n");
        printf("%s", result);
    }

    close(sock);
    return 0;
}
