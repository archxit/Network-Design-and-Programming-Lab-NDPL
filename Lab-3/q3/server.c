#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <ctype.h>

#define PORT 8080
#define BUF_SIZE 1024

void sort_asc(char *arr) {
    int n = strlen(arr);
    for (int i = 0; i < n - 1; i++)
        for (int j = i + 1; j < n; j++)
            if (arr[i] > arr[j]) {
                char t = arr[i];
                arr[i] = arr[j];
                arr[j] = t;
            }
}

void sort_desc(char *arr) {
    int n = strlen(arr);
    for (int i = 0; i < n - 1; i++)
        for (int j = i + 1; j < n; j++)
            if (arr[i] < arr[j]) {
                char t = arr[i];
                arr[i] = arr[j];
                arr[j] = t;
            }
}

int main() {
    int server_fd, client_fd;
    struct sockaddr_in server_addr;
    char input[BUF_SIZE];

    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    listen(server_fd, 5);

    printf("Server waiting for connection...\n");
    client_fd = accept(server_fd, NULL, NULL);

    read(client_fd, input, BUF_SIZE);
    printf("Received string: %s\n", input);

    pid_t pid = fork();

    if (pid == 0) {
        /* CHILD PROCESS → digits ascending */
        char digits[BUF_SIZE] = "";
        for (int i = 0; input[i]; i++)
            if (isdigit(input[i]))
                strncat(digits, &input[i], 1);

        sort_asc(digits);

        char msg[BUF_SIZE];
        sprintf(msg,
                "Output at the child process of the server (PID %d): %s",
                getpid(), digits);

        write(client_fd, msg, BUF_SIZE);
        exit(0);
    } else {
        /* PARENT PROCESS → characters descending */
        wait(NULL);

        char chars[BUF_SIZE] = "";
        for (int i = 0; input[i]; i++)
            if (isalpha(input[i]))
                strncat(chars, &input[i], 1);

        sort_desc(chars);

        char msg[BUF_SIZE];
        sprintf(msg,
                "Output at the parent process of the server (PID %d): %s",
                getpid(), chars);

        write(client_fd, msg, BUF_SIZE);
    }

    close(client_fd);
    close(server_fd);
    return 0;
}
