// client.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 3388
#define MAXSIZE 100

int main()
{
    int sockfd;
    struct sockaddr_in serveraddr;
    char buff[MAXSIZE];
    pid_t pid;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Socket error");
        exit(1);
    }

    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(PORT);
    serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(sockfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr));

    pid = fork();

    /* CHILD → RECEIVE */
    if (pid == 0) {
        while (1) {
            memset(buff, 0, MAXSIZE);
            if (recv(sockfd, buff, MAXSIZE, 0) <= 0)
                break;

            printf("Server: %s", buff);

            if (strncmp(buff, "exit", 4) == 0)
                break;
        }
        exit(0);
    }

    /* PARENT → SEND */
    else {
        while (1) {
            memset(buff, 0, MAXSIZE);
            fgets(buff, MAXSIZE, stdin);
            send(sockfd, buff, strlen(buff), 0);

            if (strncmp(buff, "exit", 4) == 0)
                break;
        }
    }

    close(sockfd);
    return 0;
}
