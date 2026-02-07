#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUF_SIZE 2048

void swap(char *a, char *b) {
    char temp = *a;
    *a = *b;
    *b = temp;
}

void permute(char *str, int l, int r, char *result) {
    if (l == r) {
        strcat(result, str);
        strcat(result, "\n");
        return;
    }

    for (int i = l; i <= r; i++) {
        swap(&str[l], &str[i]);
        permute(str, l + 1, r, result);
        swap(&str[l], &str[i]);   // backtrack
    }
}

int main() {
    int server_fd, client_fd;
    struct sockaddr_in server_addr;
    char buffer[BUF_SIZE];
    char result[BUF_SIZE * 2];

    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    listen(server_fd, 5);

    printf("TCP Server waiting for connection...\n");

    client_fd = accept(server_fd, NULL, NULL);
    printf("Client connected.\n");

    while (1) {
        bzero(buffer, BUF_SIZE);
        read(client_fd, buffer, BUF_SIZE);

        if (strcmp(buffer, "stop") == 0)
            break;

        printf("\nReceived string: %s\n", buffer);
        printf("Permutations:\n");

        bzero(result, sizeof(result));
        permute(buffer, 0, strlen(buffer) - 1, result);

        printf("%s", result);

        write(client_fd, result, sizeof(result));
    }

    close(client_fd);
    close(server_fd);
    return 0;
}
