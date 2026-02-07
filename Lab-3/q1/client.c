#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUF_SIZE 100

int main() {
    int sock;
    struct sockaddr_in server_addr;
    char buffer[BUF_SIZE];

    sock = socket(AF_INET, SOCK_STREAM, 0);

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr));
    printf("Connected to server.\n");

    pid_t pid = fork();

    if (pid == 0) {
        /* Child process: Receive messages */
        printf("\n[Client Child]");
        printf("\nPID: %d  PPID: %d\n", getpid(), getppid());

        while (1) {
            bzero(buffer, BUF_SIZE);
            read(sock, buffer, BUF_SIZE);
            if (strcmp(buffer, "bye") == 0)
                break;
            printf("Server: %s\n", buffer);
        }
    } else {
        /* Parent process: Send messages */
        printf("\n[Client Parent]");
        printf("\nPID: %d  PPID: %d\n", getpid(), getppid());

        while (1) {
            bzero(buffer, BUF_SIZE);
            printf("Client: ");
            fgets(buffer, BUF_SIZE, stdin);
            buffer[strcspn(buffer, "\n")] = 0;
            write(sock, buffer, BUF_SIZE);
            if (strcmp(buffer, "bye") == 0)
                break;
        }
    }

    close(sock);
    return 0;
}
    