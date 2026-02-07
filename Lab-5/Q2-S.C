// Q2S.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>

#define PORT 8080

void sort(char s[]) {
    int i, j;
    char t;
    for (i = 0; i < strlen(s) - 1; i++)
        for (j = i + 1; j < strlen(s); j++)
            if (s[i] > s[j]) {
                t = s[i];
                s[i] = s[j];
                s[j] = t;
            }
}

int main() {
    int sfd, cfd;
    struct sockaddr_in serv, cli;
    socklen_t len = sizeof(cli);
    char buffer[200], str1[100], str2[100], result[100];

    sfd = socket(AF_INET, SOCK_STREAM, 0);

    serv.sin_family = AF_INET;
    serv.sin_addr.s_addr = INADDR_ANY;
    serv.sin_port = htons(PORT);

    bind(sfd, (struct sockaddr *)&serv, sizeof(serv));
    listen(sfd, 5);

    printf("Iterative TCP Server running...\n");

    while (1) {
        cfd = accept(sfd, (struct sockaddr *)&cli, &len);

        time_t now = time(NULL);
        printf("\nConnected to %s:%d at %s",
               inet_ntoa(cli.sin_addr),
               ntohs(cli.sin_port),
               ctime(&now));

        memset(buffer, 0, sizeof(buffer));
        recv(cfd, buffer, sizeof(buffer), 0);

        sscanf(buffer, "%s %s", str1, str2);

        if (strlen(str1) != strlen(str2)) {
            strcpy(result, "Strings are NOT anagrams");
        } else {
            sort(str1);
            sort(str2);
            if (strcmp(str1, str2) == 0)
                strcpy(result, "Strings are anagrams");
            else
                strcpy(result, "Strings are NOT anagrams");
        }

        send(cfd, result, strlen(result) + 1, 0);
        close(cfd);
    }
}
