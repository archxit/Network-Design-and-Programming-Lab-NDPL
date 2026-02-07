#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUF_SIZE 100

int main() {
    int server_fd, client_fd;
    struct sockaddr_in server_addr;
    char buffer[BUF_SIZE];

    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    listen(server_fd, 5);

    printf("Server waiting for connection...\n");

    client_fd = accept(server_fd, NULL, NULL);
    printf("Client connected.\n");

    pid_t pid = fork();

    if (pid == 0) {
        /* Child process: Receive messages */
        printf("\n[Server Child]");
        printf("\nPID: %d  PPID: %d\n", getpid(), getppid());

        while (1) {
            bzero(buffer, BUF_SIZE);
            read(client_fd, buffer, BUF_SIZE);
            if (strcmp(buffer, "bye") == 0)
                break;
            printf("Client: %s\n", buffer);
        }
    } else {
        /* Parent process: Send messages */
        printf("\n[Server Parent]");
        printf("\nPID: %d  PPID: %d\n", getpid(), getppid());

        while (1) {
            bzero(buffer, BUF_SIZE);
            printf("Server: ");
            fgets(buffer, BUF_SIZE, stdin);
            buffer[strcspn(buffer, "\n")] = 0;
            write(client_fd, buffer, BUF_SIZE);
            if (strcmp(buffer, "bye") == 0)
                break;
        }
    }

    close(client_fd);
    close(server_fd);
    return 0;
}
