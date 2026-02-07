#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUF_SIZE 1024

int main() {
    int server_fd, client_fd;
    struct sockaddr_in server_addr;
    char filename[BUF_SIZE], buffer[BUF_SIZE];

    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    listen(server_fd, 5);

    printf("TCP File Server running...\n");

    client_fd = accept(server_fd, NULL, NULL);

    while (1) {
        recv(client_fd, filename, BUF_SIZE, 0);

        if (strcmp(filename, "stop") == 0)
            break;

        FILE *fp = fopen(filename, "r");
        if (!fp) {
            strcpy(buffer, "FILE_NOT_FOUND");
            send(client_fd, buffer, BUF_SIZE, 0);
            continue;
        }

        int ch, alph=0, lines=0, spaces=0, digits=0, others=0;
        int size = 0;

        while ((ch = fgetc(fp)) != EOF) {
            size++;
            if (isalpha(ch)) alph++;
            else if (isdigit(ch)) digits++;
            else if (ch == ' ') spaces++;
            else if (ch == '\n') lines++;
            else others++;
        }

        rewind(fp);

        send(client_fd, "FILE_FOUND", BUF_SIZE, 0);
        send(client_fd, &size, sizeof(int), 0);
        send(client_fd, &alph, sizeof(int), 0);
        send(client_fd, &lines, sizeof(int), 0);
        send(client_fd, &spaces, sizeof(int), 0);
        send(client_fd, &digits, sizeof(int), 0);
        send(client_fd, &others, sizeof(int), 0);

        while (fgets(buffer, BUF_SIZE, fp))
            send(client_fd, buffer, BUF_SIZE, 0);

        strcpy(buffer, "EOF");
        send(client_fd, buffer, BUF_SIZE, 0);

        fclose(fp);
    }

    close(client_fd);
    close(server_fd);
    return 0;
}
