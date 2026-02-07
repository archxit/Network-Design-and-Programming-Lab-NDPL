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
    char filename[BUF_SIZE], buffer[BUF_SIZE];

    sock = socket(AF_INET, SOCK_STREAM, 0);

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr));

    while (1) {
        printf("\nEnter filename (or stop): ");
        scanf("%s", filename);

        send(sock, filename, BUF_SIZE, 0);

        if (strcmp(filename, "stop") == 0)
            break;

        recv(sock, buffer, BUF_SIZE, 0);

        if (strcmp(buffer, "FILE_NOT_FOUND") == 0) {
            printf("File not found on server.\n");
            continue;
        }

        int size, alph, lines, spaces, digits, others;

        recv(sock, &size, sizeof(int), 0);
        recv(sock, &alph, sizeof(int), 0);
        recv(sock, &lines, sizeof(int), 0);
        recv(sock, &spaces, sizeof(int), 0);
        recv(sock, &digits, sizeof(int), 0);
        recv(sock, &others, sizeof(int), 0);

        printf("\nFile Size: %d bytes", size);
        printf("\nAlphabets: %d", alph);
        printf("\nLines: %d", lines);
        printf("\nSpaces: %d", spaces);
        printf("\nDigits: %d", digits);
        printf("\nOther Characters: %d\n", others);
        printf("\n--- File Content ---\n");

        while (1) {
            recv(sock, buffer, BUF_SIZE, 0);
            if (strcmp(buffer, "EOF") == 0)
                break;
            printf("%s", buffer);
        }
    }

    close(sock);
    return 0;
}
