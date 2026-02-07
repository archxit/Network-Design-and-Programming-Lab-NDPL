// server.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080

int main() {
    int sfd, cfd;
    struct sockaddr_in serv, cli;
    socklen_t len = sizeof(cli);
    char buffer[300], response[500];
    int option;

    sfd = socket(AF_INET, SOCK_STREAM, 0);

    serv.sin_family = AF_INET;
    serv.sin_addr.s_addr = INADDR_ANY;
    serv.sin_port = htons(PORT);

    bind(sfd, (struct sockaddr *)&serv, sizeof(serv));
    listen(sfd, 5);

    printf("TCP Server running...\n");

    while (1) {
        cfd = accept(sfd, (struct sockaddr *)&cli, &len);
        recv(cfd, buffer, sizeof(buffer), 0);

        sscanf(buffer, "%d", &option);

        pid_t pid = fork();

        if (pid == 0) {   // Child process
            if (option == 1) {
                sprintf(response,
                        "Child PID: %d\nName: Archit\nAddress: Manipal, Karnataka",
                        getpid());
            } else if (option == 2) {
                sprintf(response,
                        "Child PID: %d\nDept: CSE\nSemester: 4\nSection: A\nCourses: OS, CN, DBMS",
                        getpid());
            } else if (option == 3) {
                sprintf(response,
                        "Child PID: %d\nMarks Obtained: 88",
                        getpid());
            } else {
                sprintf(response, "Invalid Option");
            }

            send(cfd, response, strlen(response) + 1, 0);
            close(cfd);
            exit(0);
        }
    }
}
