// server.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 3388
#define MAX 100

int main()
{
    int sockfd, newsockfd;
    struct sockaddr_in serveraddr, clientaddr;
    socklen_t len;
    char buff[MAX];
    pid_t pid;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(PORT);
    serveraddr.sin_addr.s_addr = INADDR_ANY;

    bind(sockfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
    listen(sockfd, 1);

    printf("Server waiting...\n");
    fflush(stdout);

    len = sizeof(clientaddr);
    newsockfd = accept(sockfd, (struct sockaddr*)&clientaddr, &len);

    pid = fork();

    /* CHILD → RECEIVE */
    if (pid == 0) {
        while (1) {
            int n = recv(newsockfd, buff, MAX - 1, 0);
            if (n <= 0) break;

            buff[n] = '\0';  // IMPORTANT
            write(1, "Client: ", 8);
            write(1, buff, strlen(buff));

            if (!strncmp(buff, "exit", 4))
                break;
        }
        exit(0);
    }

    /* PARENT → SEND */
    else {
        while (1) {
            fgets(buff, MAX, stdin);
            send(newsockfd, buff, strlen(buff), 0);

            if (!strncmp(buff, "exit", 4))
                break;
        }
    }

    close(newsockfd);
    close(sockfd);
    return 0;
}
